#ifndef TVIRT_COMM_SERVER_HPP_
#define TVIRT_COMM_SERVER_HPP_


#include <string>
#include <stdint.h>
#include "../utils/Return.hpp" 




namespace tvirt {

namespace comm {


class Server {
protected:
  std::string address;
  uint16_t port;
  virtual Return<std::string> bind() = 0;

public:
  Server(const std::string &address, const uint16_t port);

  virtual ~Server();
};


} //comm

} //tvirt


#endif /* TVIRT_COMM */
