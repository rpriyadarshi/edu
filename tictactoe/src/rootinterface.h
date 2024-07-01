#ifndef __ROOTINTERFACE_H
#define __ROOTINTERFACE_H

//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "nodeinterface.h"

namespace NGame {
  class CRootInterface : public CNodeInterface {
  public: // functions
    CRootInterface();
    ~CRootInterface();
    
    // get functions.
    virtual const CBoardInterface* getBoard() const;
    virtual const CBoardInterface* getParent() const;
    virtual const int getSize() const;
    
    // access
    virtual CBoardInterface& board(const int index) const;
    virtual CBoardInterface& operator[](const int index) const;
    
    // create functions.
    virtual CBoardInterface& createBoard(const enum minmaxE who) 
      throw (std::bad_alloc);
    
    // searches.
    virtual CBoardInterface& find(const CBoardInterface& likeMe, 
      const bool deep = false) const throw (NException::CFindFailed);
    virtual CBoardInterface& findBestMin() const;
    virtual CBoardInterface& findBestMax() const;

    // build functions.
    virtual const int buildMinMaxTree() throw (std::bad_alloc);
    virtual const int buildMinMaxTree(const enum minmaxE who) 
      throw (std::bad_alloc);
    virtual const int buildAlphaBetaTree() throw (std::bad_alloc);
    virtual const int buildAlphaBetaTree(const enum minmaxE who, 
      int alpha, int beta) throw (std::bad_alloc);
    
    // update data
    virtual void updateIdentity(const enum minmaxE value);

    // debug interface
    virtual void trace(const int indent, std::ostream& o, 
      int deep = 0) const;
    virtual void dump(const int indent, std::ostream& o, 
      int deep = 0) const;
    
  protected: // functions
    // set functions
    void setBoard(CBoardInterface* p_board);
    
  private: // data
    CBoardInterface* mp_board;
  };
  
  inline const CBoardInterface* 
    CRootInterface::getBoard() const
  {
    return mp_board;
  }
  
  inline const int 
    CRootInterface::getSize() const
  {
    return 1;
  }
  
  inline const CBoardInterface* 
    CRootInterface::getParent() const
  {
    return 0;
  }
  
  inline void 
    CRootInterface::setBoard(CBoardInterface* p_board)
  {
    mp_board = p_board;
  }
}

#endif // __ROOTINTERFACE_H
