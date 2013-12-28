cmake_minimum_required (VERSION 2.8)
project (tvirt)


add_executable(daemon 
daemon/libvirt/Virt.cpp
daemon/libvirt/Virt.hpp
daemon/libvirt/Vm.hpp
)
