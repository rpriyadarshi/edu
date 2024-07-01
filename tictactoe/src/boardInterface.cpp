//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "boardInterface.h"

namespace NGame {
  CBoardInterface::CBoardInterface() 
    : mp_parent(0), mp_child(0)
  {
  }
  
  CBoardInterface::~CBoardInterface() 
  {
  }
  
  const std::string 
    CBoardInterface::drawEnum(const enum minmaxE who) 
  {
    switch (who) {
    case empty: return " "; 
    case min: return "O"; 
    case max: return "X"; 
    };
    return "<bad>"; 
  }
}
