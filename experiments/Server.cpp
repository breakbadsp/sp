#include <thread>

#include "Server.hpp"
#include "MsgIO.hpp"

void ServeClient(sp::TcpMsgIO p_cli_io)
{
  std::cout << "Server received new connection from "
    << p_cli_io.GetIpAddr() << ":" << p_cli_io.GetPort() << '\n';
  auto cli_msg = p_cli_io.Read();
  if(cli_msg.has_value())
  {
    std::cout << "Server received:  " << cli_msg.value() << '\n';
    p_cli_io.Write(cli_msg.value());
  }
}

int main()
{
  sp::TcpIpServer server(1234, 10);

  while(true)
  {
    auto cli_sock = server.Accept();
    if(cli_sock->IsValid())
    {
      std::thread t(ServeClient, std::move(cli_sock));
      t.detach();
    }
  }
}
