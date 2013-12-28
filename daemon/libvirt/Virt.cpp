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


Return<Virt::List> getListInactiveVm() {
  char *names[32];
  int r;
  if ((r = virConnectListDefinedDomains (conn, &names, sizeof(ids)>>2)) < 1){
    return false;
  }

  List inactiveVms;


  while (r--) {
    inactiveVms.push_back(Vm(r, names[r]))xs;
  }

  return inactiveVms;
}


Virt::List Virt::getListActiveVm(){
  int ids [32];

  virConnectListDomains (conn, ids, sizeof(ids)>>2);
  
}

Virt::List Virt::getListAllVm() {
  
}
    
} //daemon
  
} //tvirt

static void getDomainInfo(int id) {
  virConnectPtr conn = NULL; /* the hypervisor connection */
  virDomainPtr dom = NULL;   /* the domain being checked */
  virDomainInfo info;        /* the information being fetched */
  int ret;

  /* NULL means connect to local Xen hypervisor */
  conn = virConnectOpenReadOnly(NULL);
  if (conn == NULL) {
    fprintf(stderr, "Failed to connect to hypervisor\n");
    goto error;
  }

  /* Find the domain of the given id */
  dom = virDomainLookupByID(conn, id);
  if (dom == NULL) {
    fprintf(stderr, "Failed to find Domain %d\n", id);
    goto error;
  }

  /* Get the information */
  ret = virDomainGetInfo(dom, &info);
  if (ret < 0) {
    fprintf(stderr, "Failed to get information for Domain %d\n", id);
    goto error;
  }

  printf("Domains %d: %d CPUs\n", id, info.nrVirtCpu);

 error:
  if (dom != NULL)
    virDomainFree(dom);
  if (conn != NULL)
    virConnectClose(conn);
}

int main() {

  getDomainInfo(0);

  return(0);
}
