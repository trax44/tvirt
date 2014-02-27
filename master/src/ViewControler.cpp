#include "ViewControler.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>
namespace tvirt {

namespace master {


ViewControler::ViewControler() {}


Return<ViewControler::HypervisorConnection*> 
ViewControler::addConnection (const std::string &address, 
                              uint16_t port) {

  

  try {
    proto::Request requestHandShake;
    std::string replyBuffer;
    Requester requester(address, port);

    requestHandShake.set_type(proto::RequestType::DOMAIN_LIST);

    Return<proto::Reply> reply = requester.executeCommand(requestHandShake, &replyBuffer);
    
    if (!reply.success) {
      return false;
    }
    
    proto::Hypervisor hypervisor;
    if (!reply.data.success() || !hypervisor.ParseFromString(replyBuffer)){
      std::cerr << "Failed to parse received data" << std::endl;
      return false;
    }

    HypervisorConnection *t = new HypervisorConnection (reply.data.uuid(), address, port, requester, hypervisor);
    std::unique_ptr<HypervisorConnection> tt (t);
    hypervisorConnections.insert(std::make_pair(std::string("plop"), t));

    
    return &t;
  } catch (const std::runtime_error& error) {
    std::cout << "failed to connect " <<  error.what() << std::endl;
    return false;
  }

}

Return<void> ViewControler::doActionOnGuest(      HypervisorConnection &hypervisor,
                                            const uint64_t guestID, 
                                            const proto::RequestType actionType) {
  proto::Request request;
  std::string buffer;
  
  request.set_type(actionType);
  request.set_domainid(guestID);
  Return <proto::Reply> reply = hypervisor.requester.executeCommand(request, &buffer);
  return (reply.success && reply.data.success());
}

Return<void> ViewControler::doActionOnGuest(const HypervisorID hypervisorID,
                                            const uint64_t guestID, 
                                            const proto::RequestType actionType) {
  /// \TODO TBD
  return false;
}


Return<void> ViewControler::removeConnection (const std::string &, 
                                              uint16_t port) {
  /// \TODO TBD
  return false;
}

Return<void> ViewControler::removeConnection (const HypervisorID) {
  /// \TODO TBD
  return false;
}



} // master

} // tvirt
