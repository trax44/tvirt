#ifndef TVIRT_MASTER_WEB_MASTER_H_
#define TVIRT_MASTER_WEB_MASTER_H_


#include <stdint.h>
#include <string>
#include <list>

#include "ViewModel.hpp"
#include "Requester.hpp"
#include "../../utils/Return.hpp"

namespace tvirt {

namespace master {

class ViewControler {
public:
  typedef uint16_t ConnectionID;
  typedef uint64_t GuestID;
  
private:
  enum ConnectionState{
    UP, DOWN
  };

  struct HypervisorConnection {
    HypervisorConnection (const ConnectionID id,
                          const std::string address,
                          const uint16_t port):
      id(id),
      address(address),
      port(port), 
      requester(address, port){}

    
    const ConnectionID id;
    const std::string  address;
    const uint16_t     port;
    ConnectionState    state;
    Requester          requester;
  };

  typedef std::list<HypervisorConnection> Connections;

  Connections hypervisorConnections;
  ViewModel *view;
  
public:
  ViewControler(ViewModel *view);

  Return<ConnectionID> addConnection    (const std::string &, uint16_t port);
  Return<void>         removeConnection (const std::string &, uint16_t port);
  Return<void>         removeConnection (const ConnectionID);

  Return <ViewControler::HypervisorConnection *> 
  getConnection(const ConnectionID connectionID);
  
  Return<Hypervisor>         connectToHypervisor(const ConnectionID connectionID);
};

} // master

} // tvirt

#endif /* TVIRT_MASTER_WEB */
