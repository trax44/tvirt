#include <iostream>
#include "libvirt/Virt.hpp"


int main(int argc, char *argv[]) {
  tvirt::daemon::Virt virt;

  Return<tvirt::daemon::Virt::List> rList = virt.getListAllVm();

  if (!rList.success){
    std::cerr << "FAIL" << std::endl;
    return -1;
  }

  std::cout << rList.data.size() << " domain found" << std::endl;

  for (tvirt::daemon::Virt::List::const_iterator 
         it = rList.data.begin() , end = rList.data.end();
       it != end ; ++it){

    std::cout << "vm> " << it->getName() << std::endl;
    
  }

  return 0;
}










