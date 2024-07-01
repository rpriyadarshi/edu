//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "boardnode.h"
#include "boardinterface.h"

namespace NGame {
  CNodeInterface::CNodeInterface()
  {
  }
  
  CNodeInterface::~CNodeInterface()
  {
  }
  
  const enum minmaxE 
    CNodeInterface::data(const int row, const int col, const int index) const
  {
    return getBoard() == 0 ? empty : board(index).data(row, col);
  }
  
  void 
    CNodeInterface::data(const int row, const int col, const enum minmaxE who, 
    const int index) throw (NException::CUsedEntry)
  {
    if (getBoard() == 0) return;
    board(index).data(row, col, who);
  }
  
}
