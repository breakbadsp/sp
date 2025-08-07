#include "cmn.hpp"


std::optional<std::pair<std::string, short>> sp::GetIpPort(const struct sockaddr& p_cli_addr)
{
  char ip[INET6_ADDRSTRLEN];
  if(p_cli_addr.sa_family == AF_INET)
  {
    const struct sockaddr_in& addr_in = (const struct sockaddr_in&) (p_cli_addr);
    const char* ip1 = inet_ntop(AF_INET, &(addr_in.sin_addr), ip, INET_ADDRSTRLEN);
    if(ip1 != nullptr)
    {
      return {std::pair(ip1, ntohs(addr_in.sin_port))};
    }
  }
  return {};
}