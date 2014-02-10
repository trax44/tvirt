#include "Requester.hpp"

namespace tvirt {

namespace master {

Requester::Requester(const std::string &address,
                     const uint16_t port):
  socket(address, port, ZMQ_REQ){}



Return<tvirt::Reply> Requester::executeCommand (const tvirt::Request &request,
                                                std::string *replyBuffer) {
  
  tvirt::Reply replyHeader;
  std::string buffer;
  request.SerializeToString(&buffer);
  
  socket.send(buffer);

  std::string data;
  Return<int> ret = socket.recv(&data);
  
  if (ret.data == -1){
    return false;
  }
  
  replyHeader.ParseFromString(data);
  
  if (ret.success){
    Return<int> retBody = socket.recv(replyBuffer);
    if (retBody.data == -1 || retBody.success) {
      return Return<tvirt::Reply>(false, replyHeader);
    }
  }

  return Return<tvirt::Reply>(true, replyHeader);
}



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




















