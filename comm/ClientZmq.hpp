#ifndef TVIRT_COMM_CLIENTZMQ_HPP_
#define TVIRT_COMM_CLIENTZMQ_HPP_

#include "Zmq.hpp"


namespace tvirt {

namespace comm {

class ClientZmq: public Zmq{
private:

  Return<std::string> connect();

public:
  ClientZmq(const std::string &address, 
            const uint16_t port,
            const int type);
};
} //comm


} //tvirt

#endif /* TVIRT_COMM */










