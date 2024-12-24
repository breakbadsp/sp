#pragma once

#include <stdlib.h>
#include <cstring>
#include <utility>
#include <iostream>

namespace sp{
class string {
public:
    string() = default;
    ~string() {
        delete [] buffer_;
    }

    string(const char* p_cstr) {
        init(p_cstr);
    }

    string(const string& p_rhs) {
        buffer_ = nullptr;
        size_ = 0;
        if(p_rhs.size_ != 0) {
            buffer_ = new char[p_rhs.size_+1];
            size_ = p_rhs.size_;
            strcpy(buffer_, p_rhs.buffer_);
        }
    }

    string(string&& p_rhs)
    : buffer_(p_rhs.buffer_)
    , size_(p_rhs.size_) {
        p_rhs.buffer_ = nullptr;
        p_rhs.size_ = 0;
    }

    string& operator=(const string& p_rhs) {
        buffer_ = nullptr;
        size_ = 0;
        if(&p_rhs != this && p_rhs.size_ > 0) {
            delete [] buffer_;
            buffer_ = new char[p_rhs.size_+1];
            size_ = p_rhs.size_;
            strcpy(buffer_, p_rhs.buffer_);
        }
        return * this;
    }

    string& operator=(string&& p_rhs) {
        if(&p_rhs != this) {
            delete [] buffer_;

            buffer_ = p_rhs.buffer_;
            size_= p_rhs.size_;

            p_rhs.buffer_ = nullptr;
            p_rhs.size_ = 0;
        }

        return *this;
    }

    const char* c_str() const { return buffer_; }
    bool empty() const { return size_ == 0; }


    friend std::ostream& operator<<(std::ostream& p_os, const sp::string& p_rhs) {
        p_os << p_rhs.c_str();
        return p_os;
    }


private:
    char* buffer_ {nullptr};
    size_t size_ {0};

    void init(const char* p_cstr) {
        size_ = std::strlen(p_cstr);
        buffer_ = new char[size_+1];
        strcpy(buffer_, p_cstr);
    }
};
}

/*std::ostream& operator<<(std::ostream& p_os, const sp::string& p_rhs) {
    p_os << p_rhs.c_str();
    return p_os;
}*/ 

void func3()
{
        std::cout << "temp string \n";
        sp::string test("temp string ");
        sp::string test2(std::move(test));
        sp::string test3(test2);
        std::cout << "out: " << test3 << '\n';

        sp::string test4;
        sp::string test5;
        test4 = test3;
        test5 = std::move(test4);
        std::cout << test5 << '\n';
}

void func2()
    {
        std::cout << "Empty string2:" << '\n';
        sp::string test("");
        sp::string test2(std::move(test));
        sp::string test3(test2);
        sp::string test4;
        sp::string test5;
        test4 = test3;
        test5 = std::move(test4);
        std::cout << test5 << '\n';
    }

void func1()
    {
        std::cout << "Empty string1:" << '\n';
        sp::string test("");
        sp::string test2(std::move(test));
        
        sp::string test3(test2);
        sp::string test4;
        sp::string test5;
        test4 = test3;
        test5 = std::move(test4);
        std::cout << "Empty string1 end move." << '\n';
        std::cout << test << '\n';
        std::cout << "Empty string1 end." << '\n';
    }


int main() {
    std::cout << "test starts\n";
    func1();
    func2();
    func3();
}