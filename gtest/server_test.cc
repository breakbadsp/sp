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
  std::mutex mut;
  std::condition_variable cv;
  bool server_ready = false;

  std::thread server_thread = std::thread([&]()
  {  
    sp::TcpIpServer server(tcp_server_port, 2);
    ASSERT_EQ(server.IsValid(), true);
    
    {
      std::unique_lock ul(mut);
      server_ready = true;
      cv.notify_one();
    }

    auto client_socket = server.Accept();
    if(!client_socket->IsValid())
    {
      std::cerr << "Server Accept failed, error : " << errno << std::endl;
      return;
    }
    sp::TcpMsgIO cli_msg_io(std::move(client_socket));
    std::cout << "New cli connection from " << cli_msg_io.GetIpAddr() << ":" 
      << cli_msg_io.GetPort() << std::endl;

    while(true)
    {
      const auto cli_msg = cli_msg_io.Read();
      if(!cli_msg)
      {
        std::cerr << "Read failed, error : " << errno << std::endl;
        //std::cerr << "client " << client_socket.GetIpAddr() << " disconnected: " << std::endl;
        break;
      }
      std::cout << "Server Received: " << *cli_msg << std::endl;

      const std::string response = "Server Echo: " + *cli_msg;
      auto ret = cli_msg_io.Write(response);
      if(ret < 0)
      {
        std::cerr << "Server Write failed, error : " << errno << std::endl;
        return;
      }
    }
  });

  std::thread client_thread = std::thread([&](){

    {
      std::unique_lock ul(mut);
      while(!server_ready)
        cv.wait(ul);
    }

    auto client_socket = std::make_unique<sp::TcpIpSocket>();

    if(client_socket->Connect("127.0.0.1", tcp_server_port) == false)
      return;

    sp::TcpMsgIO io(std::move(client_socket));

    std::string msg; 
    for(size_t i = 0; i < 10; i++)
    {
      msg = "Hello, server ";
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
