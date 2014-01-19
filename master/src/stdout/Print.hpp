#ifndef TVIRT_MASTER_STDOUT_PRINT_H_
#define TVIRT_MASTER_STDOUT_PRINT_H_



#include "../../../proto/Request.pb.h"
#include "../../../proto/Hypervisor.pb.h"
#include "../../../utils/Return.hpp"


namespace tvirt {

namespace master {

namespace stdout {

void printGuest (const tvirt::Guest &guest);
void printHypervisor(const tvirt::Hypervisor &hypervisor);
void printCPU(const tvirt::CPU &cpu);
void printMemory (const tvirt::Memory &memory);
void printMonitoringState (const tvirt::MonitoringState &monitoringState);
void printBody (const tvirt::Reply &reply,
                const std::string  &buffer);
const Return<std::string> printTypeName (const tvirt::Type & type);
void printReply(const tvirt::Reply &reply);


} // stdout

} // master

} // tvirt

#endif /* TVIRT_MASTER_STDOUT */
