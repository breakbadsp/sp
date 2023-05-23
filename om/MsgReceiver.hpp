#include <unistd.h>
#include <cstring>
#include <iostream>

namespace om {
class MsgReceiver
{
    public:
        ~MsgReceiver() {
            close(socket_fd_);
            std::cout << "Connection closed from client\n";
        }

        MsgReceiver(int p_socket_fd) 
        : socket_fd_(p_socket_fd) {
        }

        void Run() {
            std::cout << "New MsgReceiver created\n";
            while(true) {
                //TODO:: Create protocol which should have header. For now read data in 1024 bytes buffer
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));
                const auto ret = read(socket_fd_, buffer, 1024);
                if(ret <= 0) {
                    HandleError();
                    break;
                }
                std::cout << "Client sent data:" << buffer << "\n";
                //TODO:: Add authentication support for client.
            }
            delete this; //TODO:: Change this to proper memory management
        }

    private:
        int socket_fd_ {0};

        void HandleError() {
            //TODO:: Receive errno and print possible error
        }
};
}//om