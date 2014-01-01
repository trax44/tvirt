#include "Daemon.hpp"


namespace tvirt {
namespace daemon {


Daemon::Daemon(const std::string &address,
               const uint16_t port):
  address(address),
  port(port),
  publisher(address, port, ZMQ_PUB),
  replyer(address, port, ZMQ_REP){


}


Return<void> Daemon::execute(const Request::Type command,
                             std::string *serializedAnswer){
  // \TODO: avoid copy of hypervisor (getHypervisor return reference)
  
    switch(command){
    case Request::DOMAIN_DESTROY:
      break;

    case Request::DOMAIN_REBOOT:
      break;

    case Request::DOMAIN_START:
      break;

    case Request::DOMAIN_LIST:
      {
        const Hypervisor &hypervisor = virt.getHypervisor();
        hypervisor.SerializeToString(serializedAnswer);
      }
      break;

    default:
      std::cerr << "Unknow command " << command << std::endl;
      return false;
    }
    return true;
}

void Daemon::handlerRequest(const std::string &address, 
                            const uint16_t port){
  
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
      Return<void> r = execute(request.type(), &serializedAnswer);
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









