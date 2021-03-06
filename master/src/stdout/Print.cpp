#include <iostream>
#include "Print.hpp"

namespace tvirt {

namespace daemon {

namespace stdout {

void printGuest (const tvirt::Guest &guest){
    std::cout << "Guest: "
              << ((guest.active())?"ON  ":"OFF ")
              << guest.host().name() 
              << " Uniq ID: " 
              << guest.id()
              << std::endl;
}

void printHypervisor(const tvirt::Hypervisor &hypervisor){

  std::cout << "Hypervisor" << std::endl;
  std::cout << "type      " << hypervisor.type()        << std::endl;
  std::cout << "host name " << hypervisor.host().name() << std::endl;
    
  for (int i = 0 , e = hypervisor.guests_size(); i < e ; i++){
    printGuest(hypervisor.guests(i));
  }
  std::cout << "==================================================" << std::endl;

}

void printCPU(const tvirt::CPU &cpu){
  std::cout << "CPU nb " 
            << cpu.nbcpu()
            << " " 
            << cpu.time()
            << std::endl;
}

void printMemory (const tvirt::Memory &memory){
  std::cout << "Memory " 
            << memory.used() 
            << "/"
            << memory.total() 
            << std::endl;
}

void printMonitoringState (const tvirt::MonitoringState &monitoringState) {
  std::cout << "Monitoring state" << std::endl;
  printCPU (monitoringState.cpu());
 
  printMemory(monitoringState.memory());
  std::cout << "IO " << monitoringState.io() << std::endl;
}

void printBody (const proto::Reply &reply,
                 const std::string  &buffer){

  switch (reply.type()) {
  case proto::DOMAIN_DESTROY:
  case proto::DOMAIN_REBOOT:
  case proto::DOMAIN_START:
    // nothing to do
    break;
  case proto::DOMAIN_LIST:
    {tvirt::Hypervisor hyper;
    hyper.ParseFromString(buffer);
    printHypervisor(hyper);}
    break;
  case proto::DOMAIN_GET_STATE:
    {tvirt::MonitoringState monitoringState;
    monitoringState.ParseFromString(buffer);
    printMonitoringState(monitoringState);}
    break;

  default:
    std::cerr << "Unknown type" << std::endl;

  }
}

const Return<std::string> printTypeName (const proto::Type & type){
  switch (type) {
  case proto::DOMAIN_DESTROY:
    return Return<std::string> (true, "destroy domain");
  case proto::DOMAIN_REBOOT:
    return Return<std::string> (true, "reboot domain");
  case proto::DOMAIN_START:
    return Return<std::string> (true, "start domain");
  case proto::DOMAIN_LIST:
    return Return<std::string> (true, "list domains");
  case proto::DOMAIN_GET_STATE:
    return Return<std::string> (true, "get domain state");
  default:
    return Return<std::string> (false, "Unknown");
  }
}

void printReply(const proto::Reply &reply) {
  const Return<std::string> r = printTypeName(reply.type());
  std::cout << ((reply.success())?"OK   ":"FAIL ") 
            << r.data
            << std::endl;

  if (reply.has_reason()){
    std::cout << "    " << reply.reason() << std::endl;
  }
}


} // stdout

} // daemon

} // tvirt
