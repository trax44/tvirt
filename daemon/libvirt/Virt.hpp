#ifndef TVIRT_DAEMON_VIRT_HPP_
#define TVIRT_DAEMON_VIRT_HPP_

#include <list>
#include <libvirt/libvirt.h>

#include "../../utils/Return.hpp"
#include "Vm.hpp"
#include "../../proto/Hypervisor.pb.h"
#include "../MonitoringStateListener.hpp"


namespace tvirt {
namespace daemon {


    
class Virt {
public:
  typedef virDomainPtr DomainID;

private:

  typedef std::list<MonitoringStateListener *> MonitoringStateCallBacks;

  pthread_t callBackThread;
  virConnectPtr conn; 


  proto::Hypervisor         hypervisor;
  proto::MonitoringState    monitoringState;
  static volatile bool      pollEvent;
  MonitoringStateCallBacks  monitoringStateCallBacks; 


  Return<void> getListAllDomains (proto::Hypervisor &hypervisor);
  
  static void *callbackLoopStatic  (void *me);
  void        *callbackLoop        ();
  static int   callbackEventStatic (virConnectPtr conn,
                                    virDomainPtr dom,
                                    int event,
                                    int detail,
                                    void *opaque);

  int   callbackEvent              (virConnectPtr conn,
                                    virDomainPtr dom,
                                    int event,
                                    int detail);
public:
  Virt();


  template <typename Target>
  void addListenerMonitoringGuest (MonitoringStateListener *listener){
    monitoringStateCallBacks.push_back(listener);
  }

  void                                        setGuest          (proto::Guest *guest, 
                                                                 const virDomainPtr domainPtr);

  Return<void>                                setHost           (proto::Host *host, 
                                                                 const virDomainPtr domainPtr);
  
  const Return<const proto::Hypervisor&>      getHypervisor     ();
  const Return<const proto::MonitoringState&> getMonitoringState(const DomainID domainPtr);
  const Return<void>                          start             (const DomainID domainPtr);
  const Return<void>                          rebootDomain      (const DomainID domainPtr);
  const Return<void>                          startDomain       (const DomainID domainPtr);
  const Return<void>                          stopDomain        (const DomainID domainPtr);
  const Return<void>                          destroyDomain     (const DomainID domainPtr);

};


} //daemon
  
} //tvirt

#endif /* TVIRT_DAEMON */
