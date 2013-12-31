#ifndef TVIRT_COMM_COMM_HPP_
#define TVIRT_COMM_COMM_HPP_


#include <string>
#include <zmq.h>
#include <stdint.h>
#include "../utils/Return.hpp" 




namespace tvirt {

namespace comm {


class Client {
private:
  std::string address;
  uint16_t port;
  virtual Return<std::string> connect() = 0;

public:
  Client(const std::string &address, const uint16_t port);

  virtual ~Client();
};


} //comm

} //tvirt


#endif /* TVIRT_COMM */
