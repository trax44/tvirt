#ifndef TVIRT_DAEMON_DAEMON_HPP_
#define TVIRT_DAEMON_DAEMON_HPP_


#include <string>

#include "libvirt/Virt.hpp"
#include "../comm/ServerZmq.hpp"
#include "../proto/Request.pb.h"


namespace tvirt {
namespace daemon {

class Daemon {
private:
  const std::string address;
  const uint16_t portPublisher;
  const uint16_t portRequest;
  tvirt::daemon::Virt virt;
  tvirt::comm::ServerZmq publisher;  
  tvirt::comm::ServerZmq replyer;  
  std::string uuid;

  Return<void> execute(const proto::Request &request,
                       std::string *ret);
public:
  Daemon(const std::string &address, 
         const uint16_t portPublisher,
         const uint16_t portRequest);

  void handlerRequest();

};


} //daemon
} //tvirt


#endif /* TVIRT_DAEMON */










