namespace sp {
class list {
public:
    ///TODO::Delete all the commented code once this container is matured
    int get(int index) {
        if(head_ == nullptr) 
            return -1;
    
        if(index == 0) 
            return head_->value_;

        auto* itr = head_.get();
        int i = 0;
        for(;i < index && itr->next_ != nullptr; ++i){
            itr = itr->next_.get();
        }
        
        if(itr == nullptr || i < index)
            return -1;
        
        return itr->value_;
    }
    
    void push_front(int val) {
        if(head_ == nullptr){
            //std::cout << "After adding " << val <<" at head, NEW \n";
            head_ = std::make_unique<Node>(val);
            //Print();
            return;
        }
        
        head_ = std::make_unique<Node>(val, std::move(head_));
        //std::cout << "After adding " << val <<" at Head \n";
        //Print();
    }
    
    void push_back(int val) {
        if(head_ == nullptr) 
            return push_front(val);
        
        auto* itr = head_.get();
        while(itr->next_ != nullptr){
            itr = itr->next_.get();
        }
        itr->next_ = std::make_unique<Node>(val);
        
        ///std::cout << "After adding " << val <<" at Tail \n";
        //Print();
    }
    
    void insert_at(int index, int val) {
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
        int i = 1;
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
            itr->next_ = std::make_unique<Node>(val);
            //std::cout << "After adding " << val << " at " << index << "\n";
            return;
        }
        
        itr->next_ = std::make_unique<Node>(val, std::move(itr->next_));
        //std::cout << "After adding " << val << " at " << index << "\n";
        //Print();
    }
    
    void delete_at(int index) {
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
        int i = 1;
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
        Node(int val) : value_(val) {}
        Node(int val, std::unique_ptr<Node> next) :
            value_(val),
            next_(std::move(next))
        {}
        
        int value_;
        std::unique_ptr<Node> next_; 
    };
    
    std::unique_ptr<Node> head_;
};
} //namespace sp

/**
 * Your MyLinkedList object will be instantiated and called as such:
 * MyLinkedList* obj = new MyLinkedList();
 * int param_1 = obj->get(index);
 * obj->push_front(val);
 * obj->push_back(val);
 * obj->insert_at(index,val);
 * obj->delete_at(index);
 */