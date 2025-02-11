 #pragma once

#include <sys/types.h> //needed only for some legacy BSD
#include <sys/socket.h>
#include <netinet/in.h> //needed for sockaddr_in
#include <netinet/tcp.h> //needed for TCP_NODELAY
#include <unistd.h> //needed for close
#include <fcntl.h> //needed for fcntl
#include <sys/time.h> //needed for timeval

#include <iostream>


namespace sp
{
class Socket
{
  protected:
    Socket(int p_domain, int p_type) 
    : fd_(socket(p_domain, p_type, 0))
    {
    }

    Socket(int p_fd) : fd_(p_fd)
    {
    }

  public:
    Socket() = delete;

    ~Socket()
    {
      if(fd_ != -1)
        close(fd_);
    }

    int Bind(const struct sockaddr *p_addr, socklen_t p_addrlen)
    {
      return ::bind(fd_, p_addr, p_addrlen);
    }

    int Listen(int p_backlog)
    {
      return ::listen(fd_, p_backlog);
    }

    int Accept(struct sockaddr *p_addr, socklen_t *p_addrlen)
    {
      return ::accept(fd_, p_addr, p_addrlen);
    }

    int Connect(const struct sockaddr *p_addr, socklen_t p_addrlen)
    {
      return ::connect(fd_, p_addr, p_addrlen);
    }

    int Send(const void *p_buf, size_t p_len, int p_flags)
    {
      return ::send(fd_, p_buf, p_len, p_flags);
    }

    int Recv(void *p_buf, size_t p_len, int p_flags)
    {
      return ::recv(fd_, p_buf, p_len, p_flags);
    }

    int read(void *p_buf, size_t p_len)
    {
      return ::read(fd_, p_buf, p_len);
    }

    int write(const void *p_buf, size_t p_len)
    {
      return ::write(fd_, p_buf, p_len);
    }

    int GetFd() const
    {
      return fd_;
    }

    int SetNonBlocking()
    {
      return fcntl(fd_, F_SETFL, O_NONBLOCK);
    }

    int SetReuseAddr()
    {
      int opt = 1;
      return setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    }

    int SetReusePort()
    {
      int opt = 1;
      return setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    }

    int SetKeepAlive()
    {
      int opt = 1;
      return setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
    }

    int SetLinger(int p_onoff, int p_linger)
    {
      struct linger opt = {p_onoff, p_linger};
      return setsockopt(fd_, SOL_SOCKET, SO_LINGER, &opt, sizeof(opt));
    }

    int SetRecvTimeout(int p_sec, int p_usec)
    {
      struct timeval opt = {p_sec, p_usec};
      return setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &opt, sizeof(opt));
    }

    int SetSendTimeout(int p_sec, int p_usec)
    {
      struct timeval opt = {p_sec, p_usec};
      return setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &opt, sizeof(opt));
    }

    int SetRecvBufSize(int p_size)
    {
      return setsockopt(fd_, SOL_SOCKET, SO_RCVBUF, &p_size, sizeof(p_size));
    }

    int SetSendBufSize(int p_size)
    {
      return setsockopt(fd_, SOL_SOCKET, SO_SNDBUF, &p_size, sizeof(p_size));
    }

    int SetSockOpt(int p_level, int p_optname, const void *p_optval, socklen_t p_optlen)
    {
      return setsockopt(fd_, p_level, p_optname, p_optval, p_optlen);
    }

  private:
    int fd_;
};


class TcpIpSocket : public Socket
{
  public:
    TcpIpSocket() : Socket(AF_INET, SOCK_STREAM)
    {
    }

    TcpIpSocket(int p_fd) : Socket(p_fd)
    {
    }

    int SetNoDelay()
    {
      int opt = 1;
      return SetSockOpt(IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
    }

    int SetQuickAck()
    {
      int opt = 1;
      return SetSockOpt(IPPROTO_TCP, TCP_QUICKACK, &opt, sizeof(opt));
    }

    // set low latency option of the socket
    int SetLowLatency()
    {
      auto ret = SetNoDelay();
      if(ret != 0)
        return ret;
      ret = SetQuickAck();
      return ret;
    }

    // set high throughput option of the socket
    int SetHighThroughput()
    {
      int opt = 1;
      return SetSockOpt(IPPROTO_TCP, TCP_CORK, &opt, sizeof(opt));
    }
};

class UdpIpSocket : public Socket
{
  public:
    UdpIpSocket() : Socket(AF_INET, SOCK_DGRAM)
    {
    }

    int RecvFrom(void *p_buf, size_t p_len, int p_flags, struct sockaddr *p_addr, socklen_t *p_addrlen)
    {
      return recvfrom(GetFd(), p_buf, p_len, p_flags, p_addr, p_addrlen);
    }

    int SendTo(const void *p_buf, size_t p_len, int p_flags, const struct sockaddr *p_addr, socklen_t p_addrlen)
    {
      return sendto(GetFd(), p_buf, p_len, p_flags, p_addr, p_addrlen);
    }

    int SetBroadcast()
    {
      int opt = 1;
      return SetSockOpt(SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
    }

    int SetMulticastTtl(int p_ttl)
    {
      return SetSockOpt(IPPROTO_IP, IP_MULTICAST_TTL, &p_ttl, sizeof(p_ttl));
    }

    int SetMulticastLoop(int p_loop)
    {
      return SetSockOpt(IPPROTO_IP, IP_MULTICAST_LOOP, &p_loop, sizeof(p_loop));
    }

    int SetMulticastIf(const char *p_ifname)
    {
      return SetSockOpt(IPPROTO_IP, IP_MULTICAST_IF, p_ifname, sizeof(p_ifname));
    }
};

class UnixSocket : public Socket
{
  public:
    UnixSocket() : Socket(AF_UNIX, SOCK_STREAM)
    {
    }
};

} // namespace sp
