#include "Zmq.hpp"

#include <sstream>


namespace tvirt {
namespace comm {

Return<std::string> Zmq::init(const int type) {
  std::stringstream ss;
  ss << port;


  endPoint = address + ":" + ss.str();

  std::cout << "endPoint " << endPoint << std::endl;

  if ((context = zmq_init(1)) == NULL) {
    return Return<std::string> (false, 
                                "Failed to init zmq with 1 thread ("
                                + std::string(zmq_strerror(zmq_errno())) +")");
  }

  if ((socket = zmq_socket(context, type)) == NULL){
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

int Zmq::recv(Return<std::string> *ret) {
  zmq_msg_t part;

  if (socket == NULL){
    ret->data.append("Socket not initiliazed");
    ret->success = false;
    return 0;
  }

  int rc = zmq_msg_init (&part);
  
  if (rc != 0){
    ret->data.append(zmq_strerror(zmq_errno()));
    ret->success = false;
    return 0;
  }
  
  rc = zmq_msg_recv (&part, socket, 0);
  
  if (rc == -1){
    zmq_msg_close (&part); 
    ret->data.append(zmq_strerror(zmq_errno()));
    ret->success = false;
    return 0;
  }

  ret->success = true;
  ret->data.append(static_cast<char*>(zmq_msg_data (&part)), 
                   zmq_msg_size(&part));
  
  zmq_msg_close (&part); 

  return rc;
}


} //comm
} //tvirt










