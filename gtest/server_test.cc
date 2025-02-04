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

TEST_F(TcpIpServerTest, SimpleChatServer)
{
  std::thread server_thread = std::thread([&](){

    sp::TcpIpServer server(tcp_server_port);

    struct sockaddr_in cli_addr;
    socklen_t cli_addrlen = sizeof(cli_addr);
    auto client_fd = server.Accept((struct sockaddr *)&cli_addr, &cli_addrlen);
    if(client_fd.GetFd() == -1)
    {
      std::cerr << "Server Accept failed, error : " << errno << std::endl;

      return;
    }

    uint32_t len = 0;
    auto ret = read(client_fd.GetFd(), &len, sizeof(len));
    if(ret <= 0)
    {
      std::cerr << "Server Read failed, error : " << errno << std::endl;
      
      //print error
      char errbuf[256];
      strerror_r(errno, errbuf, sizeof(errbuf));
      std::cerr << "Error: " << errbuf << std::endl;
      return;
    }
    len = ntohl(len);
    std::cout << "Server Received " << ret << " bytes of data len=" << len << std::endl;

    char* buff = (char*) malloc(len+1);
    ret = recv(client_fd.GetFd(), buff, len, 0);
    if(ret <= 0)
    {
      std::cerr << "Server Read failed, error : " << errno << std::endl;
      return;
    }
    buff[len] = '\0';
    std::cout << "Server Received: " << buff << "." << std::endl;
    
    const char* response = "Hello, client";
    const size_t response_len = strlen(response)+1;

    free(buff);
    buff = (char*) malloc(response_len+2);
    strncpy((char*)buff, response, response_len-1);
    buff[response_len] = '\0';

    std::cout << "Server Sending: " << buff << std::endl;
    len = htonl(response_len);
    auto wrote = write(client_fd.GetFd(), &len, sizeof(len));
    if(wrote <= 0)
    {
      std::cerr << "Server Write failed, error : " << errno << std::endl;
      return;
    }

    std::cout << "Server Sent Length: " << ntohl(len) << std::endl;
    len = write(client_fd.GetFd(), buff, response_len);
    if (len <= 0)
    {
      std::cerr << "Server Write failed, error : " << errno << std::endl;
      return;
    }

    std::cout << "Server Sent " << response << std::endl;
    len = read(client_fd.GetFd(), &len, sizeof(len));
    if(len <= 0)
    {
      std::cerr << "Server Read failed, error : " << errno << std::endl;
      return;
    }

    free(buff);
    std::cout << "Server end, receive termination" << std::endl;
  });

  std::thread client_thread = std::thread([&](){
    sp::TcpIpSocket client;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(tcp_server_port);
    auto ret = inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    ASSERT_EQ(ret, 1);

    EXPECT_EQ(client.Connect((struct sockaddr *)&addr, sizeof(addr)), 0);


    const char *msg = "Hello, server";
    long len = strlen(msg)+1;
    char* buff = (char*) malloc(len);
    auto len1 = htonl(len);
    std::cout << "Client Sending Length: " << len << std::endl;
    ret = write(client.GetFd(), &len1, sizeof(len1));
    if(ret == -1) {
      std::cerr << "Client Write failed, error : " << errno << std::endl;
      return;
    }
    
    std::cout << "Client Sending: " << msg << std::endl;
    ret = write(client.GetFd(), msg, strlen(msg));
    if(ret == -1) {
      std::cerr << "Client Write failed, error : " << errno << std::endl;
      return;
    }

    std::cout << "Client Waiting for response" << std::endl;
    ret = read(client.GetFd(), &len1, sizeof(len1));
    if(ret == -1) {
      std::cerr << "Client Read failed, error : " << errno << std::endl;
      return;
    }

    len1 = ntohl(len1);
    ASSERT_EQ(ret, sizeof(len1));
    std::cout << "Client Received Length: " << len1 << std::endl;

    free(buff);
    buff = (char*) malloc(len1+1);
    ret = recv(client.GetFd(), buff, len1, 0);
    buff[len1] = '\0';
    std::cout << "Client Received: " << buff << std::endl;
    ret = write(client.GetFd(), &len1, sizeof(len1));
    if(ret == -1) {
      std::cerr << "Client Write failed, error : " << errno << std::endl;
      return;
    }

    free(buff); 
    std::cout << "Client ending after sending termination" << std::endl;
  });

  server_thread.join();
  client_thread.join();
}