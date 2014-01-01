#include <stdexcept>
#include "ServerZmq.hpp"


namespace tvirt {

namespace comm {


ServerZmq::ServerZmq(const std::string &address, 
                     const uint16_t port,
                     const int type) :
  Zmq::Zmq(address, port, type){
  
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

  
  if (zmq_bind (socket, endPoint.c_str()) != 0){
    return Return<std::string>(false, zmq_strerror(zmq_errno()));
  } else {
    return true;
  }
}


Return<void> ServerZmq::setFilter(const void *option_value, size_t option_len) {
  if (zmq_setsockopt (socket, ZMQ_SUBSCRIBE, option_value, option_len) != 0){
    std::cerr << "Fail to subscribe" << std::endl;
    return false;
  } else {
    return true;
  }
}

} //comm

} //tvirt
