#ifndef TVIRT_MASTER_WEB_MASTER_H_
#define TVIRT_MASTER_WEB_MASTER_H_


#include <stdint.h>
#include <string>
#include <unordered_map>
#include <memory>

#include "Requester.hpp"
#include "../../utils/Return.hpp"
#include "../../proto/Hypervisor.pb.h"
#include "../../proto/Request.pb.h"

namespace tvirt {

namespace master {



class ViewControler {
public:
  typedef uint64_t GuestID;
  typedef std::string HypervisorID;
  enum ConnectionState{
    UP, DOWN
  };

  struct HypervisorConnection {
    HypervisorConnection (const std::string id,
                          const std::string address,
                          const uint16_t port,
                          const Requester requester,
                          const proto::Hypervisor  hypervisor):
      id(id),
      address(address),
      port(port), 
      requester(requester),
      hypervisor(hypervisor)
    {}

    
    const std::string  id;
    const std::string  address;
    const uint16_t     port;
    ConnectionState    state;
    Requester          requester;
    proto::Hypervisor  hypervisor;
    
  };

private:

  /// \TODO should be a std::unique_ptr
  typedef std::unordered_map<HypervisorID, HypervisorConnection* > Connections;

  Connections hypervisorConnections;
  
public:
  ViewControler();

  Return<HypervisorConnection*>
  addConnection    (const std::string &, uint16_t port);

  Return<void>         removeConnection (const std::string &, uint16_t port);
  Return<void>         removeConnection (const HypervisorID);

  Return<void>         doActionOnGuest(const HypervisorID connectionID,
                                       const uint64_t guestID,
                                       const proto::RequestType actionType);
  
  Return<void>         doActionOnGuest(      HypervisorConnection &hypervisor,
                                       const uint64_t guestID, 
                                       const proto::RequestType actionType);
    
};

} // master

} // tvirt

#endif /* TVIRT_MASTER_WEB */
