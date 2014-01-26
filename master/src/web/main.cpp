#include <list>

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>

#include <Wt/WIconPair>
#include <Wt/WText>
#include <Wt/WTree>
#include <Wt/WTreeNode>

#include "../Requester.hpp"


namespace tvirt {

namespace master  {

namespace web {

class HypervisorConnect : public Wt::WContainerWidget {
private:
  Wt::WLineEdit     wtAddress;
  Wt::WLineEdit     wtPort;
  void connect();
  
public:
  HypervisorConnect(Wt::WContainerWidget *parent);
};

HypervisorConnect::HypervisorConnect(Wt::WContainerWidget *parent) {

  Wt::WPushButton *wtSubmit = new Wt::WPushButton("Conenct");
  Wt::WVBoxLayout *vbox = new Wt::WVBoxLayout();
  vbox->addWidget(new Wt::WText("Address"));
  vbox->addWidget(&wtAddress);
  vbox->addWidget(new Wt::WText("Port"));
  vbox->addWidget(&wtPort);
  vbox->addWidget(wtSubmit);

  wtSubmit->clicked().connect(this, &HypervisorConnect::connect);
  
  this->setLayout(vbox);
}

void HypervisorConnect::connect() {
  
}



class MainApp : public Wt::WApplication {
public:
  MainApp(const Wt::WEnvironment& env);
  
  
private:
  std::list<Requester> hypervisorsConnection;
};




MainApp::MainApp(const Wt::WEnvironment& env)
  : Wt::WApplication(env) {
  setTitle("Enyx Cloud Administration Tool");
  
  
  root()->addWidget(new HypervisorConnect(root()));
}


} // web

} // master

} // tvirt


Wt::WApplication *createApplication(const Wt::WEnvironment& env) {
  return new tvirt::master::web::MainApp(env);
}


int main(int argc, char **argv) {
  return Wt::WRun(argc, argv, &createApplication);
}

