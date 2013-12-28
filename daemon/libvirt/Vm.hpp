#ifndef TVIRT_DAEMON_VM_HPP_
#define TVIRT_DAEMON_VM_HPP_

#include <stdint.h>
#include <string>


class Vm{
public:
  typedef uint32_t ID;
  

private:
  const std::string name;
  const ID id;

public:

  Vm (ID id, const std::string &name): 
    id   (id), 
    name (name){}

};


#endif /* TVIRT_DAEMON */
