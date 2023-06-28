#include "AsyncQueue.h"

namespace om {
class MsgProcessor {
    public:
        void Run() {
            
        }

    private:
        sp::AsyncQueue<void*> queue_;
};
}