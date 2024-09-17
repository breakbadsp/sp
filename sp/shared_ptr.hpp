#pragma once

#include <atomic>
#include <shared_mutex>
#include <mutex>

#include "cmn.hpp"

namespace sp
{
template<class T>
class shared_ptr {
public:
    shared_ptr() = default;
    ~shared_ptr() {
        dec_ref_count();
    }
    
    explicit shared_ptr(T* p_ptr) 
    : raw_ptr_ (p_ptr) 
    , cb_(new CB()) {
        inc_ref_count();
    }

    shared_ptr(const shared_ptr& p_rhs)
    : raw_ptr_(p_rhs.raw_ptr_) 
    , cb_(p_rhs.cb_) {
        inc_ref_count();
    }

    shared_ptr(shared_ptr&& p_rhs)
    : raw_ptr_(p_rhs.raw_ptr_) 
    , cb_(p_rhs.cb_) {
        p_rhs.raw_ptr_ = nullptr;
        p_rhs.cb_ = nullptr;
    }

    shared_ptr& operator=(const shared_ptr& p_rhs) {
        if(this == &p_rhs)
            return *this;

        raw_ptr_ = p_rhs.raw_ptr_;
        cb_ = p_rhs.cb_;
        inc_ref_count();

        return *this;
    }

    shared_ptr& operator=(shared_ptr&& p_rhs) {
        if(this == &p_rhs)
            return *this;
        
        raw_ptr_ = p_rhs.raw_ptr_;
        cb_ = p_rhs.cb_;
        p_rhs.raw_ptr_ = nullptr;
        p_rhs.cb_ = nullptr;

        return *this;
    }

     T* operator->() { return get(); }
    const T* operator->() const { return get(); }

    T& operator*() { return *get(); }
    const T& operator*() const { return *get(); }

    T* get() { return raw_ptr_; }
    const T* get() const { return raw_ptr_; }

    size_t use_count() const { return cb_->ref_count_.load(); }


    void reset(T* p_ptr = nullptr) {
        dec_ref_count();

        raw_ptr_ = p_ptr;
        if(raw_ptr_) {
            cb_ = new CB();
            inc_ref_count();
        }
    }

    
    //with raw
    bool operator==(T* p_raw) const { return p_raw == raw_ptr_; }
    bool operator!=(T* p_raw) const { return p_raw != raw_ptr_; }

    bool operator==(std::nullptr_t p_null) const { return p_null == raw_ptr_; }
    bool operator!=(std::nullptr_t p_null) const { return p_null != raw_ptr_; }

    bool operator==(const shared_ptr& p_other) const { return raw_ptr_ == p_other.raw_ptr_; }
    bool operator!=(const shared_ptr& p_other) const { return raw_ptr_ != p_other.raw_ptr_; }
        
    explicit operator bool() const { return raw_ptr_ != nullptr; }


private:
    struct CB {
        std::atomic<size_t> ref_count_ {0};
    };

    T* raw_ptr_ {nullptr};
    CB* cb_ {nullptr};

    void inc_ref_count() { cb_->ref_count_++; }
    void dec_ref_count() {
        if(raw_ptr_ == nullptr)
            return;

        if(--(cb_->ref_count_) == 0) {
            delete raw_ptr_;
            raw_ptr_ = nullptr;
            delete cb_;
            cb_ = nullptr;
        }
    }
};
} //sp