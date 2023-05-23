#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <thread>
#include <iostream>

#include "MsgReceiver.hpp"

namespace om{

class Server
{
    public:
        Server(const uint16_t p_port) : port_(p_port) {}

        bool Run()
        {
            std::cout << "Starting server on port " << port_ << '\n';
            const auto fd = socket(AF_INET, SOCK_STREAM, 0);
            if(fd == -1)
                return false;
            
            struct sockaddr_in my_addr, peer_addr;
            memset(&my_addr, 0, sizeof(my_addr));
            memset(&peer_addr, 0, sizeof(peer_addr));

            my_addr.sin_family = AF_INET;
            my_addr.sin_port = htons(port_);
            my_addr.sin_addr.s_addr = INADDR_ANY;

            if( bind(fd, (sockaddr*)(&my_addr), sizeof(my_addr)) )
                return false;
            
            if( listen(fd, 50) )
                return false;
            
            while(true)
            {
                std::cout << "Waiting for connections on port " << port_ << '\n';
                socklen_t peer_addr_size;
                const auto cfd = accept(fd, (sockaddr*)(&peer_addr), &peer_addr_size);
                if(cfd == -1)
                    return false;
                
                std::cout << "New connection request recieved from ip:" << peer_addr.sin_addr.s_addr << " on port:" << port_ << '\n';
                MsgReceiver* receiver = new MsgReceiver(cfd);
                std::thread receiver_thread(&MsgReceiver::Run, receiver);
                //FIXME:: Change this detach to join, change receive into smart pointer
                receiver_thread.detach();
            }
        }

    private:
        uint16_t port_ {0};
};

}//om