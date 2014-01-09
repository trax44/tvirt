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
  std::string serializedAnswer;

  
  Return<std::string> message(false);
  Request request;
  while (ok) {
    message.data.clear();
    replyer.recv(&message);
    
    if (!message.success){
      std::cerr << "Fail to receive message" << std::endl;
      continue;
    }
    
    if (!request.ParseFromString(message.data)) {
      std::cerr << "Fail during parsing" << std::endl;
    }
    
    serializedAnswer.clear();
    {
      Return<void> r = execute(request, &serializedAnswer);
      if (!r.success) {
        std::cout << "Fail on command execution" << std::endl;
        continue;
      }
    }


    {
      Return<int>  r = replyer.send(serializedAnswer);
      if (!r.success){
        std::cerr << "Failed to send message " << std::endl;
        continue;
      }
    }

    
  }
}



} //Daemon


} //tvirt
