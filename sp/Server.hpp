#pragma once
// This will provide an abstraction for creating a TCP/IP or UDP/IP server
// APIs that I am planning to impliment are 
// 1. CreateTCPIPServer(port)

#include <iostream>
#include <netinet/in.h> //needed for sockaddr_in
#include <arpa/inet.h> //needed for inet_ntoa

#include "Socket.hpp"
#include "MsgIO.hpp"

namespace sp
{

class TcpIpServer
{
  public:
    TcpIpServer() = delete;

    TcpIpServer(int p_port, int p_backlog = 5)
    : port_(p_port)
    {
      auto socket = std::make_unique<TcpIpSocket>();
      msg_io_ = std::make_unique<TcpMsgIO>(std::move(socket));

      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(p_port);
      addr.sin_addr.s_addr = INADDR_ANY;

      if(msg_io_->get_socket().Bind(
              (struct sockaddr *)&addr, 
              sizeof(addr)) < 0
          )
      {
        std::cerr << "Bind failed, error: " << strerror(errno) 
          << std::endl;
        msg_io_.reset(nullptr);
        return;
      }

      if(msg_io_->get_socket().Listen(p_backlog) < 0)
      {
        std::cerr << "Listen failed" << std::endl;
        msg_io_.reset(nullptr);
        return;
      }

      std::cout << "Server listening on port " << p_port 
        << std::endl;
    }

    bool IsValid() const { return msg_io_.get() != nullptr; }

    std::unique_ptr<TcpIpSocket> Accept()
    {
      std::cout << "Server waiting for connections on port:" 
        << port_ << std::endl;

      struct sockaddr cli_addr;
      socklen_t cli_addrlen = sizeof(cli_addr);
      int cli_sock_fd = msg_io_->Accept(&cli_addr, &cli_addrlen);
      return std::make_unique<TcpIpSocket>(cli_sock_fd);
    }

  private:
    std::unique_ptr<TcpMsgIO> msg_io_;
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

      if(socket_.Bind((struct sockaddr *)&addr,
                      sizeof(addr)) < 0)
      {
        std::cerr << "Bind failed" << std::endl;
        exit(1);
      }
    }

    int RecvFrom(void *p_buf, size_t p_len, 
                 int p_flags, struct sockaddr *p_addr, 
                 socklen_t *p_addrlen)
    {
      return socket_.RecvFrom(p_buf, p_len, p_flags, p_addr, p_addrlen);
    }

    int SendTo(const void *p_buf, size_t p_len, int p_flags, 
          const struct sockaddr *p_addr, socklen_t p_addrlen)
    {
      return socket_.SendTo(p_buf, p_len, p_flags, p_addr, p_addrlen);
    }

  private:
    UdpIpSocket socket_;
};

} // namespace sp
