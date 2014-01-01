#include <iostream>

#include "libvirt/Virt.hpp"

#include "../comm/ClientZmq.hpp"
#include "../proto/Request.pb.h"


namespace tvirt {

void handlerRequest(const std::string &address, 
                    const uint16_t port){
  bool ok = true;

  
  comm::ClientZmq requester(address, port, ZMQ_REP);
  
  Return<std::string> ret(false);
  Request request;
  while (ok) {
    ret.data.clear();
    requester.recv(&ret);
    
    if (!ret.success){
      std::cerr << "Fail to receive message" << std::endl;
      continue;
    }

    if (!request.ParseFromString(ret.data)) {
      std::cerr << "Fail during parsing" << std::endl;
    }
    
    switch(request.type()){
    case Request::DOMAIN_DESTROY:
      
      break;
    case Request::DOMAIN_REBOOT:
      break;
    case Request::DOMAIN_START:
      break;
    default:
      std::cerr << "Unknow command " << request.type() << std::endl;
    }
    
  }
}


} //tvirt

int main(int argc, char *argv[]) {
  //\TODO use boost::program_options or better

  if (argc != 3) {
    std::cout << "Usage: " 
              << argv[0] 
              << "address port (e.g. tcp://localhost 5555)"
              << std::endl;
    return -1;
  }

  tvirt::comm::ClientZmq sender(argv[1], atoi(argv[2]), ZMQ_PUB);
  
  tvirt::daemon::Virt virt;
  const tvirt::Hypervisor &hypervisor = virt.getHypervisor();
  

  std::string serializedAnswer;
  hypervisor.SerializeToString(&serializedAnswer);
  Return<int> ret = sender.send(serializedAnswer);

  if (!ret.success){
    std::cerr << "Failed to send message " << std::endl;
  }

  std::cout << "type      " << hypervisor.type()        << std::endl;
  std::cout << "host name " << hypervisor.host().name() << std::endl;
  
  for (int i = 0 , e = hypervisor.guests_size(); i < e ; i++){
    std::cout << "  >" 
              << hypervisor.guests(i).id()
              << ":" 
              << hypervisor.guests(i).host().name() << std::endl;

  }


  return 0;
}





