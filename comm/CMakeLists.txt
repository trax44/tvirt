find_package(PkgConfig)



pkg_check_modules (ZMQ libzmq)



add_library(commZmq
Client.cpp
Client.hpp
ClientZmq.cpp
ClientZmq.hpp
Server.cpp
Server.hpp
ServerZmq.cpp
ServerZmq.hpp
)


include_directories(${ZMQ_CFLAGS})
target_link_libraries(commZmq ${ZMQ_LIBRARIES})


