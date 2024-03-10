#pragma once

#include <cstdint>

#include "Hashing.hpp"

namespace sp
{

constexpr float hash_table_load_factor = 0.6;
constexpr size_t hash_table_default_size = 17;

//Impliment hash table iterators and use them instead of indexes

enum class HashTableInsertResult
{
  Success
  , Failure
  , Overwrite
};

enum class BucketState
{
  Unused = 0
 , Used
 , Deleted
};

template<typename HashTableType, typename BucketType, typename T>
class HashTableIterator
{
  friend HashTableType;
public:
  HashTableIterator() = default;
  
  HashTableIterator(decltype(nullptr)) { bucket_ptr_ = nullptr; } //end
  HashTableIterator(BucketType* p_bucket) { bucket_ptr_ = p_bucket; }
  
  HashTableIterator(const HashTableIterator& p_other)
  : bucket_ptr_(p_other.bucket_ptr_)
  { }

  HashTableIterator(HashTableIterator&& p_other)
  : bucket_ptr_(p_other.bucket_ptr_)
  { 
    p_other.bucket_ptr_ = nullptr;
  }
  
  HashTableIterator& operator=(const HashTableIterator& p_other)
  {
    if(this == &p_other)
      return *this;
    bucket_ptr_ = p_other.bucket_ptr_;
    p_other.bucket_ptr_ = nullptr;
    return *this;
  }

  HashTableIterator& operator=(HashTableIterator&& p_other)
  {
    if(this == &p_other)
      return *this;
    bucket_ptr_ = p_other.bucket_ptr_;
    p_other.bucket_ptr_ = nullptr;
    return *this;
  }
  
  T& operator*() { return *(bucket_ptr_->data()); }
  T& operator->() { return *(bucket_ptr_->data()); }

  void operator++() { SkipToNext(); }
  bool operator==(const HashTableIterator& p_other) { return bucket_ptr_ == p_other.bucket_ptr_; }
  bool operator!=(const HashTableIterator& p_other) { return bucket_ptr_ != p_other.bucket_ptr_; }

  //TODO:: next, prev, advance(index) etc

private:
  void SkipToNext()
  {
    while( bucket_ptr_ != nullptr && 
           bucket_ptr_->state_ != BucketState::Used )
    {
      ++bucket_ptr_;
    }
  }

private:
  BucketType* bucket_ptr_ {nullptr};
};

// TODO:: Impliment const iterator

template<typename T>
class HashTable
{
public:
  struct Bucket
  {
    BucketState state_ {BucketState::Unused};
    alignas(T) uint8_t data_[sizeof(T)]; //array of sizeof(T) bytes
    T* data() { return reinterpret_cast<T*>(data_); }
    const T* data() const { return reinterpret_cast<const T*>(data_); }
  };

  using BucketType = Bucket;

  HashTable() = default;
  HashTable(size_t p_capacity) { Rehash(p_capacity); }

  HashTableInsertResult Insert(const T& p_data) //TODO:: use forwarding reference
  {
    if(capacity_ < hash_table_default_size)
    {
      Rehash(hash_table_default_size);
    }
    else if(size_ >= (size_t)(capacity_ * hash_table_load_factor))
    {
      Rehash(capacity_ * 2);
    }

    const auto hash = sp::Hash(p_data, capacity_);
    auto itr = LookupWithHash(hash);
    if(itr == end())
    {
      new (itr->data()) T(p_data);//TODO:: use forwarding reference
      itr->state_ = Bucket::State::Used;
      ++size_;
      return HashTableInsertResult::Success;
    }
    return HashTableInsertResult::Failure;
  }
  
  HashTableInsertResult InsertOrAssign(T p_data) //TODO:: use forwarding reference
  {
    if(capacity_ < hash_table_default_size)
    {
      Rehash(hash_table_default_size);
    }
    else if(size_ >= (size_t)(capacity_ * hash_table_load_factor))
    {
      Rehash(capacity_ * 2);
    }

    const auto hash = sp::Hash(p_data, capacity_);
    auto itr = LookupWithHash(hash);
    if(itr == end())
    {
      itr = &buckets_[hash];
    }
    
    auto& found_bucket = *itr;
    switch (found_bucket.state_)
    {
      case Bucket::State::Unused:
      case Bucket::State::Deleted:
      {
        new (found_bucket.data()) T(p_data);//TODO:: use forwarding reference
        found_bucket.state_ = Bucket::State::Used;
        ++size_;
        return HashTableInsertResult::Success;
      }

      case Bucket::State::Used:
      {
        found_bucket.data()->~T();
        new (found_bucket.data()) T(p_data);//TODO:: use forwarding reference
        found_bucket.state_ = Bucket::State::Used;
        return HashTableInsertResult::Overwrite;
      }
    }
    return HashTableInsertResult::Failure;
  }

  using Iterator = HashTableIterator<HashTable, BucketType, T>; 

  Iterator Find(const T& p_data)
  {
    return LookupWithHash(sp::Hash(p_data, capacity_));
  }

  bool Erase(T p_key)
  {
    const auto itr = Find(p_key);
    if(itr != end())
    {
      Erase(itr);
      return true;
    }
    return false;
  }

  void Erase(Iterator p_itr)
  {
    p_itr->bucket_ptr_->~T();
    p_itr->bucket_ptr_->state_ = Bucket::State::Deleted;
    --size_;
  }

  //iterators
  Iterator begin()
  {
    for(size_t i = 0; i < capacity_; ++i)
    {
      if(buckets_[i].state_ == Bucket::State::Used)
      {
        return &buckets_[i];
      }
    }
    return end();
  }

  Iterator end()
  {
    return nullptr;
  }

private:
  void Rehash(size_t p_capacity)
  {
    //TODO:: Choose table capacity as prime number
    if( p_capacity < hash_table_default_size)
      p_capacity = hash_table_default_size;

    auto* old_buckets = buckets_;
    buckets_ = new Bucket[p_capacity]; //TODO:: Handle calloc failure
    capacity_ = p_capacity;

    if(old_buckets)
    {
      for(auto i = 0; i < size_; ++i)
      {
        buckets_[i] = sp::move(old_buckets[i]); // TODO:: Handle move failure, restore to original
        old_buckets[i]->~T();
      }
      free(old_buckets);
    }
  }
  
  Iterator LookupWithHash(size_t p_hash, const T& p_data)
  {
    Bucket& found_bucket = buckets_[p_hash];
    while(true)
    {
      switch(found_bucket.state_)
      {
        case Bucket::State::Used:
        {
          if(*(found_bucket.data()) == p_data)
            return &found_bucket;
          break;
        }

        case Bucket::State::Deleted:
          break;
      
        case Bucket::State::Unused:
          return end();
      }
      //collision, resolved by linear probing
      p_hash = (p_hash + 1) % capacity_; 
    }
    return end();
  }


private:
  Bucket* buckets_ {nullptr};
  size_t capacity_ {0};
  size_t size_ {0};
};
};