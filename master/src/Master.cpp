#include "Master.hpp"

#include <algorithm>

namespace tvirt {

namespace master {

Return<ConnectionID> Master::addConnection    (const std::string &address, 
                                               uint16_t port) {
  for (hypervisorConnections::const_iterator 
         i = hypervisorConnections.begin() , 
         e = hypervisorConnections.end() ;
       i != e ; ++i) {

    if (i->port == port && i->address == address) {
      return false;
    }
  }

  hypervisorConnections.push_back (HypervisorConnections(hypervisorconnections.size()+1, 
                                                         address, 
                                                         port));
  

}

Return<void> Master::removeConnection (const std::string &, 
                                       uint16_t port) {

}

Return<void> Master::removeConnection (const ConnectionID) {

}

Return<void> Master::connect          (const ConnectID) {

}

Return<void> Master::connectAll       () {

}

} // master

} // tvirt
