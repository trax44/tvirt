#ifndef TVIRT_DAEMON_VIRT_HPP_
#define TVIRT_DAEMON_VIRT_HPP_

#include <list>
#include <libvirt/libvirt.h>

#include "../../utils/Return.hpp"
#include "Vm.hpp"
#include "../../proto/Hypervisor.pb.h"



namespace tvirt {
namespace daemon {
    
class Virt {
public:
  typedef int DomainID;

private:
  
  virConnectPtr conn; 

  tvirt::Hypervisor hypervisor;

  Return<void> getListInactiveVm(Hypervisor &);
  Return<void> getListActiveVm(Hypervisor &);

public:
  Virt();
  // Return<List> getListInactiveVm();
  // Return<List> getListActiveVm();
  // Return<List> getListAllVm();

  const tvirt::Hypervisor & getHypervisor();
  const Return<void> rebootDomain (const DomainID id);

};

    
} //daemon
  
} //tvirt

#endif /* TVIRT_DAEMON */
