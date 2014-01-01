
#ifndef TVIRT_COMM_SERVERZMQ_HPP_
#define TVIRT_COMM_SERVERZMQ_HPP_

#include <zmq.h>
#include "Zmq.hpp"


namespace tvirt {

namespace comm {


class ServerZmq : public Zmq {
protected:
  Return<std::string> bind();
  
  
public:

  ServerZmq(const std::string &address, 
            const uint16_t port, 
            const int type);

};


} //comm

} //tvirt

#endif /* TVIRT_COMM */
