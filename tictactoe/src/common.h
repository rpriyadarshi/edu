#ifndef __COMMON_H
#define __COMMON_H

//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <string>
#include <new>
#include <math.h>
#include <exception>

namespace NGame {

  // forward declarations
  class CNodeInterface;
  class CRootNode;
  class CBoardNode;
  class CBoardInterface;
  class CBoard;
  
  const int c_gameSize = 3;
  const int c_bitWidth = 2;
  const int c_dataMask = 3;
  const int c_filledMask = 1;
  const int c_size = 4;
  const int c_maxSize = (const int)(pow(2.0, c_size));//16;// 2^c_size
  const int c_dataSize = c_gameSize*c_gameSize*c_bitWidth;
  const int c_maxData = (const int)(pow(2.0, c_dataSize));
  const int c_utilSize = 4;
  const int c_identitySize = 2;
  const int c_minUtil = -2;
  const int c_maxUtil = 2;

  // command strings.
  const std::string c_newCmd = "new";
  const std::string c_pieceCmd = "piece";
  const std::string c_moveCmd = "move";
  const std::string c_drawCmd = "draw";
  const std::string c_dumpCmd = "dump";
  const std::string c_quitCmd = "quit";
  const std::string c_helpCmd = "help";
  const std::string c_promptCmd = "prompt";
  const std::string c_buildCmd = "build";
  const std::string c_traceCmd = "trace";

  // other vars.
  const std::string c_minmax = "minmax";
  const std::string c_alphabeta = "alphabeta";
  const std::string c_on = "on";
  const std::string c_off = "off";
  
  enum minmaxE { empty = 0, min = 1, max = 2 };
  enum commandE { badCmd, newCmd, pieceCmd, moveCmd, drawCmd, 
    dumpCmd, quitCmd, helpCmd, promptCmd, buildCmd, traceCmd };
  
  // Utility functions.
  inline const unsigned 
    bool2unsigned(const bool value) {
    return (unsigned)(value);
  }
  inline const unsigned 
    minmax2unsigned(const enum minmaxE who) {
    return (unsigned)(who);
  }
  inline const bool 
    unsigned2bool(const unsigned value) {
    return value == 0 ? false : true;
  }
  inline const enum minmaxE 
    unsigned2minmax(const unsigned who) {
    unsigned emptyWho = minmax2unsigned(empty);
    unsigned maxWho = minmax2unsigned(max);
    assert(who >= emptyWho);
    assert(who <= maxWho);
    return (const enum minmaxE)(who);
  }
  inline const std::string 
    enum2string(const enum minmaxE who) {
    switch (who) {
    case empty: return "empty"; 
    case min: return "min"; 
    case max: return "max"; 
    };
    return "<null>"; 
  }
  inline const std::string 
    enum2string(const bool value) {
    if (value == true) return "true"; 
    return "false"; 
  }
}

#endif // __COMMON_H
