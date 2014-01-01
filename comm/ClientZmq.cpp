#include <stdexcept>

#include "ClientZmq.hpp"


namespace tvirt {

namespace comm {

ClientZmq::ClientZmq(const std::string &address, 
                     const uint16_t port,
                     const int type):
  Zmq::Zmq(address, port, type){


  {
    Return<std::string> res = init(type);
    if (!res.success) {
      throw std::runtime_error(res.data);
    }
  }

  {
    Return<std::string> res = connect();
    if (!res.success) {
      throw std::runtime_error (res.data);
    }
  }
}

Return<std::string> ClientZmq::connect() {
  if (socket == NULL){
    return Return<std::string> (false, "Socket not initialized");
  }
  
  if (zmq_connect (socket, address.c_str()) != 0){
    return Return<std::string>(false, zmq_strerror(zmq_errno()));
  } else {
    return true;
  }
}


} //comm

} //tvirt
