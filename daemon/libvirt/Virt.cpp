#include <stdio.h>
#include <libvirt/libvirt.h>
#include <stdexcept>
	
#include <string>
#include "Virt.hpp"


namespace tvirt {
namespace daemon {
    
Virt::Virt() : 
  conn (NULL){

  if ((conn = virConnectOpenReadOnly(NULL)) == NULL) {
    throw std::runtime_error ("Failed to connect to hypervisor");
  }
}


Return<Virt::List> Virt::getListInactiveVm() {
  Virt::List inactiveVms;
  return getListInactiveVm(inactiveVms);
}

Return<Virt::List> Virt::getListInactiveVm(Virt::List &inactiveVms) {
  char *names[32];
  int r;
  if ((r = virConnectListDefinedDomains (conn, &names[0], 32)) < 1){
    return false;
  }

  while (r--) {
    inactiveVms.push_back(Vm(r, names[r]));
  }

  return Return<Virt::List>(inactiveVms);
}


Return<Virt::List> Virt::getListActiveVm() {
  Virt::List inactiveVms;
  return getListActiveVm(inactiveVms);
}

Return<Virt::List> Virt::getListActiveVm(Virt::List &inactiveVms) {
  int ids [32];
  int r;

  
  if ((r = virConnectListDomains (conn, ids, 32)) < 1){
    return false;
  }
  

  while (r--) {
    virDomainPtr virDomain = virDomainLookupByID(conn, ids[r]);
    inactiveVms.push_back(Vm(ids[r], virDomainGetName(virDomain)));
  }

  return Return<Virt::List>(inactiveVms);
}

Return<Virt::List> Virt::getListAllVm() {
  List allVms;
  getListInactiveVm(allVms);
  getListActiveVm(allVms);

  return allVms;
}
    
} //daemon
  
} //tvirt
