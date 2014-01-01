#include <stdexcept>
#include "ServerZmq.hpp"


namespace tvirt {

namespace comm {


ServerZmq::ServerZmq(const std::string &address, 
                     const uint16_t port,
                     const int type) :
  Server::Server(address, port){
  
  {
    Return<std::string> res = init(type);
    if (!res.success) {
      throw std::runtime_error(res.data);
    }
  }

  {
    Return<std::string> res = bind();
    if (!res.success) {
      throw std::runtime_error (res.data);
    }
  }
  
}

Return<std::string> ServerZmq::bind() {
  if (socket == NULL){
    return Return<std::string> (false, "Socket not initialized");
  }


  if (zmq_bind (socket, address.c_str()) != 0){
    return Return<std::string>(false, zmq_strerror(zmq_errno()));
  } else {
    return true;
  }
}


} //comm

} //tvirt
