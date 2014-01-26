#ifndef RME_ANY_H_
#define RME_ANY_H_

#include <typeinfo>
#include <string>

namespace tvirt {



class Any {
private:
  const void *ptr;
  const std::type_info &type;

public:
  Any():
    ptr (NULL),
    type (typeid(ptr)){}

  template<typename ValueType>
  Any(ValueType *value) : 
    ptr(reinterpret_cast<void*> (value)),
    type (typeid(*value)){}
  
  const std::string getTypeName() const{
    return type.name();
  }
  const std::type_info& getType() const{
    return type;
  }

  const void *getPtr() const{
    return ptr;
  }
};


} // tvirt



#endif /* RME */
