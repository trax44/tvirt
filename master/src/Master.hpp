#ifndef TVIRT_MASTER_WEB_MASTER_H_
#define TVIRT_MASTER_WEB_MASTER_H_


namespace tvirt {

namespace master {

class Master: public MasterModel {
public:
  typedef uint16_t ConnectionID;
  
private:
  enum ConnectionState{
    UP, DOWN
  };

  struct HypervisorConnection {
    HypervisorConnection (const ConnectionID id,
                          const std::string address,
                          const uint16_t port):
      id(id),
      address(address),
      port(port){}


    const ConnectionID id;
    const std::string address;
    const uint16_t port;
    ConnectionState state;
  };

  typedef std::list<HypervisorConnection> Connections;

  Connections hypervisorConnections;

public:
  Master();

  Return<ConnectionID> addConnection    (const std::string &, uint16_t port);
  Return<void>         removeConnection (const std::string &, uint16_t port);
  Return<void>         removeConnection (const ConnectionID);
  Return<void>         connect          (const ConnectID);
  Return<void>         connectAll       ();

  ~Master();
};

} // master

} // tvirt

#endif /* TVIRT_MASTER_WEB */
