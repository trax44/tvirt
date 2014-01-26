#ifndef TVIRT_MASTER_WEB_HYPERVISORCONNECT_H_
#define TVIRT_MASTER_WEB_HYPERVISORCONNECT_H_


#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>

namespace tvirt {

namespace master {

namespace web {

class HypervisorConnect : public Wt::WContainerWidget {
private:
  Wt::WLineEdit     wtAddress;
  Wt::WLineEdit     wtPort;
  void connect();
  
public:
  HypervisorConnect(Wt::WContainerWidget *parent);
};


} // web

} // master

} // tvirt

#endif /* TVIRT_MASTER_WEB */
