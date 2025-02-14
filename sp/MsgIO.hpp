#pragma once

#include "Socket.hpp"

namespace sp 
{
class TcpMsgIO
{
  public:
    TcpMsgIO() = default;

    TcpMsgIO(int p_sock_fd)
    : socket_(std::make_unique<TcpIpSocket>(p_sock_fd))
    { }

    TcpMsgIO(TcpIpSocket* p_socket)
    {
      socket_.reset(p_socket);
    }

    ~TcpMsgIO()
    {
      delete read_buff_;
      delete write_buff_;
    }

    TcpIpSocket& get_socket()
    {
      return *socket_;
    }

    const TcpIpSocket& get_socket() const
    {
      return *socket_;
    }

    int GetPort() const
    {
      return socket_->GetPort();
    }

    std::string GetIpAddr() const
    {
      return socket_->GetIpAddr();
    }

    int Write(std::string_view p_msg)
    {
      uint32_t len = htonl(p_msg.size());
      auto wrote = socket_->write(&len, sizeof(len));
      if(wrote <= 0)
      {
        return wrote;
      }

      if(write_buff_len_ < p_msg.size())
      {
        delete[] write_buff_;
        write_buff_len_ = p_msg.size();
        write_buff_ = new char[write_buff_len_];
      }

      strncpy(write_buff_, p_msg.data(), p_msg.size());
      wrote = socket_->write(write_buff_, p_msg.size());
      if (wrote <= 0)
      {
        std::cerr << "Write failed, error : " << errno << std::endl;
        return len;
      }
      std::cout << "Wrote " << p_msg.size() << " bytes on socket.\n";
      return wrote;
    }

    std::optional<std::string> Read()
    {
      uint32_t len = 0;
      auto ret = socket_->read(&len, sizeof(len));
      if(ret <= 0)
      {
        return {};
      }

      len = ntohl(len);
      if(read_buff_len_ < len)
      {
        delete[] read_buff_;
        read_buff_len_ = len;
        read_buff_ = new char[read_buff_len_];
      }

      ret = socket_->read(read_buff_, len);
      if(ret <= 0)
      {
        return {};
      }
      std::cout << "Read " << ret << " bytes from socket\n";
      read_buff_[len] = '\0';
      return std::string(read_buff_); //serialize
    }

    int Accept(struct sockaddr *p_addr, socklen_t *p_addrlen)
    {
      return socket_->Accept(p_addr, p_addrlen);
    }

  private:
    std::unique_ptr<TcpIpSocket> socket_ {nullptr};
    char* read_buff_  {nullptr};
    size_t read_buff_len_ {0};
    char* write_buff_ {nullptr};
    size_t write_buff_len_ {0};
}; 
}
