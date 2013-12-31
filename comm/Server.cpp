#include "Server.hpp"

namespace tvirt {

namespace comm {


Server::Server(const std::string &address, 
           const uint16_t port) :
  address(address),
  port(port){}


} //comm

} //tvirt


