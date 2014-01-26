#ifndef TVIRT_MASTER_WEB_VMS_H_
#define TVIRT_MASTER_WEB_VMS_H_

namespace tvirt {

namespace master {

namespace web {

class Hypervisor  : public Wt::WContainerWidget {
private:
    Wt::WContainerWidget *parent;

public:
  Hypervisor(Wt::WContainerWidget *parent);

  virtual ~Hypervisor();


};

} // web

} // master

} // tvirt

#endif /* TVIRT_MASTER_WEB */
