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



Return<void> Virt::getListAllDomains(Hypervisor &hypervisor){
    virDomainPtr *domainList;

    int n = virConnectListAllDomains(conn, 
                                     &domainList,
                                     VIR_CONNECT_LIST_DOMAINS_ACTIVE |VIR_CONNECT_LIST_DOMAINS_INACTIVE);
    std::vector<unsigned char> tmp (VIR_UUID_BUFLEN);
    
    for (int i = 0 ; i < n ; ++i){
        Guest* guest = hypervisor.add_guests();
        
        guest->mutable_host()->set_name(virDomainGetName(domainList[i]));
        // guest->mutable_host()->set_nbcpu(virDomainGetMaxVcpus(domainList[i]));
        guest->set_id(reinterpret_cast<uint64_t>(domainList[i]));
        guest->set_active(((virDomainIsActive(domainList[i]) == 1)?true:false));

    }

    if (n > 0){
        return true;
    } else {
        return false;
    }
}

// PUBLIC ======================================================================



const tvirt::Hypervisor & Virt::getHypervisor() {

  hypervisor.set_type(Hypervisor_Type_UNKNOWN);
  hypervisor.mutable_host()->set_name(virConnectGetHostname(conn));
  
  getListAllDomains (hypervisor);

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














