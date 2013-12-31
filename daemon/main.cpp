#include <iostream>
#include "libvirt/Virt.hpp"


int main(int argc, char *argv[]) {
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

  // Return<tvirt::daemon::Virt::List> rList = virt.getListAllVm();

  // if (!rList.success){
  //   std::cerr << "FAIL" << std::endl;
  //   return -1;
  // }

  // std::cout << rList.data.size() << " domain found" << std::endl;

  // for (tvirt::daemon::Virt::List::const_iterator 
  //        it = rList.data.begin() , end = rList.data.end();
  //      it != end ; ++it){

  //   std::cout << "vm> " << it->getName() << std::endl;
    
  // }

  return 0;
}










