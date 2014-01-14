#ifndef TVIRT_COMM_COMM_HPP_
#define TVIRT_COMM_COMM_HPP_

#include <string>
#include <stdint.h>

#include "../utils/Return.hpp"


namespace tvirt {

namespace comm {

class Comm {
protected:
  std::string address;
  uint16_t port;

public:
  Comm(const std::string &address, 
       const uint16_t port) :   address(address),
                                port(port){}

  virtual Return<int> recv(std::string *) = 0;
  virtual Return<int>send(const std::string &message) = 0;
};


} //comm
} //tvirt

#endif /* TVIRT_COMM */













