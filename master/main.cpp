#include <iostream>

#include <stdlib.h>

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

void printHypervisor(const tvirt::Hypervisor &hypervisor){
  std::cout << "type      " << hypervisor.type()        << std::endl;
  std::cout << "host name " << hypervisor.host().name() << std::endl;
    
  for (int i = 0 , e = hypervisor.guests_size(); i < e ; i++){
    std::cout << "  >"
              << ((hypervisor.guests(i).active())?"ON  ":"OFF ")
              << hypervisor.guests(i).host().name() 
              << " UUID: " 
              << hypervisor.guests(i).id()
              << std::endl;

  }
}

int main(int argc, char *argv[]) {
  
  if (argc != 3) {
    std::cout << "Usage: " 
              << argv[0] 
              << "bind port (e.g. tcp://* 5555)" 
              << std::endl;
    return -1;
  }
  tvirt::comm::ClientZmq requester(argv[1], atoi(argv[2]), ZMQ_REQ);

  std::string buffer;
  tvirt::Request request;
  request.set_type(tvirt::Request::DOMAIN_LIST);

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
      request.set_type(tvirt::Request::DOMAIN_START);
      request.set_domainid(hypervisor.guests(i).id());
      executeCommand(requester, request, &buffer);
    }

  }

  return 0;
}
