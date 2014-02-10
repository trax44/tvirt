#ifndef TVIRT_MASTER_REQUESTER_H_
#define TVIRT_MASTER_REQUESTER_H_

#include <stdint.h>
#include <string>

#include "../../utils/Any.hpp"
#include "../../utils/Return.hpp"

#include "../../comm/ClientZmq.hpp"
#include "../../proto/Request.pb.h"


namespace tvirt {

namespace master {

class Requester {
private:
  comm::ClientZmq socket;
  tvirt::Reply replyHeader;
  std::string buffer;

public:
  Requester(const std::string &address,
            const uint16_t port);


  Return<Reply> executeCommand (const tvirt::Request &request,
                                std::string *replyBuffer);
  
  

  /**
   * \fn const Return<const Any> getReplyHeader()
   * \return Return.data is a tvirt::Reply pointer object on success
   */
  const Return<const Any> getReplyHeader();

  /**
   * \fn const Return<const Any> getReplyBody()
   * \return Return.data a std::string pointer on success
   */
  const Return<const Any> getReplyBody();  
  
};

} // master

} // tvirt

#endif /* TVIRT_MASTER */
