#include <stdio.h>
#include <libvirt/libvirt.h>
#include <stdexcept>
	
#include <memory>
#include <string>

#include <pthread.h>

#include "Virt.hpp"


namespace tvirt {
namespace daemon {

#warning free libvirt structs in destructor 
    
volatile bool Virt::pollEvent = true;


Virt::Virt() : 
  conn (NULL){

  // if ((conn = virConnectOpenReadOnly(NULL)) == NULL) {
  if ((conn = virConnectOpen(NULL)) == NULL) {
    throw std::runtime_error ("Failed to connect to hypervisor");
  }


  if (virEventRegisterDefaultImpl()){
    throw std::runtime_error("Could not register default event loop");
  }


  virDomainPtr *domainList;

  int n = virConnectListAllDomains(conn, 
                                   &domainList,
                                   VIR_CONNECT_LIST_DOMAINS_ACTIVE 
                                   |VIR_CONNECT_LIST_DOMAINS_INACTIVE);
  
    

  if (virConnectDomainEventRegisterAny	
      (conn,
       NULL,
       VIR_DOMAIN_EVENT_ID_LIFECYCLE,
       VIR_DOMAIN_EVENT_CALLBACK(&Virt::callbackEvent),
       this,
       NULL) == -1){
    throw std::runtime_error("Failed to register callback");
  }
    
  
  pthread_create(&callBackThread, NULL, 
                 &Virt::callbackLoopStatic, static_cast<void*>(this));

}

void *Virt::callbackLoopStatic (void *me){
  static_cast<Virt*>(me)->callbackLoop();
  
  return NULL;
}

void *Virt::callbackLoop (){
  while (pollEvent && 
         virConnectIsAlive(conn)  == 1 &&
         virEventRunDefaultImpl() == 0) {
   
    std::cout << "you have an event to read" << std::endl;
    
  }
}

int Virt::callbackEvent(virConnectPtr conn,
                        virDomainPtr dom,
                        int event,
                        int detail,
                        void * opaque){
  std::cout 
    << "EVENT " 
    << virDomainGetName (dom) 
    << " " 
    << event 
    << std::endl;
}


Return<void> Virt::setHost (Host *host, const virDomainPtr domainPtr) {
  virDomainInfo info;
  if (virDomainGetInfo(domainPtr, &info) == -1){
    return false;
  }

  host->set_name(virDomainGetName(domainPtr));

  host->mutable_cpu()->set_nbcpu(info.nrVirtCpu);
  host->mutable_cpu()->set_time(info.cpuTime);
  host->set_memory(info.memory);
  host->set_maxmemory(info.maxMem);

  return true;
}


void Virt::setGuest(Guest *guest, const virDomainPtr domainPtr) {
  setHost (guest->mutable_host(), domainPtr);
  
  guest->set_id(reinterpret_cast<uint64_t>(domainPtr));
  guest->set_active(((virDomainIsActive(domainPtr) == 1)?true:false));
}

Return<void> Virt::getListAllDomains(Hypervisor &hypervisor) {
  virDomainPtr *domainList;

    int n = virConnectListAllDomains(conn, 
                                     &domainList,
                                     VIR_CONNECT_LIST_DOMAINS_ACTIVE 
                                     |VIR_CONNECT_LIST_DOMAINS_INACTIVE);

    std::vector<unsigned char> tmp (VIR_UUID_BUFLEN);
    
    for (int i = 0 ; i < n ; ++i){
        Guest* guest = hypervisor.add_guests();
        setGuest(guest, domainList[i]);
    }

    if (n > 0){
        return true;
    } else {
        return false;
    }
}



// PUBLIC ======================================================================


const Return<const tvirt::MonitoringState &> 
Virt::getMonitoringState (const DomainID id) {
  virDomainInfo info;
  if (virDomainGetInfo(reinterpret_cast<virDomainPtr>(id), &info) == -1){
    return Return<const tvirt::MonitoringState &>(false, monitoringState);
  }
  
  monitoringState.set_guestid(reinterpret_cast<uint64_t>(id));
  monitoringState.mutable_cpu()->set_nbcpu(info.nrVirtCpu);
  monitoringState.mutable_cpu()->set_time(info.cpuTime);
  monitoringState.mutable_memory()->set_used(info.memory);
  monitoringState.mutable_memory()->set_total(info.maxMem);
  monitoringState.set_time(time(NULL));

  
  return monitoringState;
}


const Return<const tvirt::Hypervisor &> Virt::getHypervisor() {
  hypervisor.Clear();
  hypervisor.set_type(Hypervisor_Type_UNKNOWN);
  hypervisor.mutable_host()->set_name(virConnectGetHostname(conn));
  
  Return <void> r = getListAllDomains (hypervisor);
  if (!r.success) {
    return Return<const tvirt::Hypervisor &>(false, hypervisor);
  }

  return hypervisor;
}

const Return<void> Virt::rebootDomain (const DomainID domainPtr) {
  if (virDomainReboot(domainPtr, VIR_DOMAIN_REBOOT_DEFAULT) == 0){
    return true;
  } else {
    return false;
  }
  
}


const Return<void> Virt::startDomain (const DomainID domainPtr) {

  if (virDomainCreate(domainPtr) == 0){
    return true;
  } else {
    return false;
  }
  
}


const Return<void> Virt::stopDomain (const DomainID domainPtr) {

  if (virDomainShutdownFlags(domainPtr, 
                             VIR_DOMAIN_SHUTDOWN_ACPI_POWER_BTN	|
                             VIR_DOMAIN_SHUTDOWN_GUEST_AGENT
                             ) == 0){
    return true;
  } else {
    return false;
  }
  
}

const Return<void> Virt::destroyDomain (const DomainID domainPtr) {

  if (virDomainDestroy(domainPtr) == 0){
    return true;
  } else {
    return false;
  }
  
}

    
} //daemon
  
} //tvirt
