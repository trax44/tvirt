
#ifndef TVIRT_COMM_SERVERZMQ_HPP_
#define TVIRT_COMM_SERVERZMQ_HPP_

#include <zmq.h>
#include "Server.hpp"


namespace tvirt {

namespace comm {


class ServerZmq : public Server {
protected:
  void *context;
  void *socket;

  Return<std::string> bind();
  
  
public:

  ServerZmq(const std::string &address, 
            const uint16_t port, 
            const int type);

};


} //comm

} //tvirt

#endif /* TVIRT_COMM */
