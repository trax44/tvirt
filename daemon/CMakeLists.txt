find_package(PkgConfig)

add_executable(tvirtd
main.cpp
Daemon.cpp
Daemon.hpp
libvirt/Virt.cpp
libvirt/Virt.hpp
)

pkg_check_modules (VIRT libvirt)
target_link_libraries (tvirtd ${VIRT_LIBRARIES} commZmq proto)
