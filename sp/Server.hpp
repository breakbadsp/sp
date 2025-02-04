#pragma once
// This will provide an abstraction for creating a TCP/IP or UDP/IP server
// APIs that I am planning to impliment are 
// 1. CreateTCPIPServer(port)

#include "Socket.hpp"
#include <iostream>
#include <netinet/in.h> //needed for sockaddr_in
#include <arpa/inet.h> //needed for inet_ntoa


namespace sp
{

class TcpIpServer
{
  public:
    TcpIpServer(int p_port)
    : socket_()
    , port_(p_port)
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port_);
      addr.sin_addr.s_addr = INADDR_ANY;

      if(socket_.Bind((struct sockaddr *)&addr, sizeof(addr)) < 0)
      {
        std::cerr << "Bind failed" << std::endl;
        exit(1);
      }

      if(socket_.Listen(5) < 0)
      {
        std::cerr << "Listen failed" << std::endl;
        exit(1);
      }
      std::cout << "Server listening on port " << port_ << std::endl;
    }

    TcpIpSocket Accept(struct sockaddr *p_addr, socklen_t *p_addrlen)
    {
      std::cout << "Server waiting for connection on port " << port_ << std::endl;
      TcpIpSocket client_socket = socket_.Accept(p_addr, p_addrlen);
      std::cout << "New connection from " << inet_ntoa(((struct sockaddr_in *)p_addr)->sin_addr) << std::endl;
      return client_socket;
    }

  private:
    TcpIpSocket socket_;
    int port_;
};

class UdpIpServer
{
  public:
    UdpIpServer(int p_port)
    : socket_()
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(p_port);
      addr.sin_addr.s_addr = INADDR_ANY;

      if(socket_.Bind((struct sockaddr *)&addr, sizeof(addr)) < 0)
      {
        std::cerr << "Bind failed" << std::endl;
        exit(1);
      }
    }

    int RecvFrom(void *p_buf, size_t p_len, int p_flags, struct sockaddr *p_addr, socklen_t *p_addrlen)
    {
      return socket_.RecvFrom(p_buf, p_len, p_flags, p_addr, p_addrlen);
    }

    int SendTo(const void *p_buf, size_t p_len, int p_flags, const struct sockaddr *p_addr, socklen_t p_addrlen)
    {
      return socket_.SendTo(p_buf, p_len, p_flags, p_addr, p_addrlen);
    }

  private:
    UdpIpSocket socket_;
};

} // namespace sp
