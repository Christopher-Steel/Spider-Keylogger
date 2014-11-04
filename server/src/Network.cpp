#include "Network.hpp"

#include "BoostSocket.hpp"

#include "AFactory.hpp"

Network::Network(uint16_t port, const std::string &addr)
: _acceptor(AFactory<ASocket>::instance().create("BoostSocket"))
{
  _acceptor->bind(addr, port);
  _acceptor->listen(20);
  _queueAccept();
}

void	Network::poll_clients(/*std::function<void(const std::string &, APacket &)> callback*/)
{
  BoostSocket::_service.poll();
}

void	Network::broadcast(ASocket::t_bytes &buffer, std::size_t size)
{
  for (auto &client : _pendingClients) {
    std::function<void(std::size_t)> w(std::bind(&Network::_onWrite, this, client, std::placeholders::_1));
    client->getSocket()->async_write(buffer.data(), size, w);
  }
}

void	Network::_queueAccept(void)
{
  std::function<void(std::shared_ptr<ASocket>)> acc(std::bind(&Network::_onAccept, this, std::placeholders::_1));

  _acceptor->async_accept(acc);
}

void	Network::_onAccept(std::shared_ptr<ASocket> newSock)
{
  std::cout << "Accepted a spider." << std::endl;
  _pendingClients.emplace_back(new Spider(newSock));
  _queueAccept();
}

void	Network::_onRead(std::shared_ptr<Spider> spider, ASocket::t_bytes &buffer, std::size_t size)
{
  (void)spider;
  //spider->onRead(buffer, size);
  std::cout << "Some spider said: ";
  for (std::uint8_t b : buffer) {
    std::cout << static_cast<char>(b);
  }
  std::cout << "(" << size << ")" << std::endl;
}

void	Network::_onWrite(std::shared_ptr<Spider> spider, std::size_t size)
{
  (void)spider;
  //spider->onWrite(size);
  std::cout << "Sent a message to a spider (" << size << ")" << std::endl;
}
