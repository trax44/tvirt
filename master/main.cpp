#include <iostream>

#include <stdlib.h>

#include "../comm/ServerZmq.hpp"
#include "../proto/Hypervisor.pb.h"


int main(int argc, char *argv[]) {
  
  if (argc != 3) {
    std::cout << "Usage: " 
              << argv[0] 
              << "bind port (e.g. tcp://* 5555)" 
              << std::endl;
    return -1;
  }

  tvirt::comm::ServerZmq receiver(argv[1], atoi(argv[2]), ZMQ_SUB);
  receiver.setFilter("", 0);

  Return<std::string> ret(false);
  int n = receiver.recv(&ret);

  if (!ret.success){
    std::cerr << "receive failed " << ret.data << std::endl;
    return -1;
  } 


  tvirt::Hypervisor hypervisor;
  if (!hypervisor.ParseFromString(ret.data)){
    std::cerr << "Failed to parse received data" << std::endl;
    return -1;
  }
  
  

  std::cout << "type      " << hypervisor.type()        << std::endl;
  std::cout << "host name " << hypervisor.host().name() << std::endl;
  
  for (int i = 0 , e = hypervisor.guests_size(); i < e ; i++){
    std::cout << "  >" 
              << hypervisor.guests(i).id()
              << ":" 
              << hypervisor.guests(i).host().name() << std::endl;

  }

  



  return 0;
}











