#include <Wt/WMessageBox>

#include "Hypervisors.hpp"

#include "HypervisorConnect.hpp"

namespace tvirt {

namespace master {

namespace web {

Hypervisors::Hypervisors(ViewControler & _controler, Wt::WContainerWidget *parent) :
  controler (_controler),
  Wt::WContainerWidget(parent) {

  layout = new Wt::WVBoxLayout();
  auto addButton = new Wt::WPushButton("Add");
  addButton->clicked().connect(this, &Hypervisors::add);
  layout->addWidget(addButton, 0);

  this->setLayout(layout);

}


void Hypervisors::addHypervisor(const std::string & address, const uint16_t port) {
  std::cout << "checking hypervision connection information" << std::endl;

  Return<ViewControler::ConnectionID> connection = controler.addConnection(address, port);
  
  if (connection.success) {
    auto r = controler.connectToHypervisor(connection.data);
    if (!r.success){
      return;
    }
    
    web::Hypervisor *hypervisor = new web::Hypervisor (connection.data, r.data);
    hypervisor->action().connect(this, &Hypervisors::handleGuestAction);
    layout->addWidget(hypervisor);

    
  } else {
    Wt::WMessageBox * notification = 
      new Wt::WMessageBox("Fail", 
                          "Fail to connect to hypervisor<br/> Retry?", 
                          Wt::Critical, 
                          Wt::Yes|Wt::No);

    notification->buttonClicked().connect(std::bind([=] () {
          if (notification->buttonResult() == Wt::Yes) {
            add();
          }
          delete notification;
        }));
    notification->show();
  }
}

void Hypervisors::removeHypervisorDialog(){
  std::cout << "hyp res " << hypervisorConnection->result() << std::endl;

  //delete hypervisorConnection;
  hypervisorConnection = NULL;
}


void Hypervisors::handleGuestAction (const ViewControler::ConnectionID connectionID, 
                                const uint64_t guestID, 
                                const proto::Type action){

  controler.doActionOnGuest(connectionID, guestID, action);
}

void Hypervisors::add(){
  hypervisorConnection = new HypervisorConnect();
  hypervisorConnection->res().connect(this, &Hypervisors::addHypervisor);
  hypervisorConnection->finished().connect(this, &Hypervisors::removeHypervisorDialog);
}



} // web

} // master

} // tvirt


