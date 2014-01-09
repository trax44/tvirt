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


Return<void> Virt::getListInactiveVm(Hypervisor &hypervisor) {
  char *names[32];
  int r;
  if ((r = virConnectListDefinedDomains (conn, &names[0], 32)) < 1){
    return false;
  }

  while (r--) {
    Guest* guest = hypervisor.add_guests();

    guest->mutable_host()->set_name(names[r]);
    std::cout << "NAMES " << names[r] << std::endl;
    guest->set_id(-1);
  }

  return true;
}



Return<void> Virt::getListActiveVm(Hypervisor &hypervisor) {
  int ids [32];
  int r;

  
  if ((r = virConnectListDomains (conn, ids, 32)) < 1) {
    return false;
  }
  

  while (r--) {
    Guest* guest = hypervisor.add_guests();

    virDomainPtr virDomain = virDomainLookupByID(conn, ids[r]);
    std::cout << "NAMES " 
              << virDomainGetName(virDomain) 
              << ":" 
              << ids[r] << std::endl;

    guest->mutable_host()->set_name(virDomainGetName(virDomain));
    guest->set_id(ids[r]);
  }
  return true;
}
// PUBLIC ======================================================================


const tvirt::Hypervisor & Virt::getHypervisor() {

  hypervisor.set_type(Hypervisor_Type_UNKNOWN);
  hypervisor.mutable_host()->set_name(virConnectGetHostname(conn));
  
  getListActiveVm(hypervisor);
  getListInactiveVm(hypervisor);

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














