find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})


include_directories(${CMAKE_CURRENT_BINARY_DIR})

set (PROTO_SRCS 
  Hypervisor.proto
  Request.proto
)

PROTOBUF_GENERATE_CPP(PROTO_SRCS_G PROTO_HDRS_G ${PROTO_SRCS})
add_library(proto ${PROTO_SRCS_G} ${PROTO_HDRS_G})
target_link_libraries(proto ${PROTOBUF_LIBRARIES})

