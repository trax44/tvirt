#include "Zmq.hpp"

#include <sstream>


namespace tvirt {
namespace comm {
void * Zmq::context = NULL;

Return<std::string> Zmq::init(const int type) {
  std::stringstream ss;
  ss << port;


  endPoint = address + ":" + ss.str();

  std::cout << "endPoint " << endPoint << std::endl;

  if (Zmq::context == NULL){
    if ((Zmq::context = zmq_init(1)) == NULL) {
      return Return<std::string> (false, 
                                  "Failed to init zmq with 1 thread ("
                                  + std::string(zmq_strerror(zmq_errno())) +")");
    }

  }
  if ((socket = zmq_socket(Zmq::context, type)) == NULL){
    return Return<std::string> (false, 
                                "Failed to to create socket ("
                                + std::string(zmq_strerror(zmq_errno())) +")");
  }

  return true;
}


Return<int> Zmq::send(const std::string &message, bool more) {
  int n;
  
  if (socket == NULL){
    std::cout << "Socket not initiliazed" << std::endl;
    return false;
    return 0;
  }

  
  if ((n = zmq_send(socket, message.data(), message.size(), (more)?ZMQ_SNDMORE:0)) == -1){
    std::cout << "failed to send " << zmq_strerror(zmq_errno()) << std::endl;
    return false;
  } else {
    return Return<int>(true, n);
  }
}

Return<int> Zmq::send(const std::string &message) {
  return send(message, false);
}


Return<int> Zmq::recv(std::string *data) {
  zmq_msg_t part;
  Return<int> ret(true, 0);

  if (socket == NULL){
    ret.success = false;
    data->append("Socket not initialized");
    return ret;
  }

  ret.data = zmq_msg_init (&part);
  
  if (ret.data != 0){
    data->append(zmq_strerror(zmq_errno()));
    ret.success = false;
    return ret;
  }
  
  ret.data = zmq_msg_recv (&part, socket, 0);
  
  if (ret.data == -1){
    zmq_msg_close (&part); 
    data->append(zmq_strerror(zmq_errno()));
    ret.success = false;
    return ret;
  }

  ret.success = true;
  data->append(static_cast<char*>(zmq_msg_data (&part)), 
               zmq_msg_size(&part));
  

  ret.success = zmq_msg_more(&part);
  
  zmq_msg_close (&part); 


  return ret;
}


} //comm
} //tvirt










