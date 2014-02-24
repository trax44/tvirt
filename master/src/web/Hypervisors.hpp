#ifndef TVIRT_MASTER_WEB_HYPERVISORS_HPP_
#define TVIRT_MASTER_WEB_HYPERVISORS_HPP_

#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <unordered_map>
#include <utility>

#include "../ViewControler.hpp"
#include "Hypervisor.hpp"
#include "HypervisorConnect.hpp"


namespace tvirt {

namespace master {

namespace web {

class Hypervisors  : public Wt::WContainerWidget {
private:
  ViewControler &controler;
  HypervisorConnect *hypervisorConnection;

  Wt::WVBoxLayout *layout;
  std::unordered_map<ViewControler::ConnectionID, Hypervisor*> hypervisors;

  void addHypervisor(const std::string &address, const uint16_t port);
  void removeHypervisorDialog();
  void handleGuestAction (const ViewControler::ConnectionID connectionID, 
                          const uint64_t guestID, 
                          const proto::Type action);
public:
  Hypervisors(ViewControler & _controler, Wt::WContainerWidget *parent = 0);

  void add();

};

} // web

} // master

} // tvirt

#endif /* TVIRT_MASTER_WEB */
