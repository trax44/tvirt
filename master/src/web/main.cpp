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
#include <Wt/WBootstrapTheme>
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WNavigationBar>
#include <Wt/WMenu>
#include <Wt/WStackedWidget>
#include "../Requester.hpp"
#include "Hypervisors.hpp"
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
  Wt::WStackedWidget *menuStack;  
  void request();

  Wt::WVBoxLayout *globalLayout;

  void handleGuestAction (const ViewControler::HypervisorID hypervisorID, 
                          const uint64_t guestID, 
                          const proto::RequestType action);


  Wt::WNavigationBar * createNavigationBar(Wt::WStackedWidget *stack, Wt::WContainerWidget *parent=0);
  

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


  auto hypervisors = new Hypervisors(controler, root());

  leftMenu->addItem("Hypervisors", hypervisors);



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
  
  menuStack  = new Wt::WStackedWidget(root()); 
  auto navBar     = createNavigationBar(menuStack, root());
  globalLayout->addWidget(navBar,0);
  globalLayout->addWidget(menuStack,0);

  globalLayout->addStretch(1);

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

