#pragma once
#include "owning_ptr.hpp"

namespace sp {

template<typename T>
class list {
public:
    ///TODO::Delete all the commented code once this container is matured
    T get(size_t index) {
        if(head_ == nullptr) 
            return -1;
    
        if(index == 0) 
            return head_->value_; //FIXME::Use arrow operator on sp::owning_ptr

        auto* itr = head_.get();
        size_t i = 0;
        for(;i < index && itr->next_ != nullptr; ++i){
            itr = itr->next_.get();
        }
        
        if(itr == nullptr || i < index)
            return -1;
        
        return itr->value_;
    }
    
    void push_front(const T& val) {
        if(head_ == nullptr){
            //std::cout << "After adding " << val <<" at head, NEW \n";
            head_ = sp::make_owning<Node>(val);
            //Print();
            return;
        }
        
        head_ = sp::make_owning<Node>(val, std::move(head_));
        //std::cout << "After adding " << val <<" at Head \n";
        //Print();
    }
    
    void push_back(const T& val) {
        if(head_ == nullptr) 
            return push_front(val);
        
        auto* itr = head_.get();
        while(itr->next_ != nullptr){
            itr = itr->next_.get();
        }
        itr->next_ = sp::make_owning<Node>(val);
        
        ///std::cout << "After adding " << val <<" at Tail \n";
        //Print();
    }
    
    void insert_at(size_t index, const T& val) {
        //std::cout << "before adding " << val << " at " << index << "\n";
        //Print();
        if(index == 0) 
            return push_front(val);
        
        if(head_ == nullptr){
            //std::cout << "Empty list and add at index = " << index << "  > 0\n";
            return;
        }
            
        Node* itr = head_.get();
        //Node* prev = head_.get();
        size_t i = 1;
        for(; itr->next_ != nullptr &&  i < index; ++i){
            //prev = itr;
            itr = itr->next_.get();
        }
        
        if(i < index){
            //std::cout << "Add at tail called with wrong index=" << index << "\n";
            return;
        }
        
        if(i > index){
            //std::cout << "Cannot happen, Add at tail called with wrong index=" << index << "\n";
            return;
        }
        
        //Tail
        if(itr->next_ == nullptr){
            itr->next_ = sp::make_owning<Node>(val);
            //std::cout << "After adding " << val << " at " << index << "\n";
            return;
        }
        
        itr->next_ = sp::make_owning<Node>(val, std::move(itr->next_));
        //std::cout << "After adding " << val << " at " << index << "\n";
        //Print();
    }
    
    void delete_at(size_t index) {
        if(head_ == nullptr) 
            return;
        
        if(index == 0){
            if(head_->next_ == nullptr){
                //std::cout << "deleting only first element\n";
                head_.reset(nullptr);
                return;
            }
            //auto temp = std::move(head_->next_);
            //head_.reset(nullptr);
            //head_ = std::move(temp);
            head_ = std::move(head_->next_);
            return;
        }
        
        if(index == 1 && head_->next_ == nullptr){
            //std::cout << "Nothing to delete at " << index << "\n";
            return;
        }
        
        Node* itr = head_->next_.get();
        Node* prev = head_.get();
        size_t i = 1;
        for( ;i < index && itr->next_ != nullptr; ++i){
            prev = itr;
            itr = itr->next_.get();
        }
        
        if(i < index){
            //std::cout << "Wrong index to delete " << index << " > size " << i << "\n";
            return;
        }
        
        if(itr == nullptr){
            //std::cout << "Nothing to delete at index " << index << "\n";
            return;
        }
        
        //tail
        if(itr->next_ == nullptr){
            //std::cout << "Deleting last element\n";
            prev->next_.reset(nullptr);
            //Print();
            return;
        }
        
        //middle
        //auto temp = std::move(itr->next_);//copy itr next
        //prev->next_.reset(nullptr); // delete itr
        //prev->next_ = std::move(temp); //copy itr next;
        
        prev->next_ = std::move(itr->next_);
        //std::cout << "After deleting middle element at " <<  index <<"\n";
        //Print();
    }
    
    void Print() const{
        auto* itr = head_.get();
        while(itr != nullptr){
            std::cout << itr->value_ << ", ";
            itr = itr->next_.get();
        }
        std::cout << "\n";
    }

    
private:
    struct Node{
        Node() = default;

        Node(const T& val) : value_(val) {}
        Node(const T& val, sp::owning_ptr<Node>&& next) :
            value_(val),
            next_(std::move(next))
        {}
        
        T value_ {0};
        sp::owning_ptr<Node> next_ {nullptr};
    };
    
    sp::owning_ptr<Node> head_;
};
} //namespace sp
