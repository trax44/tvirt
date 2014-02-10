#include <stdio.h>
#include <libvirt/libvirt.h>
#include <stdexcept>
	
#include <string>
#include "Virt.hpp"


namespace tvirt {
namespace daemon {


    
Virt::Virt() : 
  conn (NULL){

  // if ((conn = virConnectOpenReadOnly(NULL)) == NULL) {
  if ((conn = virConnectOpen(NULL)) == NULL) {
    throw std::runtime_error ("Failed to connect to hypervisor");
  }
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
  
  monitoringState.set_guestid(id);
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

const Return<void> Virt::rebootDomain (const DomainID id) {
  virDomainPtr domainPtr;

  if ((domainPtr = virDomainLookupByID (conn, id)) == NULL){
    return false;
  }
  
  if (virDomainReboot(domainPtr, VIR_DOMAIN_REBOOT_DEFAULT) == 0){
    return true;
  } else {
    return false;
  }
  
}


const Return<void> Virt::rebootForceDomain (const DomainID id) {
  virDomainPtr domainPtr;

  if ((domainPtr = virDomainLookupByID (conn, id)) == NULL){
    return false;
  }
  
  if (virDomainDestroy(domainPtr) == 0){
    return true;
  } else {
    return false;
  }
  
}

    
} //daemon
  
} //tvirt
