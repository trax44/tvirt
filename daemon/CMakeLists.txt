find_package(PkgConfig)

add_executable(tvirtd
main.cpp
Daemon.cpp
Daemon.hpp
libvirt/Virt.cpp
libvirt/Virt.hpp
)

pkg_check_modules (VIRT libvirt)
pkg_check_modules (UUID uuid)
target_link_libraries (tvirtd ${VIRT_LIBRARIES} ${UUID_LIBRARIES} commZmq proto)
include_directories (${VIRT_INCLUDE_DIRS} ${UUID_INCLUDE_DIRS})
