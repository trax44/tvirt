cmake_minimum_required (VERSION 2.8)
project (tvirt)




add_executable(tvirtd
./daemon/main.cpp
./daemon/libvirt/Virt.cpp
./daemon/libvirt/Virt.hpp
./daemon/libvirt/Vm.hpp
)

target_link_libraries (tvirtd -lvirt -ldl)

add_subdirectory (proto)
