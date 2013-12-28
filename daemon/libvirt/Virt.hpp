#ifndef TVIRT_DAEMON_VIRT_HPP_
#define TVIRT_DAEMON_VIRT_HPP_

#include <list>

#include "utils/Return.hpp"
#include "Vm.hpp"

namespace tvirt {
namespace daemon {
    
    
class Virt {
public:
  typedef std::list<Vm> List;

private:
  virConnectPtr conn; 

public:
  Virt();
  Return<List> getListInactiveVm();
  Return<List> getListActiveVm()
  Return<List> getListAllVm();
  virtual ~Virt();
};

    
} //daemon
  
} //tvirt

#endif /* TVIRT_DAEMON */
