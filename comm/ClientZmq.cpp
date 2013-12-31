#include "ClientZmq.hpp"


namespace tvirt {

namespace comm {

ClientZmq::ClientZmq(const std::string &address, 
                     const uint16_t port):
  Client(address, port){
  if ((context = zmq_init(1)) == NULL) {
    throw std::runtime_error ("Failed to init zmq with 1 thread ("
                              + zmq_strerror(zmq_errno()) +")");
  }


  Return<std::string> res = connect();
  if (!res.success) {
    throw std::runtime_error (res.data);
  }
}

Return<std::string> connect() {
  if (socket == NULL){
    return Return<std::string> (false, "Socket not initialized");
  }
  
  if ((socket = zmq_connect (socket, context)) == NULL){
    return Return<std::string>(false, zmq_strerror(zmq_errno()));
  } else {
    return true;
  }
}

} //comm

} //tvirt
