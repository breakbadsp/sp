#pragma once

#include <cassert>
#include "Types.hpp"

class MyHashSet {
public:
    static constexpr double load_fact = 0.5;
    static constexpr bool debug = false;
    constexpr MyHashSet() = default;
    
    bool add(int key) {
        if(size_ >= (load_fact * capacity_)) {
            rehash(capacity_ * 2);
        }

        int insert_hash = -1;
        if(find(key, insert_hash)) {
            return false;
        }
        Node* node = (store_ + insert_hash);
        node->ele = key;
        node->state = Node::State::Allocated;
        size_++;
        return true;
    }
    
    void remove(int key) {
        int index = -1;
        if(!find(key, index)) {
            return;
        }
        Node* node = (store_ + index);
        node->state = Node::State::Deallocated;
        size_--;
        return; 
    }
    
    bool contains(int key) {
        int index = -1;
        return find(key, index);
    }

    bool find(int key, int& index) {
        if(capacity_ == 0)
            return false;
        
        int hash = (key % capacity_);
        index = hash;
        
        if(hash >= capacity_)
            return false;

        Node* ele = (store_ + hash);
        Node* orign = ele;
        while(ele->state != Node::State::UnAllocated) {
            if(ele->ele == key && ele->state == Node::State::Allocated) {
                index = hash;
                assert(index != -1);
                assert(index < capacity_);
                return true;
            }
            hash = (hash + 1) % capacity_; 
            ele = store_ + hash;
            index = hash;
            if(ele == orign)
                break;
        }
        
        assert(index != -1);
        assert(index < capacity_);

        return false;
    }

private:
    struct Node {
        enum State {
            UnAllocated,
            Allocated,
            Deallocated,
        };
    
        State state = State::UnAllocated;
        int ele = 0;

        Node() = default;
        Node(int el) : ele(el), state(State::Allocated) {}
    };

    Node* store_ = nullptr;
    sp::size_t size_ = 0;
    sp::size_t capacity_ = 0;

    constexpr void rehash(sp::size_t new_cap) {
        if(new_cap == 0)
            new_cap = 2;
        Node* new_store = (Node*) new Node[new_cap];
        if(!store_) {
            store_ = new_store;
            capacity_ = new_cap;
            size_ = 0;
            //if(debug) std::cout << "Create hashtable of to size " << new_cap << '\n';
            return;
        }
        
        for(sp::size_t i = 0; i < capacity_; ++i) {
            Node* old_ele = (store_ + i);
            if(old_ele->state == Node::State::Allocated) {
                int new_hash = old_ele->ele % new_cap;
                Node* new_node = (new_store + new_hash);
                while(new_node->state != Node::State::UnAllocated) {
                    new_hash = (new_hash + 1) % new_cap;
                    new_node = new_store + new_hash;
                }
                new_node->ele = old_ele->ele;
                new_node->state = Node::State::Allocated;
                continue;
            }
        }
        
        delete  [] store_;
        capacity_ = new_cap;
        store_ = new_store;
    }

public:
    struct Iterator {
        Iterator() = delete;
        Iterator(const MyHashSet& set, bool end = false) : set_(set) {
            if(end)
                return;

            indx_ = 0;
            Node* node = set_.store_;
            while(node->state != Node::State::Allocated) {
                node++;
                indx_ = (indx_+1) % set_.capacity_;
                if(indx_ == 0)
                {
                    indx_ = -1;
                    break;
                }
            }
        }

        bool operator!=(const Iterator& p_rhs) const {
            return indx_ != p_rhs.indx_;
        }

        int operator*() {
            Node* node = set_.store_ + indx_;
            return node->ele;
        }

        Iterator& operator++() {
            auto orig = indx_;
            indx_ = (indx_ + 1) % set_.capacity_;            
            Node* node = set_.store_ + indx_;

            while((indx_ != orig) && (node->state != Node::State::Allocated) ) {
                indx_ = (indx_ + 1) % set_.capacity_;
                node = set_.store_ + indx_;            
            }

            if(indx_ == orig)
                indx_ = -1;
            
            return *this;
        }

        /*Iterator& operator--() {
            auto orig = indx_;
            indx_ = (indx_ - 1) % capacity_;            
            Node* node = set.store_ + indx_;

            while((indx_ != orig) && (node->state != Node::State::Allocated) ) {
                indx_ = (indx_ + 1) % capacity_;
                node = set.store_ + indx_;            
            }
            return *this;
        }*/

        int indx_ = -1;
        const MyHashSet& set_;
    };

public:
    Iterator begin() {
        return Iterator(*this);
    }

    Iterator end() {
        return Iterator(*this, true);
    }
};
