
#include "Hypervisor.hpp"
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WIconPair>
#include <Wt/WPushButton>
#include <Wt/WPopupMenu>
#include <Wt/WMenuItem>
namespace tvirt {

namespace master {

namespace web {


Hypervisor::Hypervisor(const ViewControler::HypervisorConnection *hypervisor):
  hypervisor(hypervisor) {
    
  setTitle(hypervisor->address);
  setCollapsible(true);


  Wt::WTable *table = new Wt::WTable();
  setCentralWidget(table);
  
  table->addStyleClass("table table-bordered");
  table->addStyleClass("table table-hover");

  table->setHeaderCount(1);
  table->elementAt(0, 0)->addWidget(new Wt::WText("State"));
  table->elementAt(0, 1)->addWidget(new Wt::WText("Hostname"));
  table->elementAt(0, 2)->addWidget(new Wt::WText("cpu"));
  table->elementAt(0, 3)->addWidget(new Wt::WText("memory"));
  table->elementAt(0, 4)->addWidget(new Wt::WText("OS"));
  table->elementAt(0, 5)->addWidget(new Wt::WText("Address"));
  table->elementAt(0, 6)->addWidget(new Wt::WText("Action"));

  for (int i = 0 , e = hypervisor->hypervisor.guests_size(); i < e ; i++) {
    if (hypervisor->hypervisor.guests(i).active()){
      table->elementAt(i+1, 0)->addWidget(new Wt::WIconPair("network-idle.png", "network-error.png", false));
    } else {
      table->elementAt(i+1, 0)->addWidget(new Wt::WIconPair("network-error.png", "network-idle.png", false));
    }
    table->elementAt(i+1, 1)->addWidget(new Wt::WText(hypervisor->hypervisor.guests(i).host().name()));

    if (hypervisor->hypervisor.guests(i).host().has_cpu()){
      table->elementAt(i+1, 2)->addWidget(new Wt::WText(std::to_string(hypervisor->hypervisor.guests(i).host().cpu().nbcpu())));
    } else {
      table->elementAt(i+1, 2)->addWidget(new Wt::WText("Unknonw"));
    }

    if (hypervisor->hypervisor.guests(i).host().has_memory()){
      table->elementAt(i+1, 3)->addWidget(new Wt::WText(std::to_string(hypervisor->hypervisor.guests(i).host().memory())));
    } else {
      table->elementAt(i+1, 3)->addWidget(new Wt::WText("Unknonw"));
    }

    if (hypervisor->hypervisor.guests(i).host().has_os()){
      table->elementAt(i+1, 4)->addWidget(new Wt::WText(osName(hypervisor->hypervisor.guests(i).host().os())));
    } else {
      table->elementAt(i+1, 4)->addWidget(new Wt::WText("Unknonw"));
    }

    if (hypervisor->hypervisor.guests(i).host().has_address()){
      table->elementAt(i+1, 5)->addWidget
        (new Wt::WText(hypervisor->hypervisor.guests(i).host().address().address()));
    } else {
      table->elementAt(i+1, 5)->addWidget(new Wt::WText("Unknonw"));
    }

    Wt::WPopupMenu *popup = new Wt::WPopupMenu();

    popup->addItem("Start")->
      triggered().connect(std::bind (&Hypervisor::emitAction, 
                                     this,
                                     hypervisor->id,
                                     hypervisor->hypervisor.guests(i).id(), 
                                     proto::RequestType::DOMAIN_START));
    popup->addItem("Halt")->
      triggered().connect(std::bind (&Hypervisor::emitAction, 
                                     this,
                                     hypervisor->id,
                                     hypervisor->hypervisor.guests(i).id(), 
                                     proto::RequestType::DOMAIN_DESTROY));
    
    popup->addItem("Reboot")->
      triggered().connect(std::bind (&Hypervisor::emitAction, 
                                     this, 
                                     hypervisor->id,
                                     hypervisor->hypervisor.guests(i).id(), 
                                     proto::RequestType::DOMAIN_REBOOT));

    popup->addItem("Destroy")->
      triggered().connect(std::bind (&Hypervisor::emitAction, 
                                     this, 
                                     hypervisor->id,
                                     hypervisor->hypervisor.guests(i).id(), 
                                     proto::RequestType::DOMAIN_DESTROY));
    
    // popup->addItem("Send Comamnd")->
    //   triggered().connect(std::bind (&Hypervisor::emitAction, 
    //                                  this, 
    //                                  hypervisor->id,
    //                                  hypervisor->hypervisor.guests(i).id(), 
    //                                  proto::RequestType::DOMAIN_SEND_COMMAND));
    

    Wt::WPushButton *action = new Wt::WPushButton("Action");
    action->setMenu(popup);
    table->elementAt(i+1, 6)->addWidget(action);

  }
}

void Hypervisor::emitAction (const ViewControler::HypervisorID hypervisorID, 
                             const uint64_t guestID, 
                             const proto::RequestType actionType) {
  done_.emit(hypervisorID, guestID, actionType);
}

const std::string Hypervisor::osName(int i) {
  switch (i){
  case proto::Host::OS::Host_OS_GENTOO :return "GENTOO";
  case proto::Host::OS::Host_OS_DEBIAN :return "DEBIAN"; 
  case proto::Host::OS::Host_OS_CENTOS :return "CENTOS"; 
  case proto::Host::OS::Host_OS_FEDORA :return "FEDORA";
  case proto::Host::OS::Host_OS_UBUNTU :return "UBUNTU"; 
  case proto::Host::OS::Host_OS_ARCH   :return "ARCH"  ; 
  case proto::Host::OS::Host_OS_LINUX  :return "LINUX" ; 
  case proto::Host::OS::Host_OS_BDS    :return "BDS"   ; 
  case proto::Host::OS::Host_OS_OTHER  :return "OTHER" ; 
  case proto::Host::OS::Host_OS_WIND   :return "WIND"  ; 
  }
  
}


} // web

} // master

} // tvirt
