#ifndef TVIRT_COMM_CLIENTZMQ_HPP_
#define TVIRT_COMM_CLIENTZMQ_HPP_

#include "Client.hpp"
#include "../utils/Return.hpp"

namespace tvirt {

namespace comm {

class ClientZmq: public Client {
private:
  Return<std::string> connec();

public:
  ClientZmq(const std::string &address, const uint16_t port);
};
} //comm


} //tvirt

#endif /* TVIRT_COMM */










