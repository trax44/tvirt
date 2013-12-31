#include "ServerZmq.hpp"


namespace tvirt {

namespace comm {


ServerZmq::ServerZmq(const std::string &address, 
                     const uint16_t port) :
  Server::Server(address, port){
  
  if ((context = zmq_init(1)) == NULL) {
    throw std::runtime_error ("Failed to init zmq with 1 thread ("
                              + zmq_strerror(zmq_errno()) +")");
  }
  
  Return<std::string> res = bind();
  if (!res.success) {
    throw std::runtime_error (res.data);
  }
  
}

Return<std::string> ServerZmq::bind() {
  if (socket == NULL){
    return Return<std::string> (false, "Socket not initialized");
  }


  if ((socket = zmq_bind (socket, context)) == NULL){
    return Return<std::string>(false, zmq_strerror(zmq_errno()));
  } else {
    return true;
  }
}


} //comm

} //tvirt
