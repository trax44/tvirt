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
  typedef virDomainPtr DomainID;

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

  Return<void>                         setHost           (Host *host, 
                                                          const virDomainPtr domainPtr);

  const Return<const Hypervisor&>      getHypervisor     ();
  const Return<const MonitoringState&> getMonitoringState(const DomainID domainPtr);
  const Return<void>                   start             (const DomainID domainPtr);
  const Return<void>                   rebootDomain      (const DomainID domainPtr);
  const Return<void>                   startDomain       (const DomainID domainPtr);
  const Return<void>                   stopDomain        (const DomainID domainPtr);
  const Return<void>                   destroyDomain     (const DomainID domainPtr);
};

    
} //daemon
  
} //tvirt

#endif /* TVIRT_DAEMON */
