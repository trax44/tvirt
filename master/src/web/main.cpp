#include <list>

#include <Wt/WApplication>
// #include <Wt/WBreak>

// #include <Wt/WIconPair>
// #include <Wt/WText>
// #include <Wt/WTree>
// #include <Wt/WTreeNode>

#include "../Requester.hpp"
#include "HypervisorConnect.hpp"

namespace tvirt {

namespace master  {

namespace web {



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

