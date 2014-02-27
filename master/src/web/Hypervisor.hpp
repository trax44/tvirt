#ifndef TVIRT_MASTER_WEB_HYPERVISOR_H_
#define TVIRT_MASTER_WEB_HYPERVISOR_H_

#include <Wt/WPanel>
#include <Wt/WSignal>
#include "../ViewControler.hpp"
#include "../../../proto/Hypervisor.pb.h"
#include "../../../proto/Request.pb.h"

namespace tvirt {

namespace master {

namespace web {

class Hypervisor : public Wt::WPanel{
public:

private:

  const ViewControler::HypervisorConnection *hypervisor;
  Wt::Signal<const ViewControler::HypervisorID, 
             const uint64_t, 
             const proto::RequestType> done_;

  const std::string osName(int i);  
  void emitAction (const ViewControler::HypervisorID,
                   const uint64_t guestID, 
                   const proto::RequestType action);
public:
  Hypervisor(const ViewControler::HypervisorConnection *hypervisor);
  Wt::Signal<const ViewControler::HypervisorID, 
             const uint64_t, 
             const proto::RequestType>& action() { return done_; }
};

} // web

} // master

} // tvirt

#endif /* TVIRT_MASTER_WEB */










