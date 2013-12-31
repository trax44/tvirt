#ifndef TVIRT_COMM_COMM_HPP_
#define TVIRT_COMM_COMM_HPP_


#include <string>
#include <stdint.h>
#include "../utils/Return.hpp" 




namespace tvirt {

namespace comm {


class Server {
private:
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
