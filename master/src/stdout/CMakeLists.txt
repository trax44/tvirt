add_executable(master main.cpp Print.cpp Print.hpp)
target_link_libraries (master commZmq proto)
