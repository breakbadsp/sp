#ifndef OBJECTPOOL_HPP
#define OBJECTPOOL_HPP

#include <memory>
#include <array>
#include <optional>
#include <iostream>
#include <bitset>

#undef USE_BITSET
#undef USE_PACKED_POOL

template<typename T, size_t PoolSize>
class ObjectPool {

public:
    // Allocate + construct
    template<typename... Args>
    T* emplace(Args&&... args) {

        if(!IsUsed(nfi_)) {
            // Modern construction
            T* ptr = std::construct_at(
                reinterpret_cast<T*>(&storage_[nfi_]),
                std::forward<Args>(args)...
            );
            SetUsed(nfi_, true);
            return ptr;
        }

        // Find free slot
        for (size_t i = 0; i < PoolSize; ++i) {
            if (!IsUsed(i)) {
                SetUsed(i, true);
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
                SetUsed(i, false);
                return;
            }
        }
    }
    
    ~ObjectPool() {
        // Destroy any remaining objects
        for (size_t i = 0; i < PoolSize; ++i) {
            if (IsUsed(i)) {
                std::destroy_at(reinterpret_cast<T*>(&storage_[i]));
            }
        }
    }

private:
        // Use std::byte for type-safe raw storage
    #ifdef USE_PACKED_POOL
        alignas(T) std::array<std::pair<std::byte[sizeof(T)], bool>, PoolSize> storage_ {};
    #else
        alignas(T) std::array<std::byte[sizeof(T)], PoolSize> storage_ {};
    #endif

    #ifdef USE_BITSET
        std::bitset<PoolSize> in_use_{};
    #else
        std::array<bool, PoolSize> in_use_{};
    #endif
        size_t nfi_ {0};

    constexpr inline bool IsUsed(size_t index) const 
    {
    #ifdef USE_PACKED_POOL
        return storage_[index].second;
    #else
        return in_use_[index]; 
    #endif
    }

    constexpr inline void SetUsed(size_t index, bool used) 
    {
    #ifdef USE_PACKED_POOL
        storage_[index].second = used;
    #else
        in_use_[index] = used;
    #endif
        nfi_ = used ? (index + 1) % PoolSize : index; // next free index hint
    }
};
#endif // OBJECTPOOL_HPP