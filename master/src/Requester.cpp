#include "Requester.hpp"

namespace tvirt {

namespace master {

Requester::Requester(const std::string &address,
                     const uint16_t port):
  socket(address, port, ZMQ_REQ){}



Return<proto::Reply> Requester::executeCommand (const proto::Request &request,
                                                std::string *replyBuffer) {
  
  proto::Reply replyHeader;
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
      return Return<proto::Reply>(false, replyHeader);
    }
  }

  return Return<proto::Reply>(true, replyHeader);
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




















