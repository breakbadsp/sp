#pragma once

#include <cassert>
#include <cstdlib>
#include "Hashing.hpp"

const auto cache_line_size = std::hardware_destructive_interference_size;
namespace sp {
template<typename KeyType, typename ValueType>
class MyHashSet {
public:
    static constexpr double load_fact = 0.5;
    static constexpr bool debug = false;
    constexpr MyHashSet() = default;
    constexpr MyHashSet(size_t p_cap) {
        assert(p_cap != 0);
        rehash(p_cap);
    }

    MyHashSet& operator=(MyHashSet&& p_rhs) noexcept {
        if(this == &p_rhs)
            return *this;

        for (size_t i = 0; i < capacity_; ++i) {
            Node* node = (store_ + i);
            if(node->state == Node::State::Allocated) {
                node->~Node(); // Call destructor explicitly
            }
        }
        std::free(store_);
        store_ = p_rhs.store_;
        size_ = p_rhs.size_;
        capacity_ = p_rhs.capacity_;
        p_rhs.store_ = nullptr;
        p_rhs.size_ = 0;
        p_rhs.capacity_ = 0;
        return *this;
    }

    ~MyHashSet() {
        for (size_t i = 0; i < capacity_; ++i) {
            Node* node = (store_ + i);
            if(node->state == Node::State::Allocated) {
                node->~Node(); // Call destructor explicitly
            }
        }
        std::free(store_);
        store_ = nullptr;
    }
    
    bool add(const std::pair<KeyType, ValueType>& p_ele) {
        if(size_ >= static_cast<size_t>(load_fact * capacity_)) {
            rehash(capacity_ * 2);
        }

        std::size_t insert_hash = 0;
        if(find(p_ele.first, insert_hash)) {
            return false;
        }
        Node* node = (store_ + insert_hash);
        node->ele = p_ele;
        node->state = Node::State::Allocated;
        size_++;
        return true;
    }
    
    void remove(KeyType p_key) {
        std::size_t index = 0;
        if(!find(p_key, index)) {
            return;
        }
        Node* node = (store_ + index);
        node->state = Node::State::Deallocated;
        size_--;
        return; 
    }
    
    bool contains(KeyType p_key) {
        std::size_t index = 0;
        return find(p_key, index);
    }

    ValueType& operator[](const KeyType& p_key) {
        if(size_ >= (load_fact * capacity_)) {
            rehash(capacity_ * 2);
        }

        std::size_t insert_hash = 0;
        if(find(p_key, insert_hash)) {
            Node* node = (store_ + insert_hash);
            return node->ele.second();
        }

        Node* node = (store_ + insert_hash);
        new (node) Node(p_key, ValueType());
        node->state = Node::State::Allocated;
        size_++;
        if(debug) std::cout << " added " << p_key << ", size=" << size_ << '\n';
        return node->ele.second();
    }

    bool find(KeyType p_key, std::size_t& index) {
        if(capacity_ == 0)
            return false;
        
        //auto hash = sp::MultiplicationHash(p_key, capacity_);
        auto hash = sp::Fnv1aHash(p_key) % capacity_;//, capacity_);
        //if(debug) std::cout << "for key:" << p_key << " has is :" << hash << '\n';
        index = hash;
        
        if(hash >= capacity_)
            return false;

        Node* node = (store_ + hash);
        Node* orign = node;
        while(node->state != Node::State::UnAllocated) {
            if(node->ele.key == p_key && node->state == Node::State::Allocated) {
                index = hash;
                //assert(index != 0);
                assert(index < capacity_);
                return true;
            }
            hash = (hash + 1) % capacity_; 
            node = store_ + hash;
            index = hash;
            if(node == orign)
                break;
        }
        
        //assert(index != 0);
        assert(index < capacity_);

        return false;
    }

    size_t size() const { return size_; }
    size_t capacity() const { return capacity(); }

private:
    struct Node {
        enum class State : uint8_t {
            UnAllocated,
            Allocated,
            Deallocated,
        };
    
        State state = State::UnAllocated;
        //sp::KeyValuePair<KeyType, ValueType> ele;
        sp::Pair<KeyType, ValueType> ele;

        Node() = default;
        Node(const Node& p_rhs) = default;
        Node(Node&& p_rhs) = default;
        Node& operator=(const Node& p_rhs) = default;
        Node& operator=(Node&& p_rhs) = default;

        Node(std::pair<KeyType, ValueType> p_el) 
            : ele(p_el)
            , state(State::UnAllocated) 
            {}
        Node(const KeyType& p_key, const ValueType& p_value)
            : state(State::UnAllocated)
            , ele(p_key, p_value)
            {}
    };

    Node* store_ = nullptr;
    sp::size_t size_ = 0;
    sp::size_t capacity_ = 0;

    constexpr void rehash(sp::size_t new_cap) {
        /*std::cout << "rehashing to size " << new_cap << '\n';

        // Calculate nodes per cache line
        static constexpr size_t nodes_per_cache_line =
            cache_line_size / sizeof(Node);
        std::cout << "Node size " << sizeof(Node) << ", "
            << "cache line size: " << cache_line_size << ", "
            << "nodes per cache line: " << nodes_per_cache_line << std::endl;*/

        if(new_cap == 0)
            new_cap = 2;

        Node* new_store = (Node*) (std::malloc(new_cap * sizeof(Node)));

        /*size_t aligned_size = ((new_cap * sizeof(Node) + cache_line_size - 1) / cache_line_size) * cache_line_size;
        Node* new_store = (Node*) (std::aligned_alloc(cache_line_size, aligned_size));
        new_cap = aligned_size / sizeof(Node);
        memset(new_store, 0, aligned_size);*/

        memset(new_store, 0, new_cap * sizeof(Node));
        for (size_t i = 0; i < new_cap; ++i) {
            (new_store + i)->state = Node::State::UnAllocated;
        }

        if(!store_) {
            store_ = new_store;
            capacity_ = new_cap;
            size_ = 0;
            if(debug) std::cout << "Create hashtable of to size " << new_cap << '\n';
            return;
        }
        
        for(sp::size_t i = 0; i < capacity_; ++i) {
            Node* old_node = (store_ + i);
            if(old_node->state == Node::State::Allocated) {
                //std::size_t new_hash = sp::MultiplicationHash(old_ele->ele.first, new_cap);
                auto new_hash = sp::Fnv1aHash(old_node->ele.key) % new_cap;
                Node* new_node = (new_store + new_hash);
                while(new_node->state != Node::State::UnAllocated) {
                    new_hash = (new_hash + 1) % new_cap;
                    new_node = new_store + new_hash;
                }
                new (new_node) Node(std::move(*old_node));
                new_node->state = Node::State::Allocated;
                continue;
            }
        }
        //TODO:: Clean up old store with destructors
        for(sp::size_t i = 0; i < capacity_; ++i) {
            Node* old_node = (store_ + i);
            if(old_node->state == Node::State::Allocated) {
                old_node->~Node(); // Call destructor explicitly
            }
        }
        std::free(store_);

        capacity_ = new_cap;
        store_ = new_store;
        if(debug) std::cout << "rehash to size " << new_cap << '\n';
    }

public:
    struct Iterator {
        Iterator() = delete;
        Iterator(const MyHashSet& set, bool end = false) 
            : set_(set)
            , indx_(__INT_MAX__) {
            if(end)
                return;

            indx_ = 0;
            Node* node = set_.store_;
            while(node->state != Node::State::Allocated) {
                node++;
                indx_ = (indx_+1) % set_.capacity_;
                if(indx_ == 0)
                {
                    indx_ = __INT_MAX__;
                    break;
                }
            }
        }

        bool operator!=(const Iterator& p_rhs) const {
            return indx_ != p_rhs.indx_;
        }

        auto& operator*() {
            Node* node = set_.store_ + indx_;
            return node->ele;
        }

        auto* operator->() {
            Node* node = set_.store_ + indx_;
            return &node->ele;
        }

        Iterator& operator++() {
            indx_ = (indx_ + 1);
            Node* node = set_.store_ + indx_;

            while((indx_ != set_.capacity_) && (node->state != Node::State::Allocated) ) {
                indx_ = (indx_ + 1);
                node = set_.store_ + indx_;            
            }

            if(indx_ == set_.capacity_) {
                indx_ = __INT_MAX__;
                if(debug) std::cout << "End reached " << '\n';
            }

            return *this;
        }

        /*Iterator& operator--() {
            auto orig = indx_;
            indx_ = (indx_ - 1) % capacity_;            
            Node* node = set.store_ + indx_;

            while((indx_ != orig) && (node->state != Node::State::Allocated) ) {
                indx_ = (indx_ + 1) % capacity_;
                node = set.store_ + indx_;            
            }
            return *this;
        }*/

        const MyHashSet& set_;
        size_t indx_ = set_.capacity_;
    };

public:
    Iterator begin() {
        return Iterator(*this);
    }

    Iterator end() {
        return Iterator(*this, true);
    }
};
}//sp