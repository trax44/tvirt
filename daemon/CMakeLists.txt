find_package(PkgConfig)

add_executable(tvirtd
main.cpp
libvirt/Virt.cpp
libvirt/Virt.hpp
libvirt/Vm.hpp
)

pkg_check_modules (VIRT libvirt)
target_link_libraries (tvirtd ${VIRT_LIBRARIES} commZmq proto)
