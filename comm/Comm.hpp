#ifndef TVIRT_COMM_COMM_HPP_
#define TVIRT_COMM_COMM_HPP_

#include <string>
#include "../utils/Return.hpp"


namespace tvirt {

namespace comm {

class Comm {
public:
  virtual int recv(Return<std::string> *) = 0;
  virtual Return<void>send(const std::string &message) = 0;
  virtual Return<std::string>recv() = 0;
};


} //comm
} //tvirt

#endif /* TVIRT_COMM */










