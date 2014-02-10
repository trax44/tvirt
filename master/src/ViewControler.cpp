#include "ViewControler.hpp"

#include <algorithm>
#include <stdexcept>

namespace tvirt {

namespace master {


ViewControler::ViewControler(ViewModel *view) : 
  view (view){}


Return<ViewControler::ConnectionID> 
ViewControler::addConnection (const std::string &address, 
                              uint16_t port) {

  
  
  for (Connections::const_iterator 
         i = hypervisorConnections.begin() , 
         e = hypervisorConnections.end() ;
       i != e ; ++i) {

    if (i->port == port && i->address == address) {
      std::cout << "This connection already exist" << std::endl;
      return false;
    }
  }
  
  ConnectionID id;

  try {
    
    Requester requester(address, port);
    id = hypervisorConnections.size();
    
    HypervisorConnection t (hypervisorConnections.size(), address, port);
    hypervisorConnections.push_back (t);
    
  } catch (const std::runtime_error& error) {
    std::cout << "failed to connect " <<  error.what() << std::endl;
    return false;
  }

  return id;
}




Return<Hypervisor> ViewControler::connectToHypervisor(const ConnectionID connectionID){
  auto r = getConnection(connectionID);
  if (!r.success){
    return false;
  }

  std::string buffer;
  tvirt::Request request;

  request.set_type(tvirt::DOMAIN_LIST);
  Return <Reply> reply = r.data->requester.executeCommand(request, &buffer);

  if (!reply.success){
    return false;
  }

  Hypervisor hypervisor;
  if (!reply.data.success() || !hypervisor.ParseFromString(buffer)){
    std::cerr << "Failed to parse received data" << std::endl;
    return false;
  }

  return hypervisor;
}

Return <ViewControler::HypervisorConnection *> 
ViewControler::getConnection(const ConnectionID connectionID) {
  auto it = std::find_if (hypervisorConnections.begin(), 
                          hypervisorConnections.end(), 
                          [connectionID](HypervisorConnection &i)->bool{
                            return (connectionID == i.id);
                          }
                          );
  
  if (it != hypervisorConnections.end()) {
    return &(*it);
  } else {
    return {false, NULL};
  }
}


Return<void> ViewControler::removeConnection (const std::string &, 
                                       uint16_t port) {
  return false;
}

Return<void> ViewControler::removeConnection (const ConnectionID) {
  return false;
}



} // master

} // tvirt
