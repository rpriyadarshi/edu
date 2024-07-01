#ifndef __UTL_H
#define __UTL_H

//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "common.h"

const int Byte = 8;

namespace NUtil {
  template <class T>
    void printBits(std::ostream& o, T f, int size)
  {
    char* buf = new char[size+1];
    T flag = 1;
    for (int i = 0; i < size; i++) {
      T bit = f&flag;
      if (bit == 0) {
        buf[size-i-1] = '0';
      } else {
        buf[size-i-1] = '1';
      }
      flag = flag << 1;
    }
    buf[size] = '\0';
    o << buf;
    delete [] buf;
  }
  
  template <class T>
    void printBits(std::ostream& o, T f)
  {
    int size = sizeof(T)*Byte;
    printBits(o, f, size);
  }
  
  inline std::string
    indent(const int indent)
  {
    std::string str;
    for (int i = 0; i < indent; i++) str += " ";
    return str;
  }
  
  const int c_indentIncr = 2;
  extern const double c_oneByRootTwo;

  template <class T>
    T max(T v1, T v2)
  {
    return v1 > v2 ? v1 : v2;
  }

  template <class T>
    T min(T v1, T v2)
  {
    return v1 < v2 ? v1 : v2;
  }
}

#endif // __UTL_H
