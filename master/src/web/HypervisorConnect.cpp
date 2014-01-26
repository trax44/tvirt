#include "HypervisorConnect.hpp"


namespace tvirt {

namespace master {

namespace web {


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


} // web

} // master

} // tvirt
