#ifndef TVIRT_COMM_ZMQ_HPP_
#define TVIRT_COMM_ZMQ_HPP_

#include <string>
#include "../utils/Return.hpp"

namespace tvirt {
namespace comm {

class Zmq {
public:
  Return<std::string>init(const int type);
  int recv(Return<std::string> *ret);
  Return<void>send(const std::string &message);


};


} //comm
} //tvirt


#endif /* TVIRT_COMM */










