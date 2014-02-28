#ifndef TVIRT_DAEMON_DAEMON_HPP_
#define TVIRT_DAEMON_DAEMON_HPP_


#include <string>

#include "libvirt/Virt.hpp"
#include "../comm/ServerZmq.hpp"
#include "../proto/Request.pb.h"

#include "MonitoringStateListener.hpp"

namespace tvirt {
namespace daemon {

class Daemon : public   MonitoringStateListener{

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

  void monitoringStateEvent(const proto::MonitoringState &);

public:
  Daemon(const std::string &address, 
         const uint16_t portPublisher,
         const uint16_t portRequest);

  void handlerRequest();

};


} //daemon
} //tvirt


#endif /* TVIRT_DAEMON */










