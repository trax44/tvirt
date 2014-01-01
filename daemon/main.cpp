#include <iostream>

#include "libvirt/Virt.hpp"

#include "../comm/ClientZmq.hpp"


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





