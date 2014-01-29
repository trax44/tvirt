#ifndef TVIRT_MASTER_WEB_HYPERVISORCONNECT_H_
#define TVIRT_MASTER_WEB_HYPERVISORCONNECT_H_


#include <Wt/WDialog>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WIntValidator>
#include <Wt/WSignal>

#include <string>
#include <stdint.h>

namespace tvirt {

namespace master {

namespace web {

class HypervisorConnect : public Wt::WDialog {
private:
  Wt::WLineEdit        wtAddress;
  Wt::WLineEdit        wtPort;
  Wt::WIntValidator    validator;

  void connect();
  Wt::Signal<std::string , uint16_t> done_;
  
  void validate();

public:
  HypervisorConnect();
  Wt::Signal<std::string , uint16_t>& res() { return done_; }
};


} // web

} // master

} // tvirt

#endif /* TVIRT_MASTER_WEB */
