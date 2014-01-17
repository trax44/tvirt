#include "Daemon.hpp"


namespace tvirt {
namespace daemon {


Daemon::Daemon(const std::string &address,
               const uint16_t portPublisher,
               const uint16_t portRequest):

  address(address),
  portPublisher(portPublisher),
  portRequest(portRequest),
  publisher(address, portPublisher, ZMQ_PUB),
  replyer(address, portRequest, ZMQ_REP){

  handlerRequest();
}


Return<void> Daemon::execute(const Request & request,
                             std::string *serializedAnswer){
  // \TODO: avoid copy of hypervisor (getHypervisor return reference)
  Return<void> ret = true; 
  std::cout << "executing command " << std::endl;
  
  switch(request.type()){
  case Request::DOMAIN_DESTROY:
    if (!request.has_domainid()){
      return false;
    }
    ret =  virt.rebootForceDomain(request.domainid());
    break;

  case Request::DOMAIN_REBOOT:
    if (!request.has_domainid()){
      return false;
    }
    ret =  virt.rebootDomain(request.domainid());
    break;

  case Request::DOMAIN_START:
    if (!request.has_domainid()){
      return false;
    }
    return ((virDomainCreate(reinterpret_cast<virDomainPtr>(request.domainid())) == 0)?true:false);
    break;

  case Request::DOMAIN_LIST:
    {
      const Hypervisor &hypervisor = virt.getHypervisor();
      hypervisor.SerializeToString(serializedAnswer);
      ret = true;
    }
    break;

  default:
    std::cerr << "Unknow command " << request.type() << std::endl;
    ret = false;
  }
  return ret;
}

void Daemon::handlerRequest(){
  
  bool ok = true;

  Reply replyHeader;
  std::string replyHeaderBuffer;

  std::string replyBodyBuffer;


  std::string requestBuffer;
  Request request;
  while (ok) {
    requestBuffer.clear();
    Return<int> r = replyer.recv(&requestBuffer);
    
    if (r.data == -1){
      std::cerr << "Fail to receive message " << requestBuffer << std::endl;
      continue;
    }
    
    if (!request.ParseFromString(requestBuffer)) {
      std::cerr << "Fail during parsing" << std::endl;
    }
    
    replyBodyBuffer.clear();
    
    Return<void> execReturn = execute(request, &replyBodyBuffer);

    
    replyHeader.set_success(execReturn.success);
    if(request.has_requestid()){
      replyHeader.set_requestid(request.requestid());
    }
    
    replyHeader.SerializeToString(&replyHeaderBuffer);


    Return<int>  sendReturn = replyer.send(replyHeaderBuffer, true);
    if (!sendReturn.success){
      std::cerr << "Failed to send message " << std::endl;
      ok = false;
    }
    

    if (execReturn.success) {
      Return<int>  sendReturn = replyer.send(replyBodyBuffer, false);
    }
  }
}



} //Daemon


} //tvirt
