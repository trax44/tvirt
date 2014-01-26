#include "Requester.hpp"

namespace tvirt {

namespace master {

Requester::Requester(const std::string &address,
                     const uint16_t port):
  socket(address, port, ZMQ_REQ){}



const Return<const Any> Requester::getReplyHeader(){
  Return<int> r = socket.recv(&buffer);
  
  if (r.data <= 0){
    //    socket.flush();
    return Return<const Any>(false, Any(&replyHeader));
  }

  if (!replyHeader.SerializeToString(&buffer)) {
    return false;
  }

  return &replyHeader;
}


const Return<const Any> Requester::getReplyBody(){
  Return<int> ret = socket.recv(&buffer);
  
  if (ret.data <= 0){
    //    socket.flush();
    return false;
  }

  return &buffer;
} 


} // master

} // tvirt




















