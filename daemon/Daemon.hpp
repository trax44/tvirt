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
  const uint16_t port;
  tvirt::daemon::Virt virt;
  tvirt::comm::ServerZmq publisher;  
  tvirt::comm::ServerZmq replyer;  

  Return<void> execute(const Request::Type command,
                       std::string *ret);
public:
  Daemon(const std::string &address, const uint16_t port);

  void handlerRequest(const std::string &address, 
                      const uint16_t port);

};


} //daemon
} //tvirt


#endif /* TVIRT_DAEMON */










