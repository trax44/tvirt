add_executable(master-web
main.cpp
HypervisorConnect.cpp
HypervisorConnect.hpp
)


set(WITTY_LIBRARIES -lwt -lwthttp)
target_link_libraries (master-web commZmq proto ${WITTY_LIBRARIES})

