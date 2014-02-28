#ifndef TVIRT_DAEMON_MONITORINGSTATELISTENER_H_
#define TVIRT_DAEMON_MONITORINGSTATELISTENER_H_

namespace tvirt {

namespace daemon {

class MonitoringStateListener {
public:
  virtual void monitoringStateEvent(const proto::MonitoringState &) = 0;
};


} // daemon

} // tvirt
#endif /* TVIRT_DAEMON */
