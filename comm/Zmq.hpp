#ifndef TVIRT_COMM_ZMQ_HPP_
#define TVIRT_COMM_ZMQ_HPP_

#include <string>
#include <zmq.h>

#include "Comm.hpp"
#include "../utils/Return.hpp"

namespace tvirt {
namespace comm {

class Zmq :public Comm{
protected:
  void *context;
  void *socket;
  int type;
public:
  Zmq (const std::string &address, 
       const uint16_t port,
       const int type): Comm::Comm(address, port), 
                        type(type){}
  Return<std::string>init(const int type);
  int recv(Return<std::string> *ret);
  Return<int>send(const std::string &message);


};


} //comm
} //tvirt


#endif /* TVIRT_COMM */










