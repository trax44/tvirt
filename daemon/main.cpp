#include <iostream>
#include "libvirt/Virt.hpp"

#include "../comm/ClientZmq.hpp"
#include "../comm/ServerZmq.hpp"





int main(int argc, char *argv[]) {

  bool modeSync = true;


  //\TODO use boost::program_options or better

  if (argc == 2) {
    if (strcmp(argv[1], "--async") == 0){
      modeSync = false;
    } else {
      return -1;
    }
  } else {
    return -1;
  }


  

  tvirt::daemon::Virt virt;
  const tvirt::Hypervisor &hypervisor = virt.getHypervisor();
  

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










