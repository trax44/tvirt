#include "Zmq.hpp"


namespace tvirt {
namespace comm {

Return<std::string> Zmq::init(const int type){
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


Return<int> Zmq::send(const std::string &message) {
  int n;
  if ((n = zmq_send(socket, message.data(), message.size(), 0)) > 0){
    return false;
  } else {
    return Return<int>(true, n);
  }
}

int Zmq::recv(Return<std::string> *ret) {
  zmq_msg_t part;
  int rc = zmq_msg_init (&part);
  
  if (rc != 0){
    ret->success = false;
    return 0;
  }
  
  rc = zmq_msg_recv (&part, socket, 0);
  
  if (rc != -1){
    zmq_msg_close (&part); 
    ret->success = false;
    return 0;
  }

  ret->data.append(static_cast<char*>(zmq_msg_data (&part)), 
                   zmq_msg_size(&part));
  
  zmq_msg_close (&part); 

  return rc;
}


} //comm
} //tvirt










