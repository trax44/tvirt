#include "HypervisorConnect.hpp"
#include <boost/bind.hpp>
#include <stdlib.h>
#include <iostream>
namespace tvirt {

namespace master {

namespace web {


HypervisorConnect::HypervisorConnect():Wt::WDialog("Connection"),
                                       validator(1,65535),
                                       done_(this){

  setStyleClass("dialog");
    
  setModal(true);
  
  Wt::WPushButton *ok = new Wt::WPushButton("OK", footer());
  ok->setDefault(true);

  Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", footer());
  rejectWhenEscapePressed();
  
  ok->clicked().connect(this, &HypervisorConnect::validate);

  Wt::WVBoxLayout *vbox = new Wt::WVBoxLayout(this->contents());
  vbox->addWidget(new Wt::WText("Address"));
  vbox->addWidget(&wtAddress);
  vbox->addWidget(new Wt::WText("Port"));
  vbox->addWidget(&wtPort);
  
  validator.setMandatory(true);
  wtPort.setValidator(&validator);
  
  show();
}

void HypervisorConnect::validate() {
  
  std::cout << "sending signal" << std::endl;
  done_.emit(wtAddress.text().toUTF8(), atoi(wtPort.text().toUTF8().c_str()));

  accept();
}




} // web

} // master

} // tvirt

