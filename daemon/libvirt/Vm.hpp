#ifndef TVIRT_DAEMON_VM_HPP_
#define TVIRT_DAEMON_VM_HPP_

#include <stdint.h>
#include <string>


class Vm{
public:
  typedef uint32_t ID;
  

private:
  const ID id;
  const std::string name;

public:

  Vm (ID id, const std::string &name): 
    id   (id), 
    name (name){}
  

  const std::string &getName()const{
    return name;
  }

};


#endif /* TVIRT_DAEMON */










