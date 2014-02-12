#ifndef TVIRT_MASTER_WEB_HYPERVISOR_H_
#define TVIRT_MASTER_WEB_HYPERVISOR_H_

#include <Wt/WPanel>
#include <Wt/WSignal>
#include "../../../proto/Hypervisor.pb.h"
#include "../../../proto/Request.pb.h"

namespace tvirt {

namespace master {

namespace web {

class Hypervisor : public Wt::WPanel{
public:

private:

  
  Wt::Signal<uint64_t, proto::Type> done_;

  const std::string osName(int i);  
  void emitAction (uint64_t guestID, proto::Type action);
public:
  Hypervisor(const ::tvirt::Hypervisor &hypervisor);
  Wt::Signal<uint64_t, proto::Type>& action() { return done_; }
};

} // web

} // master

} // tvirt

#endif /* TVIRT_MASTER_WEB */










