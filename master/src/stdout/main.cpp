#include <iostream>

#include <stdlib.h>
#include <pthread.h>

#include "../../../comm/ClientZmq.hpp"
#include "../../../proto/Request.pb.h"

#include "../../../proto/Hypervisor.pb.h"



Return<tvirt::proto::Reply> executeCommand (tvirt::comm::ClientZmq &requester,
                                     const tvirt::proto::Request   &request,
                                     std::string *replyBuffer){
  
  tvirt::proto::Reply replyHeader;
  std::string buffer;
  request.SerializeToString(&buffer);
  
  
  requester.send(buffer);

  std::string data;
  Return<int> ret = requester.recv(&data);
  
  if (ret.data == -1){
    std::cerr << "receive failed " << data << std::endl;
    return false;
  } 
  
  replyHeader.ParseFromString(data);
  
  if (ret.success){
    Return<int> retBody = requester.recv(replyBuffer);
    if (retBody.data == -1 || retBody.success){
      std::cout << "Body " << retBody.data << ":" << retBody.success << std::endl;
      return Return<tvirt::proto::Reply>(false, replyHeader);
    }
  } 

  return Return<tvirt::proto::Reply>(true, replyHeader);
}

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

void printBody (const tvirt::proto::Reply &reply,
                 const std::string  &buffer){

  switch (reply.type()) {
  case tvirt::proto::DOMAIN_DESTROY:
  case tvirt::proto::DOMAIN_REBOOT:
  case tvirt::proto::DOMAIN_START:
    // nothing to do
    break;
  case tvirt::proto::DOMAIN_LIST:
    {tvirt::Hypervisor hyper;
    hyper.ParseFromString(buffer);
    printHypervisor(hyper);}
    break;
  case tvirt::proto::DOMAIN_GET_STATE:
    {tvirt::MonitoringState monitoringState;
    monitoringState.ParseFromString(buffer);
    printMonitoringState(monitoringState);}
    break;

  default:
    std::cerr << "Unknown type" << std::endl;

  }
}

const Return<std::string> printTypeName (const tvirt::proto::Type & type){
  switch (type) {
  case tvirt::proto::DOMAIN_DESTROY:
    return Return<std::string> (true, "destroy domain");
  case tvirt::proto::DOMAIN_REBOOT:
    return Return<std::string> (true, "reboot domain");
  case tvirt::proto::DOMAIN_START:
    return Return<std::string> (true, "start domain");
  case tvirt::proto::DOMAIN_LIST:
    return Return<std::string> (true, "list domains");
  case tvirt::proto::DOMAIN_GET_STATE:
    return Return<std::string> (true, "get domain state");
  default:
    return Return<std::string> (false, "Unknown");
  }
}

void printReply(const tvirt::proto::Reply &reply) {
  const Return<std::string> r = printTypeName(reply.type());
  std::cout << ((reply.success())?"OK   ":"FAIL ") 
            << r.data
            << std::endl;

  if (reply.has_reason()){
    std::cout << "    " << reply.reason() << std::endl;
  }
}

void *sub (void *socket_){
  std::cout << "welcome to sub" << std::endl;
  tvirt::comm::ClientZmq *socket = static_cast <tvirt::comm::ClientZmq *> (socket_);

  bool ok = true;
  std::string replyBuffer;
  std::string bodyBuffer;

  tvirt::proto::Reply reply;
  while (ok){
    Return<int> r = socket->recv(&replyBuffer);
    reply.ParseFromString(replyBuffer);
    printReply(reply);
    
    while (r.data > 0 && r.success) {
      r = socket->recv(&bodyBuffer);
      if (r.data > 0) {
        printBody(reply, bodyBuffer);
      }
    }
  }
}



int main(int argc, char *argv[]) {
  
  if (argc != 4) {
    std::cout << "Usage: " 
              << argv[0] 
              << "bind request-port sub-port (e.g. tcp://* 5555 5556)" 
              << std::endl;
    return -1;
  }
  tvirt::comm::ClientZmq requester(argv[1], atoi(argv[2]), ZMQ_REQ);
  tvirt::comm::ClientZmq n(argv[1], atoi(argv[3]), ZMQ_SUB);

  pthread_t subThread;
  pthread_create(&subThread, NULL, sub, &n);

  std::string buffer;
  tvirt::proto::Request request;
  
  // request domain list 
  request.set_type(tvirt::proto::DOMAIN_LIST);
  Return<tvirt::proto::Reply> ret = executeCommand(requester, request, &buffer);
  if (!ret.success){
    return -1;
  }
  
  // handling result 
  tvirt::Hypervisor hypervisor;
  if (!ret.data.success() || !hypervisor.ParseFromString(buffer)){
    std::cerr << "Failed to parse received data" << std::endl;
    return -1;
  }

  printHypervisor(hypervisor);
    

  // start all the inactive domains
  for (int i = 0 , e = hypervisor.guests_size(); i < e ; i++){
    if (!hypervisor.guests(i).active()){
      buffer.clear();
      request.set_type(tvirt::proto::DOMAIN_START);
      request.set_domainid(hypervisor.guests(i).id());
      executeCommand(requester, request, &buffer);
    }
  }




  if (hypervisor.guests_size() > 0){
    std::cout << "Asking for guest info" << std::endl;
    // request guest monitoring state 
    request.set_type(tvirt::proto::DOMAIN_GET_STATE);
    request.set_domainid(hypervisor.guests(0).id());
    Return<tvirt::proto::Reply> ret = executeCommand(requester, request, &buffer);
    if (!ret.success){
      std::cout << "Failed to retrieve information" << std::endl;
      return -1;
    }
  
    // handling result 
    tvirt::MonitoringState monitoringState;
    if (!ret.data.success() || !monitoringState.ParseFromString(buffer)){
      std::cerr << "Failed to parse received data" << std::endl;
      return -1;
    }

    printMonitoringState(monitoringState);
  }


  return 0;
}
