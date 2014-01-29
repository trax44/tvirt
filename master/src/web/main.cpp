#include <list>

#include <pthread.h>
#include <unistd.h>

#include <Wt/WApplication>
#include <Wt/WLayout>
#include <Wt/WDialog>
#include <Wt/WServer>
#include <Wt/WMessageBox>

#include "../Requester.hpp"
#include "HypervisorConnect.hpp"
#include "../ViewModel.hpp"
#include "../ViewControler.hpp"

#include <iostream>
#include <stdio.h>
#include <string>
namespace tvirt {

namespace master  {

namespace web {



class WebGUI : public Wt::WApplication ,
               public ViewModel {
public:
  WebGUI (const Wt::WEnvironment& env);
  Return<std::pair<std::string, uint16_t> > askForHypervisorConnection();

private:
  ViewControler controler;
  
  void run();
  void request();
  std::list<Requester> hypervisorsConnection;
  HypervisorConnect *hypervisorConnection;

  void addHypervisor(const std::string address, uint16_t port);
  void addHypervisor2();
};


void WebGUI::run(){
  controler.run();
}

WebGUI::WebGUI(const Wt::WEnvironment& env)
  : Wt::WApplication(env),
    controler(this)
{
  setTitle("Enyx Cloud Administration Tool");
  useStyleSheet("web/css.css");

  Wt::WPushButton *run = new Wt::WPushButton("run");
  run->clicked().connect(this, &WebGUI::run);

  // root()->addWidget((hypervisorConnection = new HypervisorConnect()));
  // hypervisorConnection->res().connect(this, &WebGUI::addHypervisor);
  // hypervisorConnection->finished().connect(this, &WebGUI::addHypervisor2);
}


void WebGUI::addHypervisor2(){
  std::cout << "deleting " << ((hypervisorConnection->result() == Wt::WDialog::Accepted)?"true":"false")<< std::endl;
  delete hypervisorConnection;
}
void WebGUI::addHypervisor(const std::string address, uint16_t port){
  std::cout << "checking hypervision connection information" << std::endl;
  Return<ViewControler::ConnectionID> connection = controler.addConnection(address, port);
  
  Wt::WMessageBox *answer;
  if (connection.success){
    answer = new Wt::WMessageBox(":)", "Connecting" , Wt::Information, Wt::Ok);
  } else {
    answer = new Wt::WMessageBox("FAIL", "Fail to connect to hypervisor" , Wt::Critical, Wt::Ok);
  }
  answer->show();
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

