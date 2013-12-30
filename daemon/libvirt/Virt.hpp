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
  Return<void> getListInactiveVm(List&);
  Return<void> getListActiveVm(List&);

public:
  Virt();
  Return<List> getListInactiveVm();
  Return<List> getListActiveVm();
  Return<List> getListAllVm();
};

    
} //daemon
  
} //tvirt

#endif /* TVIRT_DAEMON */
