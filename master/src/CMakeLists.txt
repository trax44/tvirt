add_executable(
  master-web
  Requester.hpp
  Requester.cpp
  ViewControler.cpp
  ViewControler.hpp
  ViewModel.hpp

  web/main.cpp
  web/HypervisorConnect.cpp
  web/HypervisorConnect.hpp
  )


set(WITTY_LIBRARIES -lwt -lwthttp -lboost_signals)
target_link_libraries (master-web commZmq proto ${WITTY_LIBRARIES})




add_executable(
  master-stdout 
  stdout/main.cpp 
  stdout/Print.cpp 
  stdout/Print.hpp
  )

target_link_libraries (master-stdout commZmq proto)
