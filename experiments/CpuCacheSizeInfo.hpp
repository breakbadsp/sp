#ifndef CPU_CACHE_DETECTOR_H
#define CPU_CACHE_DETECTOR_H

#include <iostream>
#include <vector>
#include <string>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#include <fstream>
#include <sstream>
#elif __APPLE__
#include <sys/sysctl.h>
#endif

class CacheInfo {
public:
    size_t l1d_size = 0;  // L1 Data cache
    size_t l1i_size = 0;  // L1 Instruction cache
    size_t l2_size = 0;
    size_t l3_size = 0;
    size_t cache_line_size = 0;
    
    void print() const {
        std::cout << "CPU Cache Information:\n";
        std::cout << "=====================\n";
        if (l1d_size > 0)
        std::cout << "L1 Data Cache:        " << (l1d_size / 1024) << " KB\n";
        if (l1i_size > 0)
        std::cout << "L1 Instruction Cache: " << (l1i_size / 1024) << " KB\n";
        if (l2_size > 0)
        std::cout << "L2 Cache:             " << (l2_size / 1024) << " KB\n";
        if (l3_size > 0)
        std::cout << "L3 Cache:             " << (l3_size / 1024) << " KB\n";
        if (cache_line_size > 0)
        std::cout << "Cache Line Size:      " << cache_line_size << " bytes\n";
        std::cout << "=====================\n";
    }
};

class CacheDetector {
public:
    static CacheInfo getCacheInfo() {
        CacheInfo info;
        
#ifdef _WIN32
        getWindowsCacheInfo(info);
#elif __linux__
        getLinuxCacheInfo(info);
#elif __APPLE__
        getMacCacheInfo(info);
#else
        std::cerr << "Unsupported platform\n";
#endif
        
        return info;
    }
    
private:
#ifdef _WIN32
    static void getWindowsCacheInfo(CacheInfo& info) {
        DWORD bufferSize = 0;
        GetLogicalProcessorInformation(nullptr, &bufferSize);
        
        std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer(
            bufferSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
        
        if (GetLogicalProcessorInformation(buffer.data(), &bufferSize)) {
            for (const auto& item : buffer) {
                if (item.Relationship == RelationCache) {
                    const CACHE_DESCRIPTOR& cache = item.Cache;
                    
                    switch (cache.Level) {
                        case 1:
                            if (cache.Type == CacheData) {
                                info.l1d_size = cache.Size;
                            } else if (cache.Type == CacheInstruction) {
                                info.l1i_size = cache.Size;
                            }
                            info.cache_line_size = cache.LineSize;
                            break;
                        case 2:
                            info.l2_size = cache.Size;
                            break;
                        case 3:
                            info.l3_size = cache.Size;
                            break;
                    }
                }
            }
        }
    }
#endif

#ifdef __linux__
    static void getLinuxCacheInfo(CacheInfo& info) {
        // Try sysfs interface first (more reliable)
        info.l1d_size = readCacheSize("/sys/devices/system/cpu/cpu0/cache/index0/size");
        info.l1i_size = readCacheSize("/sys/devices/system/cpu/cpu0/cache/index1/size");
        info.l2_size = readCacheSize("/sys/devices/system/cpu/cpu0/cache/index2/size");
        info.l3_size = readCacheSize("/sys/devices/system/cpu/cpu0/cache/index3/size");
        
        // Get cache line size
        std::ifstream lineFile("/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size");
        if (lineFile.is_open()) {
            lineFile >> info.cache_line_size;
        }
        
        // Fallback to sysconf if sysfs doesn't work
        if (info.l1d_size == 0) {
            long l1 = sysconf(_SC_LEVEL1_DCACHE_SIZE);
            long l2 = sysconf(_SC_LEVEL2_CACHE_SIZE);
            long l3 = sysconf(_SC_LEVEL3_CACHE_SIZE);
            
            if (l1 > 0) info.l1d_size = l1;
            if (l2 > 0) info.l2_size = l2;
            if (l3 > 0) info.l3_size = l3;
            
            long linesize = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
            if (linesize > 0) info.cache_line_size = linesize;
        }
    }
    
    static size_t readCacheSize(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return 0;
        
        std::string sizeStr;
        file >> sizeStr;
        
        if (sizeStr.empty()) return 0;
        
        size_t multiplier = 1;
        char unit = sizeStr.back();
        
        if (unit == 'K') {
            multiplier = 1024;
            sizeStr.pop_back();
        } else if (unit == 'M') {
            multiplier = 1024 * 1024;
            sizeStr.pop_back();
        }
        
        try {
            return std::stoull(sizeStr) * multiplier;
        } catch (...) {
            return 0;
        }
    }
#endif

#ifdef __APPLE__
    static void getMacCacheInfo(CacheInfo& info) {
        size_t size = sizeof(size_t);
        
        sysctlbyname("hw.l1dcachesize", &info.l1d_size, &size, nullptr, 0);
        sysctlbyname("hw.l1icachesize", &info.l1i_size, &size, nullptr, 0);
        sysctlbyname("hw.l2cachesize", &info.l2_size, &size, nullptr, 0);
        sysctlbyname("hw.l3cachesize", &info.l3_size, &size, nullptr, 0);
        sysctlbyname("hw.cachelinesize", &info.cache_line_size, &size, nullptr, 0);
    }
#endif
};

#endif // CPU_CACHE_DETECTOR_H