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

  tvirt::Hypervisor         hypervisor;
  tvirt::MonitoringState    monitoringState;
  Return<void> getListAllDomains(Hypervisor &hypervisor);

public:
  Virt();
  // Return<List> getListInactiveVm();
  // Return<List> getListActiveVm();
  // Return<List> getListAllVm();
  void                                 setGuest          (Guest *guest, 
                                                          const virDomainPtr domainPtr);
  const Return<const Hypervisor&>      getHypervisor     ();
  const Return<const MonitoringState&> getMonitoringState(const DomainID id);
  const Return<void>                   rebootDomain      (const DomainID id);
  const Return<void>                   rebootForceDomain (const DomainID id);
};

    
} //daemon
  
} //tvirt

#endif /* TVIRT_DAEMON */
