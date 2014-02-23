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


Return<void> Daemon::execute(const proto::Request & request,
                             std::string *serializedAnswer){
#warning clean ret vs return 
  Return<void> ret = true; 
  std::cout << "executing command " 
            << request.type()
            << std::endl;

  
  switch(request.type()){
  case proto::DOMAIN_DESTROY:
    if (!request.has_domainid()){
      return false;
    }
    ret =  virt.destroyDomain(reinterpret_cast<virDomainPtr>(request.domainid()));
    serializedAnswer->clear();
    break;

  case proto::DOMAIN_REBOOT:
    if (!request.has_domainid()){
      return false;
    }
    ret =  virt.rebootDomain(reinterpret_cast<virDomainPtr>(request.domainid()));
    serializedAnswer->clear();
    break;

  case proto::DOMAIN_START:
    if (!request.has_domainid()){
      return false;
    }
    serializedAnswer->clear();
    ret =  virt.startDomain(reinterpret_cast<virDomainPtr>(request.domainid()));
    serializedAnswer->clear();
    break;

  case proto::DOMAIN_LIST:
    {
      const Return<const Hypervisor &>hypervisor = virt.getHypervisor();
      if (hypervisor.success){
        hypervisor.data.SerializeToString(serializedAnswer);
        ret = true;
      } else {
        ret = false;
      }
    }
    break;
  case proto::DOMAIN_GET_STATE:
    {
      if (!request.has_domainid()){
        return false;
      }
      const Return<const MonitoringState &> monitoringState = 
        virt.getMonitoringState(reinterpret_cast<virDomainPtr>(request.domainid()));
      if (!monitoringState.success) {
        return false;
      }
      monitoringState.data.SerializeToString(serializedAnswer);
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

  proto::Reply replyHeader;
  std::string replyHeaderBuffer;

  std::string replyBodyBuffer;


  std::string requestBuffer;
  proto::Request request;
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
    
    replyHeaderBuffer.clear();
    replyBodyBuffer.clear();
    
    Return<void> execReturn = execute(request, &replyBodyBuffer);

    if (!execReturn.success) {
      std::cout << "failed to execute command " << request.type() << std::endl;
    }

    replyHeader.set_type(request.type());
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
    

    if ((replyHeaderBuffer.size() > 0)) {
      Return<int>  sendReturn = replyer.send(replyBodyBuffer, false);
    }
  }
}



} //Daemon


} //tvirt
