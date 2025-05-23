#pragma once

#include <utility>
#include <cstdlib>
#include <iostream>
#include <cassert>

namespace sp {
template<typename T>
class vector {
public:
    constexpr vector() = default;
    ~vector() { clear(); }

    vector(size_t p_capacity) 
    : capacity_(p_capacity)
    , size_(0) {
        buffer_ = reinterpret_cast<T*>(malloc(sizeof(T) * capacity_));
    }

    vector(const vector& p_rhs) 
    : capacity_(p_rhs.capacity_)
    , size_(p_rhs.size_) {
        if(capacity_ > 0) {
            buffer_ = reinterpret_cast<T*>(malloc(sizeof(T) * capacity_));
            for(size_t i = 0; i < size_; ++i) {
                new (slot(i)) T(*p_rhs.slot(i));
            }
        }
    }

    vector(vector&& p_rhs) noexcept
    : buffer_(p_rhs.buffer_)
    , capacity_(p_rhs.capacity_)
    , size_(p_rhs.size_) {
        p_rhs.buffer_ = nullptr;
        p_rhs.capacity_ = 0;
        p_rhs.size_ = 0;
    }

    vector& operator=(const vector& p_rhs) {
        if(this != &p_rhs) {
            if (!p_rhs.buffer_) {
                clear();
                return *this;
            }

            auto* new_buffer = reinterpret_cast<T*>(malloc(sizeof(T) * p_rhs.capacity_));
            for(size_t i = 0; i < p_rhs.size_; ++i) {
                new (&new_buffer[i]) T(*p_rhs.slot(i));
            }
            clear();
            capacity_ = p_rhs.capacity_;
            size_ = p_rhs.size_;
            buffer_ = reinterpret_cast<T*>(new_buffer);
        }
        return *this;
    }

    vector& operator=(vector&& p_rhs) noexcept {
        if(this != &p_rhs) {
            clear();
            buffer_ = p_rhs.buffer_;
            capacity_ = p_rhs.capacity_;
            size_ = p_rhs.size_;        
            p_rhs.buffer_ = nullptr;
            p_rhs.capacity_ = 0;
            p_rhs.size_ = 0;
        }
        return *this;
    }

    const T& operator[](size_t p_idx) const { return *slot(p_idx); }      
    T& operator[](size_t p_idx) { return *slot(p_idx); }

    void push_back(const T& p_rhs) {
        if(size_ == capacity_) {
            reallocate(capacity_ * 2);
        }
        new (slot(size_)) T(p_rhs);
        ++size_;
    }

    void push_back(T&& p_rhs) {
        if(size_ >= capacity_) {
            reallocate(capacity_ * 2);
        }
        new (slot(size_)) T(std::move(p_rhs));
        ++size_;
    }

    T pop_back() {
        assert(size_ > 0);
        auto ele = std::move(*last());
        last()->~T();
        --size_;
        return ele;
    }

    const T& back() const { return *last(); }
    T& back() { return *last(); }

    size_t size() const noexcept { return size_; }
    size_t capacity() const noexcept { return capacity_; }

    T* data() { return reinterpret_cast<T*>(buffer_);  }
    const T* data() const { return reinterpret_cast<T*>(buffer_);  }

    bool empty() const noexcept { return size() == 0; }

private:
    T* buffer_ = nullptr;
    size_t capacity_ {0};
    size_t size_ {0};

    void reallocate(size_t p_new_cap) {
        if(p_new_cap == 0) p_new_cap = 4;
        T* new_buff = reinterpret_cast<T*>(malloc(sizeof(T) * p_new_cap));
        for(size_t i = 0; i < size_; ++i) {
            new (&new_buff[i]) T(*slot(i));
        }

        capacity_ = p_new_cap;
        clear();
        buffer_ = reinterpret_cast<T*>(new_buff);
    }

    T* slot(size_t idx ) {
        return reinterpret_cast<T*>(buffer_ + idx);
    }

    const T* slot(size_t idx ) const {
        return reinterpret_cast<T*>(buffer_ + idx);
    }

    const T* last() const { return slot(size_ - 1); }
    T* last() { return slot(size_ - 1); }


    void clear() {
        for(size_t i = 0; i < size_; ++i) {
            slot(i)->~T();
        }
        free(buffer_);
        buffer_ = nullptr;
    }
};
}
