#ifndef TVIRT_MASTER_STDOUT_PRINT_H_
#define TVIRT_MASTER_STDOUT_PRINT_H_



#include "../../../proto/Request.pb.h"
#include "../../../proto/Hypervisor.pb.h"
#include "../../../utils/Return.hpp"


namespace tvirt {

namespace master {

namespace stdout {

void printGuest (const Guest &guest);
void printHypervisor(const Hypervisor &hypervisor);
void printCPU(const CPU &cpu);
void printMemory (const Memory &memory);
void printMonitoringState (const MonitoringState &monitoringState);
void printBody (const proto::Reply &reply,
                const std::string  &buffer);
const Return<std::string> printTypeName (const proto::Type & type);
void printReply(const proto::Reply &reply);


} // stdout

} // master

} // tvirt

#endif /* TVIRT_MASTER_STDOUT */
