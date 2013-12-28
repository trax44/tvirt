#ifndef RAY11_RETURN_HPP_
#define RAY11_RETURN_HPP_

#include <string>
#include <iostream>

template <class T>
class Return {

public:
  T data;
  bool success;

  Return (bool _success):
    success(_success) {
  }
  
  Return (T _data, bool _success): 
    data(_data),
    success(_success) {
  }

  Return (T _data): 
    data(_data),
    success(true) {
  }

  template<typename CurrentType>
  Return (const Return <CurrentType> &r){
    data     = r.data;
    success  = r.success;
  }

  template<typename CurrentType>
  Return (Return <CurrentType> &r){
    data     = r.data;
    success  = r.success;
  }

};

template<>
class Return < void > {

public:
  bool success;

  Return (bool _success):
    success (_success){
  }
};

#endif


