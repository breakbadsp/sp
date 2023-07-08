#pragma once
#include "shared_ptr.hpp"

namespace sp {

template<typename T>
class doubly_list {
public:
    ///TODO::Delete all the commented code once this container is matured
    T get(size_t index) {
        if(head_ == nullptr) 
            return {};
    
        if(index == 0) 
            return head_->value_; //FIXME::Use arrow operator on sp::shared_ptr

        auto* itr = head_.get();
        size_t i = 0;
        for(;i < index && itr->next_ != nullptr; ++i){
            itr = itr->next_.get();
        }
        
        if(itr == nullptr || i < index)
            return {};
        
        return itr->value_;
    }
    
    void push_front(const T& val) {
        if(head_ == nullptr){
            //std::cout << "After adding " << val <<" at head, NEW \n";
            head_ = new Node(val);
            tail_ = head_;
            //Print();
            return;
        }
        head_ = new Node(val, std::move(head_), nullptr);
        //std::cout << "After adding " << val <<" at Head \n";
        //Print();
    }
    
    void push_back(const T& val) {
        if(tail_ == nullptr) 
            return push_front(val);
        
        auto* new_node = new Node(val, nullptr, tail_);
        tail_->next_ = new_node;
        tail_ = tail_->next_;
        
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
            //FIXME:: assert(itr == tail)
            itr->next_ = new Node(val, nullptr, std::move(tail_));
            tail_ = itr->next_;
            //std::cout << "After adding " << val << " at " << index << "\n";
            return;
        }
        auto new_node_next = itr->next_;
        auto new_node_prev = new_node_next->prev_;

        itr->next_ = new Node(val, new_node_next, new_node_prev);
        new_node_next->prev_ = itr->next_;
        //std::cout << "After adding " << val << " at " << index << "\n";
        //Print();
    }
    
    void delete_at(size_t index) {
        if(head_ == nullptr) 
            return;
        
        if(index == 0){
            if(head_->next_ == nullptr){
                //std::cout << "deleting only first element\n";
                head_ = nullptr;
                tail_ = nullptr;
                return;
            }
            //auto temp = std::move(head_->next_);
            //head_.reset(nullptr);
            //head_ = std::move(temp);
            head_ = std::move(head_->next_);
            head_->prev_ = nullptr;
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
        
        //tail //FIXME:: assert(taill == itr)
        if(itr->next_ == nullptr){
            //std::cout << "Deleting last element\n";
            prev->next_ = nullptr;
            tail_ = prev;
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
            //std::cout << itr->value_ << ", ";
            itr = itr->next_.get();
        }
        std::cout << "\n";
    }

    
private:
    struct Node{
        Node() = default;

        Node(const T& val) : value_(val) {
        }

        Node(const T& val, sp::shared_ptr<Node> p_next, sp::shared_ptr<Node> p_prev) :
            value_(val)
            ,next_(p_next)
            ,prev_(p_prev)
        {}
        
        T value_ {0};
        sp::shared_ptr<Node> next_ {nullptr};
        sp::shared_ptr<Node> prev_ {nullptr};
    };
    
    sp::shared_ptr<Node> head_;
    sp::shared_ptr<Node> tail_;
    //FIXME:: Memory leak in all operations of this container
};
} //namespace sp


