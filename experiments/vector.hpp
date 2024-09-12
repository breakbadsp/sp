#include <utility>
#include <cstdlib>
#include <iostream>
#include <cassert>


namespace sp {
template<typename T>
class vector {
public:
    vector() = default;
    ~vector() { clear(); }

    vector(size_t p_capacity) 
    : capacity_(p_capacity)
    , size_(0) {
        buffer_ = reinterpret_cast<unsigned char*>(malloc(sizeof(T) * capacity_));
    }

    vector(const vector& p_rhs) 
    : capacity_(p_rhs.capacity_)
    , size_(p_rhs.size_) {
        buffer_ = reinterpret_cast<unsigned char*>(malloc(sizeof(T) * capacity_));
        for(size_t i = 0; i < size_; ++i) {
            new (slot(i)) T(*p_rhs.slot(i));
        }
    }

    vector(vector&& p_rhs)
    : buffer_(p_rhs.buffer_)
    , capacity_(p_rhs.capacity_)
    , size_(p_rhs.size_) {
        p_rhs.buffer_ = nullptr;
        p_rhs.capacity_ = 0;
        p_rhs.size_ = 0;
    }

    vector& operator=(const vector& p_rhs) {
        if(this != &p_rhs) {
            auto* new_buffer = reinterpret_cast<T*>(malloc(sizeof(T) * p_rhs.capacity_));
            for(size_t i = 0; i < p_rhs.size_; ++i) {
                new (&new_buffer[i]) T(*p_rhs.slot(i));
            }
            clear();
            capacity_ = p_rhs.capacity_;
            size_ = p_rhs.size_;
            buffer_ = reinterpret_cast<unsigned char*>(new_buffer);
        }
        return *this;
    }

    vector& operator=(vector&& p_rhs) {
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

    T pop_back()
    {
        assert(size_ > 0);
        auto ele = std::move(*slot(size_-1));
        slot(size_-1)->~T();
        --size_;
        return ele;
    }

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }

private:
    alignas(T) unsigned char* buffer_ {nullptr};
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
        buffer_ = reinterpret_cast<unsigned char*>(new_buff);
    }

    T* slot(size_t idx ) {
        return reinterpret_cast<T*>(buffer_ + (sizeof(T) * idx));
    }

    const T* slot(size_t idx ) const {
        return reinterpret_cast<T*>(buffer_ + (sizeof(T) * idx));
    }


    void clear() {
        for(size_t i = 0; i < size_; ++i) {
            slot(i)->~T();
        }
        free(buffer_);
        buffer_ = nullptr;
    }

};
}

void print(sp::vector<int>& p_elems) {
    for(size_t i = 0; i < p_elems.size(); ++i)
        std::cout << p_elems[i] << ' ';
    
    std::cout << ": Printed vector \n";
}

struct Temp {
    Temp() = default;
    ~Temp() { delete ptr; }

    Temp(const Temp& p_rhs) { 
        if(ptr)
            ptr = new int(*p_rhs.ptr);
    }

    Temp(Temp&& p_rhs)
    : ptr(p_rhs.ptr) {
        p_rhs.ptr = nullptr;
    }

    Temp& operator=(const Temp& p_rhs)
    {
        if(this != &p_rhs) {
            delete ptr;
            ptr = new int(*p_rhs.ptr);
        }
        return *this;
    }

    Temp& operator=(Temp&& p_rhs) {
        if(this != &p_rhs) {
            ptr = p_rhs.ptr;
            p_rhs.ptr = nullptr;
        }
        return *this;
    }

    Temp(int p)
    {
        ptr = new int(p);
    } 


    int* ptr = nullptr;

    friend std::ostream& operator<<(std::ostream& p_os, const Temp& p_rhs) {
        if(p_rhs.ptr)
            p_os << *p_rhs.ptr;
        return p_os;
    }
};

void print(sp::vector<Temp>& p_elems) {
    for(size_t i = 0; i < p_elems.size(); ++i)
        std::cout << p_elems[i] << ' ';
    
    std::cout << ": Printed vector \n";
}


void Test1()
{
    sp::vector<int> temp1;
    print(temp1);

    sp::vector<int> temp1_5(5);
    temp1_5.push_back(1);
    temp1_5.push_back(2);
    temp1_5.push_back(3);
    temp1_5.push_back(4);
    temp1_5.push_back(5);
    temp1_5.push_back(6);
    temp1_5.push_back(7);
    temp1_5.push_back(8);
    temp1_5.push_back(9);
    print(temp1_5);
    auto ele = temp1_5.pop_back();
    print(temp1_5);
    std::cout << ele << '\n';
    

    sp::vector<int> temp2(temp1_5);
    print(temp2);
    sp::vector<int> temp4(std::move(temp1_5)); 
    print(temp4);

    sp::vector<int> temp3;
    temp3 = temp2;
    print(temp3);
    sp::vector<int> temp5;
    temp5 = std::move(temp2);    
    print(temp5);
}
void Test2()
{
    sp::vector<Temp> temp1;
    print(temp1);

    sp::vector<Temp> temp1_5(5);
    temp1_5.push_back(1);
    temp1_5.push_back(2);
    temp1_5.push_back(3);
    temp1_5.push_back(4);
    temp1_5.push_back(5);
    temp1_5.push_back(6);
    temp1_5.push_back(7);
    temp1_5.push_back(8);
    temp1_5.push_back(9);

    Temp t1, t2, t3;
    temp1_5.push_back(t1);
    temp1_5.push_back(t2);
    temp1_5.push_back(t3);


    print(temp1_5);
    auto ele = temp1_5.pop_back();
    print(temp1_5);
    std::cout << ele << '\n';
    

    sp::vector<Temp> temp2(temp1_5);
    print(temp2);
    sp::vector<Temp> temp4(std::move(temp1_5)); 
    print(temp4);

    sp::vector<Temp> temp3;
    temp3 = temp2;
    print(temp3);
    sp::vector<Temp> temp5;
    temp5 = std::move(temp2);    
    print(temp5);
}

void Test3()
{
    sp::vector<Temp> temp1;
    print(temp1);

    sp::vector<Temp> temp1_5(5);
    temp1_5.push_back(1);
    temp1_5.push_back(2);
    temp1_5.push_back(3);
    temp1_5.push_back(4);
    temp1_5.push_back(5);
    temp1_5.push_back(6);
    temp1_5.push_back(7);
    temp1_5.push_back(8);
    temp1_5.push_back(9);
    
    Temp t1, t2, t3;
    temp1_5.push_back(t1);
    temp1_5.push_back(t2);
    temp1_5.push_back(t3);
    
    temp1_5.pop_back();
    temp1_5.pop_back();
    temp1_5.pop_back();
    temp1_5.pop_back();
    temp1_5.pop_back();
    temp1_5.push_back(5);
    temp1_5.push_back(6);
    temp1_5.push_back(7);
    temp1_5.push_back(8);
    temp1_5.push_back(9);
    temp1_5.push_back(10);
    temp1_5.push_back(11);
    temp1_5.push_back(12);
    
    print(temp1_5);
    auto ele = temp1_5.pop_back();
    print(temp1_5);
    std::cout << ele << '\n';
    

    sp::vector<Temp> temp2(temp1_5);
    print(temp2);
    sp::vector<Temp> temp4(std::move(temp1_5)); 
    print(temp4);

    sp::vector<Temp> temp3;
    temp3 = temp2;
    print(temp3);
    sp::vector<Temp> temp5;
    temp5 = std::move(temp2);    
    print(temp5);
}

int main()
{
   Test1();
   Test2();
   Test3();
}
