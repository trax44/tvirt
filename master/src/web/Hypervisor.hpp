#ifndef TVIRT_MASTER_WEB_HYPERVISOR_H_
#define TVIRT_MASTER_WEB_HYPERVISOR_H_

#include <Wt/WPanel>
#include <Wt/WSignal>
#include "../../../proto/Hypervisor.pb.h"

namespace tvirt {

namespace master {

namespace web {

class Hypervisor : public Wt::WPanel{
private:
  enum Action{
    BOOT,
    REBOOT,
    SEND_COMMAND,
    REFRESH
  };

  
  Wt::Signal<uint64_t, Action> done_;

  const std::string osName(int i);  
  void emitAction (uint64_t guestID, Action action);
public:
  Hypervisor(const ::tvirt::Hypervisor &hypervisor);
  Wt::Signal<uint64_t, Action>& action() { return done_; }
};

} // web

} // master

} // tvirt

#endif /* TVIRT_MASTER_WEB */










