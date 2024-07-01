#ifndef __NODEINTERFACE_H
#define __NODEINTERFACE_H

//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "findfailed.h"
#include "usedentry.h"
#include "win.h"

namespace NGame {
  // forward declaration
  class CBoardInterface;
  
  class CNodeInterface {
  public: // functions
    CNodeInterface();
    ~CNodeInterface();
    
    // get functions.
    const enum minmaxE data(const int row, const int col, 
      const int index = 0) const;
    virtual const CBoardInterface* getCurrBoard() const = 0;
    virtual const CBoardInterface* getBoard() const = 0;
    virtual const CBoardInterface* getParent() const = 0;
    virtual const int getSize() const = 0;
    
    // access
    void data(const int row, const int col, const enum minmaxE who, 
      const int index = 0) throw (NException::CUsedEntry);
    virtual CBoardInterface& board(const int index) const = 0;
    virtual CBoardInterface& operator[](const int index) const = 0;
    
    // create functions.
    virtual CBoardInterface& createBoard(const enum minmaxE who) 
      throw (std::bad_alloc) = 0;
    
    // searches.
    virtual CBoardInterface& find(const CBoardInterface& likeMe, 
      const bool deep = false) const throw (NException::CFindFailed) = 0;
    virtual CBoardInterface& findBestMin() const = 0;
    virtual CBoardInterface& findBestMax() const = 0;
    
    // algorithms 
    virtual void markMin() = 0;
    virtual void markMax() = 0;
    virtual void automaticMove() throw (NException::CWin) = 0;
    virtual void manualMove(const int row, const int col) 
      throw (NException::CFindFailed, NException::CWin) = 0;
    
    // build functions.
    virtual const int buildMinMaxTree() throw (std::bad_alloc) = 0;
    virtual const int buildMinMaxTree(const enum minmaxE who) 
      throw (std::bad_alloc) = 0;
    virtual const int buildAlphaBetaTree() throw (std::bad_alloc) = 0;
    virtual const int buildAlphaBetaTree(const enum minmaxE who, 
      int alpha, int beta) throw (std::bad_alloc) = 0;
    
    // update data
    virtual void updateIdentity(const enum minmaxE value) = 0;
    virtual void updateCurrBoard(const CBoardInterface* p_currBoard) = 0;
    
    // debug interface
    virtual void trace(const int indent, std::ostream& o, 
      int deep = 0) const = 0;
    virtual void dump(const int indent, std::ostream& o, 
      int deep = 0) const = 0;
  };
}

#endif // __NODEINTERFACE_H
