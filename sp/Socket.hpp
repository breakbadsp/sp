 #pragma once

#include <sys/types.h> //needed only for some legacy BSD
#include <sys/socket.h>

#include <iostream>


namespace sp
{
class Socket
{
  public:
    Socket() = delete;
    Socket(int p_domain, int p_type) 
    : fd_(socket(p_domain, p_type, 0))
    {
    }

    static CreateInetTCPSocket()
    {

    }

  private:
    int fd_;
    
    
    
};
} // namespace sp
