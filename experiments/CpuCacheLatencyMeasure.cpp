#include <new>
#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <pthread.h>

//TODO:: How to get the cache sizes programmatically in C++ in a portable way?


//  ✘ daksh@breakbadsp  ~/src/sprc/sp   master ±  lscpu -C
// NAME ONE-SIZE ALL-SIZE WAYS TYPE        LEVEL  SETS PHY-LINE COHERENCY-SIZE
// L1d       32K     192K    8 Data            1    64        1             64
// L1i       32K     192K    8 Instruction     1    64        1             64
// L2       256K     1.5M    4 Unified         2  1024        1             64
// L3         9M       9M   12 Unified         3 12288        1             64

[[maybe_unused]] constexpr std::size_t KB = 1024;
[[maybe_unused]] constexpr std::size_t MB = 1024 * KB;
[[maybe_unused]] constexpr auto L1_CACHE_SIZE = (32 * KB);     // 32KB per core
[[maybe_unused]] constexpr auto L2_CACHE_SIZE = (256 * KB);      // 256KB per core (1.5MB/6)
[[maybe_unused]] constexpr auto L3_CACHE_SIZE = (9 * MB);           // 9MB shared

#ifdef __cpp_lib_hardware_interference_size
    using std::hardware_constructive_interference_size;
    using std::hardware_destructive_interference_size;
#else
    // 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │ ...
    constexpr std::size_t hardware_constructive_interference_size = 64;
    constexpr std::size_t hardware_destructive_interference_size = 64;
#endif

// Force memory access with volatile
struct alignas(hardware_constructive_interference_size) CacheLineEntry {
    volatile std::uint32_t next;
    std::uint8_t padding[60]; // Ensure no false sharing
};

void MeasureCacheLatency(const size_t cache_size, const char* name)
{
    const std::size_t cache_line = hardware_destructive_interference_size;
    const size_t slots = cache_size / cache_line;
    
    std::cout << name << " working set: " << (cache_size / 1024) << "KB, slots(size/cacheline): " << slots << "\n";
    
    // Allocate aligned entries
    std::vector<CacheLineEntry> entries(slots);
    std::vector<uint32_t> order(slots);
    
    // Initialize random traversal order
    for (uint32_t i = 0; i < slots; ++i) {
        order[i] = i;
    }
    std::mt19937 rng(12345);
    std::shuffle(order.begin(), order.end(), rng);

    // Create pointer chase sequence
    for (size_t i = 0; i < slots; ++i) {
        entries[order[i]].next = order[(i + 1) % slots];
    }

    // Warmup pass
    volatile uint32_t curr = 0;
    for (size_t i = 0; i < slots; ++i) {
        curr = entries[curr].next;
    }

    // Measurement pass
    const size_t iterations = 10'000'000;
    curr = 0;
    
    auto t0 = std::chrono::steady_clock::now();
    
    for (size_t i = 0; i < iterations; ++i) {
        curr = entries[curr].next;
    }
    
    auto t1 = std::chrono::steady_clock::now();
    double ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
    
    std::cout << name << " average read latency: " << (ns / iterations) << " ns\n\n";
}

void PinThreadToCore(size_t core_id)
{
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
}

void BenchMarkL1CacheLatency()
{
    MeasureCacheLatency(L1_CACHE_SIZE, "L1 cache");
}

void BenchMarkL2CacheLatency()
{
    MeasureCacheLatency(L2_CACHE_SIZE, "L2 cache");
}

void BenchMarkL3CacheLatency()
{
    MeasureCacheLatency(L3_CACHE_SIZE, "L3 cache");
}

void BenchMarkRamLatency()
{
    constexpr size_t RAM_TEST_SIZE = 64 * MB; // 64MB to exceed caches
    MeasureCacheLatency(RAM_TEST_SIZE, "RAM");
}

int main() {
    // Disable frequency scaling if possible
    std::cout << "Note: For accurate results, run with:\n"
              << "sudo cpupower frequency-set -g performance\n\n";
              
    PinThreadToCore(0);
    
    // Warm up the CPU
    volatile int warmup = 0;
    for(int i = 0; i < 1000000; i++) warmup++;
    
    BenchMarkL1CacheLatency();
    BenchMarkL2CacheLatency();
    BenchMarkL3CacheLatency();
    BenchMarkRamLatency();
    return 0;
}