#ifndef TVIRT_DAEMON_VIRT_HPP_
#define TVIRT_DAEMON_VIRT_HPP_

#include <list>
#include <libvirt/libvirt.h>

#include "../../utils/Return.hpp"
#include "Vm.hpp"

namespace tvirt {
namespace daemon {
    
    
class Virt {
public:
  typedef std::list<Vm> List;

private:
  virConnectPtr conn; 
  Return<List> getListInactiveVm(List&);
  Return<List> getListActiveVm(List&);

public:
  Virt();
  Return<List> getListInactiveVm();
  Return<List> getListActiveVm();
  Return<List> getListAllVm();
};

    
} //daemon
  
} //tvirt

#endif /* TVIRT_DAEMON */
