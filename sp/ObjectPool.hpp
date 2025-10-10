#ifndef OBJECTPOOL_HPP
#define OBJECTPOOL_HPP

#include <memory>
#include <array>
#include <optional>
#include <iostream>
#include <bitset>

#define USE_BITSET 1
#undef USE_BITSET

template<typename T, size_t PoolSize>
class ObjectPool {
private:
    // Use std::byte for type-safe raw storage
    alignas(T) std::array<std::byte[sizeof(T)], PoolSize> storage_;
#ifdef USE_BITSET
    std::bitset<PoolSize> in_use_{};
#else
    std::array<bool, PoolSize> in_use_{};
#endif

public:
    // Allocate + construct
    template<typename... Args>
    T* emplace(Args&&... args) {
        // Find free slot
        for (size_t i = 0; i < PoolSize; ++i) {
            if (!in_use_[i]) {
                in_use_[i] = true;
                // Modern construction
                T* ptr = std::construct_at(
                    reinterpret_cast<T*>(&storage_[i]),
                    std::forward<Args>(args)...
                );
                
                return ptr;
            }
        }
        return nullptr;  // Pool exhausted
    }
    
    // Destroy + deallocate
    void destroy(T* ptr) {
        if (!ptr) return;
        
        // Find which slot
        for (size_t i = 0; i < PoolSize; ++i) {
            if (reinterpret_cast<T*>(&storage_[i]) == ptr) {
                std::destroy_at(ptr);  // Modern destruction
                in_use_[i] = false;
                return;
            }
        }
    }
    
    ~ObjectPool() {
        // Destroy any remaining objects
        for (size_t i = 0; i < PoolSize; ++i) {
            if (in_use_[i]) {
                std::destroy_at(reinterpret_cast<T*>(&storage_[i]));
            }
        }
    }
};
#endif // OBJECTPOOL_HPP