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

  Return<ViewControler::HypervisorConnection*> hypervisorModel = controler.addConnection(address, port);
  
  if (hypervisorModel.success) {
    
    web::Hypervisor *hypervisor = new web::Hypervisor (hypervisorModel.data);
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


void Hypervisors::handleGuestAction (const ViewControler::HypervisorID hypervisorID, 
                                const uint64_t guestID, 
                                const proto::RequestType action){

  controler.doActionOnGuest(hypervisorID, guestID, action);
}

void Hypervisors::add(){
  hypervisorConnection = new HypervisorConnect();
  hypervisorConnection->res().connect(this, &Hypervisors::addHypervisor);
  hypervisorConnection->finished().connect(this, &Hypervisors::removeHypervisorDialog);
}



} // web

} // master

} // tvirt


