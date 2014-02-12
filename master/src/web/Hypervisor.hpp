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

  const ViewControler::ConnectionID connectionID;
  Wt::Signal<const ViewControler::ConnectionID, 
             const uint64_t, 
             const proto::Type> done_;

  const std::string osName(int i);  
  void emitAction (const ViewControler::ConnectionID,
                   const uint64_t guestID, 
                   const proto::Type action);
public:
  Hypervisor(const ViewControler::ConnectionID connectionID, 
             const ::tvirt::Hypervisor &hypervisor);
  Wt::Signal<const ViewControler::ConnectionID, 
             const uint64_t, 
             const proto::Type>& action() { return done_; }
};

} // web

} // master

} // tvirt

#endif /* TVIRT_MASTER_WEB */










