#include <list>

#include <iostream>
#include <stdio.h>
#include <string>

#include <pthread.h>
#include <unistd.h>

#include <Wt/WApplication>
#include <Wt/WLayout>
#include <Wt/WDialog>
#include <Wt/WServer>
#include <Wt/WMessageBox>
#include <Wt/WBootstrapTheme>
#include <Wt/WTable>
#include <Wt/WText>

#include "../Requester.hpp"
#include "HypervisorConnect.hpp"
#include "Hypervisor.hpp"
#include "../ViewModel.hpp"
#include "../ViewControler.hpp"

namespace tvirt {

namespace master  {

namespace web {



class WebGUI : public Wt::WApplication ,
               public ViewModel {
public:
  WebGUI (const Wt::WEnvironment& env);

private:
  ViewControler controler;
  
  void request();
  std::list<Requester> hypervisorsConnection;
  HypervisorConnect *hypervisorConnection;

  Wt::WVBoxLayout globalLayout;

  void addHypervisor(const std::string address, uint16_t port);
  void removeHypervisorDialog();
  void askForHypervisorConnection();
};



WebGUI::WebGUI(const Wt::WEnvironment& env)
  : Wt::WApplication(env),
    controler(this) {

  setTitle("Enyx Cloud Administration Tool");
  //useStyleSheet("web/css.css");
  
  root()->setLayout(&globalLayout);
  askForHypervisorConnection();
  setTheme(new Wt::WBootstrapTheme());
  //setCssTheme("polished");
}

void WebGUI::removeHypervisorDialog(){
  std::cout << "hyp res " << hypervisorConnection->result() << std::endl;

  delete hypervisorConnection;
  hypervisorConnection = NULL;
}

void WebGUI::askForHypervisorConnection(){
  hypervisorConnection = new HypervisorConnect();
  hypervisorConnection->res().connect(this, &WebGUI::addHypervisor);
  hypervisorConnection->finished().connect(this, &WebGUI::removeHypervisorDialog);
  
}


void WebGUI::addHypervisor(const std::string address, uint16_t port) {
  std::cout << "checking hypervision connection information" << std::endl;
  Return<ViewControler::ConnectionID> connection = controler.addConnection(address, port);
  
  if (connection.success) {
    auto r = controler.connectToHypervisor(connection.data);
    if (!r.success){
      return;
    }
    
    globalLayout.addWidget(new web::Hypervisor(r.data), 0);
    globalLayout.addWidget(new Wt::WText(""), 1);
    
  } else {
    Wt::WMessageBox * notification = 
      new Wt::WMessageBox("Fail", 
                          "Fail to connect to hypervisor<br/> Retry?", 
                          Wt::Critical, 
                          Wt::Yes|Wt::No);

    notification->buttonClicked().connect(std::bind([=] () {
          if (notification->buttonResult() == Wt::Yes) {
            askForHypervisorConnection();
          }
          delete notification;
        }));
    notification->show();
  }
}

} // web

} // master

} // tvirt

Wt::WApplication *createApplication(const Wt::WEnvironment& env) {
   return new tvirt::master::web::WebGUI(env);
}

int main(int argc, char **argv) {

  int r = Wt::WRun(argc, argv, &createApplication);
  return r;
}

