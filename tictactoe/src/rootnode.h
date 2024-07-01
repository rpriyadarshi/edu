#ifndef __ROOTNODE_H
#define __ROOTNODE_H

//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "rootinterface.h"

namespace NGame {
  class CRootNode : public CRootInterface {
  public: // functions
    CRootNode();
    ~CRootNode();
    
    // access functions.
    virtual const CBoardInterface* getCurrBoard() const;
    
    // algorithms 
    virtual void markMin();
    virtual void markMax();
    virtual void automaticMove() throw (NException::CWin);
    virtual void manualMove(const int row, const int col) 
      throw (NException::CFindFailed, NException::CWin, 
      NException::CUsedEntry);
    
    // access functions.
    virtual void updateCurrBoard(const CBoardInterface* p_currBoard);
    
    // reset functions.
    void resetGame();
    
    // debug interface
    virtual void trace(const int indent, std::ostream& o, 
      int deep = 0) const;
    virtual void dump(const int indent, std::ostream& o, 
      int deep = 0) const;
    
  protected: // functions.
    // access functions.
    void setCurrBoard(const CBoardInterface* p_currBoard);
    
  private: // data.
    const CBoardInterface* mp_currBoard;
  };
  
  inline const NGame::CBoardInterface* 
    CRootNode::getCurrBoard() const
  {
    return mp_currBoard;
  }
  
  inline void 
    CRootNode::setCurrBoard(const CBoardInterface* p_currBoard)
  {
    mp_currBoard = p_currBoard;
  }
  
  inline void 
    CRootNode::updateCurrBoard(const CBoardInterface* p_currBoard)
  {
    setCurrBoard(p_currBoard);
  }
}

#endif // __ROOTNODE_H
