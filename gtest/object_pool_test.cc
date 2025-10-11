#include <gtest/gtest.h>
#include <chrono>
#include <vector>
#include <string>
#include "../sp/ObjectPool.hpp"

// Test class with non-trivial operations
class TestObject {
public:
    TestObject(int id = 0, std::string data = "default") 
      : data_(data), id_(id) {}
    
    // Make it do some work
    void doWork() {
        result_ = 0;
        for(int i = 0; i < 100; ++i) {
            result_ += id_ * i;
        }
        data_ += std::to_string(result_);
    }

private:
    std::string data_ {};
    int id_ {};
    int result_{0};
};

class ObjectPoolBenchmark : public ::testing::Test {
protected:
    static constexpr size_t NUM_OBJECTS = 1000000;  // 1 million objects
    static constexpr size_t POOL_SIZE = 1000;       // Pool size of 1000
    
    void SetUp() override {
        // Start with clean measurements
        pool_time_ = heap_time_ = 0;
    }

    // Helper to get current time in microseconds
    inline int64_t getCurrentMicros() {
        return std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
        ).count();
    }

    double pool_time_;
    double heap_time_;
};

TEST_F(ObjectPoolBenchmark, PoolVsHeapAllocation) {
    // Test with object pool
    {
        ObjectPool<TestObject, POOL_SIZE> pool;
        auto start = getCurrentMicros();
        
        // Create and use objects from pool
        for(size_t i = 0; i < NUM_OBJECTS; ++i) {
            auto* obj = pool.emplace(i, "test_" + std::to_string(i));
            ASSERT_NE(obj, nullptr);
            obj->doWork();
            pool.destroy(obj);
        }
        
        auto end = getCurrentMicros();
        pool_time_ = static_cast<double>(end - start) / 1000000.0; // Convert to seconds
    }

    // Test with heap allocation
    {
        auto start = getCurrentMicros();
        
        // Create and use objects from heap
        for(size_t i = 0; i < NUM_OBJECTS; ++i) {
            auto* obj = new TestObject(i, "test_" + std::to_string(i));
            ASSERT_NE(obj, nullptr);
            obj->doWork();
            delete obj;
        }
        
        auto end = getCurrentMicros();
        heap_time_ = static_cast<double>(end - start) / 1000000.0; // Convert to seconds
    }

    std::cout << "\nBenchmark Results (seconds):\n"
              << "Object Pool: " << pool_time_ << "\n"
              << "Heap Alloc:  " << heap_time_ << "\n"
              << "Speedup:     " << (heap_time_ / pool_time_) << "x\n\n";

    // Pool should be faster
    EXPECT_LT(pool_time_, heap_time_);
}

TEST_F(ObjectPoolBenchmark, PoolReuse) {
    ObjectPool<TestObject, POOL_SIZE> pool;
    std::vector<TestObject*> objects;
    objects.reserve(POOL_SIZE);

    auto start = getCurrentMicros();

    // Fill pool
    for(size_t i = 0; i < POOL_SIZE; ++i) {
        auto* obj = pool.emplace(i);
        ASSERT_NE(obj, nullptr);
        objects.push_back(obj);
    }

    // Use objects
    for(auto* obj : objects) {
        obj->doWork();
    }

    // Destroy half
    for(size_t i = 0; i < POOL_SIZE/2; ++i) {
        pool.destroy(objects[i]);
    }

    // Reallocate half
    for(size_t i = 0; i < POOL_SIZE/2; ++i) {
        auto* obj = pool.emplace(i + POOL_SIZE);
        ASSERT_NE(obj, nullptr);
        obj->doWork();
        pool.destroy(obj);
    }

    // Cleanup remaining
    for(size_t i = POOL_SIZE/2; i < POOL_SIZE; ++i) {
        pool.destroy(objects[i]);
    }

    auto end = getCurrentMicros();
    double reuse_time = static_cast<double>(end - start) / 1000000.0;

    std::cout << "Pool Reuse Test Time: " << reuse_time << " seconds\n";
}

TEST_F(ObjectPoolBenchmark, PoolExhaustion) {
    ObjectPool<TestObject, POOL_SIZE> pool;
    std::vector<TestObject*> objects;
    objects.reserve(POOL_SIZE + 1);

    // Fill pool to capacity
    for(size_t i = 0; i < POOL_SIZE; ++i) {
        auto* obj = pool.emplace(i);
        ASSERT_NE(obj, nullptr);
        objects.push_back(obj);
    }

    // Try to allocate one more (should fail)
    auto* overflow_obj = pool.emplace(POOL_SIZE);
    EXPECT_EQ(overflow_obj, nullptr);

    // Cleanup
    for(auto* obj : objects) {
        pool.destroy(obj);
    }
}

// Test concurrent access if needed (requires thread safety implementation)
/*
TEST_F(ObjectPoolBenchmark, ConcurrentAccess) {
    ObjectPool<TestObject, POOL_SIZE> pool;
    std::vector<std::thread> threads;
    
    auto worker = [&pool](int id) {
        for(int i = 0; i < 1000; ++i) {
            auto* obj = pool.emplace(i);
            if(obj) {
                obj->doWork();
                pool.destroy(obj);
            }
        }
    };

    // Launch multiple threads
    for(int i = 0; i < 4; ++i) {
        threads.emplace_back(worker, i);
    }

    // Join threads
    for(auto& t : threads) {
        t.join();
    }
}
*/

TEST(ObjectPoolTest, BasicAllocDeallocInt) {
    ObjectPool<int, 5> pool;

    // Allocate objects
    int* obj1 = pool.emplace(10);
    int* obj2 = pool.emplace(20);
    int* obj3 = pool.emplace(30);

    ASSERT_NE(obj1, nullptr);
    ASSERT_NE(obj2, nullptr);
    ASSERT_NE(obj3, nullptr);
    EXPECT_EQ(*obj1, 10);
    EXPECT_EQ(*obj2, 20);
    EXPECT_EQ(*obj3, 30);

    // Deallocate one object
    pool.destroy(obj2);

    // Allocate another object, should reuse the freed slot
    int* obj4 = pool.emplace(40);
    ASSERT_NE(obj4, nullptr);
    EXPECT_EQ(*obj4, 40);

    // Clean up remaining objects
    pool.destroy(obj1);
    pool.destroy(obj3);
    pool.destroy(obj4);
}

TEST(ObjectPoolTest, BasicAllocDeallocTestStruct) {
    struct TestStruct {
        int id;
        std::string name;

        TestStruct(int i, const std::string& n) : id(i), name(n) {}
    };
    ObjectPool<TestStruct, 5> pool;

    // Allocate objects
    TestStruct* obj1 = pool.emplace(1, "Object 1");
    TestStruct* obj2 = pool.emplace(2, "Object 2");
    TestStruct* obj3 = pool.emplace(3, "Object 3");

    ASSERT_NE(obj1, nullptr);
    ASSERT_NE(obj2, nullptr);
    ASSERT_NE(obj3, nullptr);
    EXPECT_EQ(obj1->id, 1);
    EXPECT_EQ(obj2->id, 2);
    EXPECT_EQ(obj3->id, 3);
    EXPECT_EQ(obj2->name, "Object 2");
    EXPECT_EQ(obj3->name, "Object 3");

    // Deallocate one object
    pool.destroy(obj2);

    // Allocate another object, should reuse the freed slot
    TestStruct* obj4 = pool.emplace(4, "Object 4");
    ASSERT_NE(obj4, nullptr);
    EXPECT_EQ(obj4->id, 4);
    EXPECT_EQ(obj4->name, "Object 4");

    // Clean up remaining objects
    pool.destroy(obj1);
    pool.destroy(obj3);
    pool.destroy(obj4);
}

//Exaust the pool size and check nullptr is returned
TEST(ObjectPoolTest, ExhaustPool) {
    ObjectPool<int, 3> pool;
    int* obj1 = pool.emplace(1);
    int* obj2 = pool.emplace(2);
    int* obj3 = pool.emplace(3);
    int* obj4 = pool.emplace(4); // Should be nullptr
    EXPECT_NE(obj1, nullptr); 
    EXPECT_NE(obj2, nullptr);
    EXPECT_NE(obj3, nullptr);
    EXPECT_EQ(obj4, nullptr); // Pool exhausted
    pool.destroy(obj1);
    pool.destroy(obj2);
    pool.destroy(obj3);

    //allocate again after destroying
    int* obj5 = pool.emplace(5);
    EXPECT_NE(obj5, nullptr);
    EXPECT_EQ(*obj5, 5);
    pool.destroy(obj5);
}