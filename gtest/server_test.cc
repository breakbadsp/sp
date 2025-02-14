#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <cstring>  //  std::strlen


#include <gtest/gtest.h>
#include "Server.hpp"


class TcpIpServerTest : public ::testing::Test
{
  protected:
    TcpIpServerTest()
    {
    }

    ~TcpIpServerTest() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

    int tcp_server_port = 12345;
};

TEST_F(TcpIpServerTest, SingleClientEchoBackServer)
{
  std::thread server_thread = std::thread([&](){

    sp::TcpIpServer server(tcp_server_port, 2);

    struct sockaddr_in cli_addr;
    socklen_t cli_addrlen = sizeof(cli_addr);
    auto client_socket = server.Accept((struct sockaddr *)&cli_addr, &cli_addrlen);

    if(!client_socket)
    {
      std::cerr << "Server Accept failed, error : " << errno << std::endl;
      return;
    }
    sp::TcpMsgIO client_msg_io(client_socket);
    //std::cout << "New connection from " << client_msg_io->GetIpAddr() << ":" 
      //<< client_socket>GetPort() << std::endl;

    while(true)
    {
      const auto cli_msg = client_msg_io.Read();
      if(!cli_msg)
      {
        std::cerr << "Read failed, error : " << errno << std::endl;
        //std::cerr << "client " << client_socket.GetIpAddr() << " disconnected: " << std::endl;
        break;
      }
      std::cout << "Server Received: " << *cli_msg << std::endl;

      const std::string response = "Server Echo: " + *cli_msg;
      auto ret = client_msg_io.Write(response);
      if(ret < 0)
      {
        std::cerr << "Server Write failed, error : " << errno << std::endl;
        return;
      }
    }
  });

  std::thread client_thread = std::thread([&](){
    sp::TcpIpSocket* client_socket = new sp::TcpIpSocket();

    if(!client_socket->Connect("127.0.0.1", tcp_server_port))
      return;

    sp::TcpMsgIO io(client_socket);

    std::string msg = "Hello, server";  
    for(size_t i = 0; i < 10; i++)
    {
      msg += ' ';
      msg += std::to_string(i);
      auto ret = io.Write(msg);
      if(ret < 0)
      {
        std::cerr << "Client Write failed, error : " << errno << std::endl;
        return;
      }
      const auto srv_msg = io.Read();
      if(srv_msg)
        std::cout << "Client Received: " << *srv_msg << std::endl;
    }

  });

  server_thread.join();
  client_thread.join();
}
