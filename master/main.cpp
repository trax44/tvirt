#include <iostream>

#include <stdlib.h>
#include <pthread.h>

#include "../comm/ClientZmq.hpp"
#include "../proto/Request.pb.h"

#include "../proto/Hypervisor.pb.h"



Return<tvirt::Reply> executeCommand (tvirt::comm::ClientZmq &requester,
                                     const tvirt::Request   &request,
                                     std::string *replyBuffer){
  
  tvirt::Reply replyHeader;
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
      return Return<tvirt::Reply>(false, replyHeader);
    }
  } 

  return Return<tvirt::Reply>(true, replyHeader);
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
  std::cout << "CPU " 
            << cpu.id() 
            << cpu.level() 
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
  for (int i = 0 , e = monitoringState.cpus_size(); i < e ; i++){
    printCPU (monitoringState.cpus(i));
  }
 
  printMemory(monitoringState.memory());
  std::cout << "IO " << monitoringState.io() << std::endl;

}

void printBody (const tvirt::Reply &reply,
                 const std::string  &buffer){

  switch (reply.type()) {
  case tvirt::DOMAIN_DESTROY:
  case tvirt::DOMAIN_REBOOT:
  case tvirt::DOMAIN_START:
    // nothing to do
    break;
  case tvirt::DOMAIN_LIST:
    {tvirt::Hypervisor hyper;
    hyper.ParseFromString(buffer);
    printHypervisor(hyper);}
    break;
  case tvirt::DOMAIN_GET_STATE:
    {tvirt::MonitoringState monitoringState;
    monitoringState.ParseFromString(buffer);
    printMonitoringState(monitoringState);}
    break;

  default:
    std::cerr << "Unknown type" << std::endl;

  }
}

const Return<std::string> printTypeName (const tvirt::Type & type){
  switch (type) {
  case tvirt::DOMAIN_DESTROY:
    return Return<std::string> (true, "destroy domain");
  case tvirt::DOMAIN_REBOOT:
    return Return<std::string> (true, "reboot domain");
  case tvirt::DOMAIN_START:
    return Return<std::string> (true, "start domain");
  case tvirt::DOMAIN_LIST:
    return Return<std::string> (true, "list domains");
  case tvirt::DOMAIN_GET_STATE:
    return Return<std::string> (true, "get domain state");
  default:
    return Return<std::string> (false, "Unknown");
  }
}

void printReply(const tvirt::Reply &reply) {
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

  tvirt::Reply reply;
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
  tvirt::Request request;
  request.set_type(tvirt::DOMAIN_LIST);

  Return<tvirt::Reply> ret = executeCommand(requester, request, &buffer);
  if (!ret.success){
    return -1;
  }
  
  tvirt::Hypervisor hypervisor;
  if (!ret.data.success() || !hypervisor.ParseFromString(buffer)){
    std::cerr << "Failed to parse received data" << std::endl;
    return -1;
  }


  printHypervisor(hypervisor);
    

  for (int i = 0 , e = hypervisor.guests_size(); i < e ; i++){
    if (!hypervisor.guests(i).active()){
      buffer.clear();
      request.set_type(tvirt::DOMAIN_START);
      request.set_domainid(hypervisor.guests(i).id());
      executeCommand(requester, request, &buffer);
    }

  }

  return 0;
}
