#include "Client.hpp"


namespace tvirt {

namespace comm {


Client::Client(const std::string &address, 
           const uint16_t port) :
  address(address),
  port(port){}

} //comm

} //tvirt



