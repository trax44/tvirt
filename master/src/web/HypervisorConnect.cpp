#include "HypervisorConnect.hpp"
#include <stdlib.h>
#include <iostream>
namespace tvirt {

namespace master {

namespace web {


HypervisorConnect::HypervisorConnect():Wt::WDialog("Connection"),
                                       validator(1,65535),
                                       done_(this){

  setStyleClass("dialog");
  wtAddress = new Wt::WLineEdit(); wtAddress->setId("address");
  wtPort    = new Wt::WLineEdit(); wtPort->setId("port");


  Wt::WPushButton *ok = new Wt::WPushButton("OK", footer());
  ok->setId("OK");
  ok->setDefault(true);

  Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", footer());
  cancel->setId("CANCEL");
  rejectWhenEscapePressed();
  
  ok->clicked().connect(this, &HypervisorConnect::validate);

  Wt::WVBoxLayout *vbox = new Wt::WVBoxLayout(this->contents());
  vbox->addWidget(new Wt::WText("Address"));
  vbox->addWidget(wtAddress);
  vbox->addWidget(new Wt::WText("Port"));
  vbox->addWidget(wtPort);
  
  validator.setMandatory(true);
  wtPort->setValidator(&validator);

  setOffsets(0, Wt::Top);
  animateShow
    (Wt::WAnimation(Wt::WAnimation::SlideInFromTop
                | Wt::WAnimation::Fade, Wt::WAnimation::Linear, 250));



  show();
}

void HypervisorConnect::validate() {
  
  std::cout << "sending signal" << std::endl;
  done_.emit(wtAddress->text().toUTF8(), atoi(wtPort->text().toUTF8().c_str()));

  accept();
}




} // web

} // master

} // tvirt

