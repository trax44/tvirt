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
#include <Wt/WNavigationBar>
#include <Wt/WMenu>
#include <Wt/WStackedWidget>

#include "../Requester.hpp"
#include "HypervisorConnect.hpp"
#include "Hypervisor.hpp"
#include "../ViewControler.hpp"

namespace tvirt {

namespace master  {

namespace web {



class WebGUI : public Wt::WApplication  {
public:
  WebGUI (const Wt::WEnvironment& env);
  ~WebGUI(){std::cout << "life is cruel" << std::endl;}

private:
  ViewControler controler;
  
  void request();
  std::list<Requester> hypervisorsConnection;
  HypervisorConnect *hypervisorConnection;

  Wt::WVBoxLayout *globalLayout;

  void handleGuestAction (const ViewControler::ConnectionID connectionID, 
                          const uint64_t guestID, 
                          const proto::Type action);


  Wt::WNavigationBar * createNavigationBar(Wt::WStackedWidget *stack, Wt::WContainerWidget *parent=0);
  void addHypervisor(const std::string address, uint16_t port);
  void removeHypervisorDialog();
  void askForHypervisorConnection();
};

Wt::WWidget* createNotification(const std::string & text){

  Wt::WTemplate *t = new Wt::WTemplate(Wt::WString::tr(text));
  return t;
}


Wt::WNavigationBar * WebGUI::createNavigationBar(Wt::WStackedWidget *stack, Wt::WContainerWidget *parent) {
  Wt::WNavigationBar * navigation = new Wt::WNavigationBar(parent);
  navigation->setTitle("Tvirt");
  navigation->setResponsive(true);

  Wt::WMenu *leftMenu = new Wt::WMenu(stack, parent);
  leftMenu->addItem("Home", new Wt::WText ("Welcome home"));
  leftMenu->addItem("Hypervisors", new Wt::WText("Managing hypervisor"))->triggered().connect(this, &WebGUI::askForHypervisorConnection);

  auto notification = new Wt::WText("trax Omar Giveranud o.givernaud@gmail.com");
  notification->setStyleClass("alert alert-info");

  leftMenu->addItem("About", notification);
  
  navigation->addMenu(leftMenu);

  Wt::WLineEdit *edit = new Wt::WLineEdit();
  navigation->addSearch(edit, Wt::AlignRight);

  return navigation;
}

WebGUI::WebGUI(const Wt::WEnvironment& env)
  : Wt::WApplication(env),
    controler() {

  setTitle("Enyx Cloud Administration Tool" __TIME__);



  useStyleSheet("web/css.css");
  globalLayout = new Wt::WVBoxLayout();
  root()->setLayout(globalLayout);
  //askForHypervisorConnection();
  setTheme(new Wt::WBootstrapTheme());
  //setCssTheme("polished");

  root()->setStyleClass("row-fluid");
  
  auto menuStack  = new Wt::WStackedWidget(root()); 
  auto navBar     = createNavigationBar(menuStack, root());
  globalLayout->addWidget(navBar,0);
  globalLayout->addWidget(menuStack,0);

  globalLayout->addStretch(1);

}

void WebGUI::removeHypervisorDialog(){
  std::cout << "hyp res " << hypervisorConnection->result() << std::endl;

  //delete hypervisorConnection;
  hypervisorConnection = NULL;
}

void WebGUI::askForHypervisorConnection(){
  hypervisorConnection = new HypervisorConnect();
  hypervisorConnection->res().connect(this, &WebGUI::addHypervisor);
  hypervisorConnection->finished().connect(this, &WebGUI::removeHypervisorDialog);
  
}

void WebGUI::handleGuestAction (const ViewControler::ConnectionID connectionID, 
                                const uint64_t guestID, 
                                const proto::Type action){

  controler.doActionOnGuest(connectionID, guestID, action);
}

void WebGUI::addHypervisor(const std::string address, uint16_t port) {
  std::cout << "checking hypervision connection information" << std::endl;
  Return<ViewControler::ConnectionID> connection = controler.addConnection(address, port);
  
  if (connection.success) {
    auto r = controler.connectToHypervisor(connection.data);
    if (!r.success){
      return;
    }
    
    web::Hypervisor *hypervisor = new web::Hypervisor (connection.data, r.data);
    hypervisor->action().connect(this, &WebGUI::handleGuestAction);
    globalLayout->insertWidget(globalLayout->count() - 1, hypervisor, 0);
    
    
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

