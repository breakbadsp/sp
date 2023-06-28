#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <cstring>
#include <thread>

#include "OM.hpp"
#include "Server.hpp"

using namespace om;
void om::CreateTCPIPServer(const uint16_t p_port)
{
    std::cout << "creating server on port " << p_port << '\n';
    Server* server = new Server(p_port);
    std::thread server_thread(&Server::Run, server);
    server_thread.detach();
}
