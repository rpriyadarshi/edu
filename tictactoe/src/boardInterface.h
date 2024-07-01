#ifndef __BOARDINTERFACE_H
#define __BOARDINTERFACE_H

//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "utl.h"
#include "findfailed.h"
#include "usedentry.h"

namespace NGame {
  class CBoardInterface {
  public: // functions
    CBoardInterface();
    virtual ~CBoardInterface();
    
    // get interface functions.
    const CNodeInterface* getParent() const;
    const CNodeInterface* getChild() const;
    virtual const enum minmaxE data(const int row, const int col) const = 0;
    virtual const int getData() const = 0;
    virtual const int getSize() const = 0;
    virtual const int getChildSize() const = 0;
    virtual const int getUtility() const = 0;
    virtual const int getIdentity() const = 0;
    
    // operators.
    virtual const bool operator==(const CBoardInterface& board) const = 0;
    
    // set interface functions.
    virtual void data(const int row, const int col, 
      const enum minmaxE who) throw (NException::CUsedEntry) = 0;
    
    // drawing function
    virtual void draw(const int indent, std::ostream& o) const = 0;
    
    // algorithms
    virtual const bool isFilled(const int row, const int col) const = 0;
    virtual const bool isEmpty() const = 0;
    virtual const bool isMovesRemaining() const = 0;
    virtual const bool terminal() const = 0;
    virtual const int utility() const = 0;
    virtual enum minmaxE identity() const = 0;
    
    // searches.
    virtual CBoardInterface& find(const CBoardInterface& likeMe, 
      const bool deep = false) throw (NException::CFindFailed) = 0;
    virtual CBoardInterface& findBestMin() = 0;
    virtual CBoardInterface& findBestMax() = 0;
    
    // build functions.
    virtual const int buildMinMaxTree() throw (std::bad_alloc) = 0;
    virtual const int buildAlphaBetaTree(int& alpha, int& beta) 
      throw (std::bad_alloc) = 0;
    
    // debug interface
    virtual void trace(const int indent, std::ostream& o, 
      int deep = 0) const = 0;
    virtual void dump(const int indent, std::ostream& o, 
      int deep = 0) const = 0;
    
    // reset data
    virtual void reset() = 0;
    
    // duplication etc.
    virtual void copy(const CBoardInterface& board) = 0; 
    
    // update functions
    void updateParent(CNodeInterface* p_parent);
    void updateChild(CNodeInterface* p_child);
    virtual void updateIdentity(const enum minmaxE value) = 0;
    
  public: // static functions
    // Utility functions.
    static const std::string drawEnum(const enum minmaxE who);
    
  protected: // functions
    // set functions
    void setParent(CNodeInterface* p_parent);
    void setChild(CNodeInterface* p_child);
    
  private:
    CNodeInterface* mp_parent;
    CNodeInterface* mp_child;
  };
  
  inline const CNodeInterface* 
    CBoardInterface::getParent() const
  {
    return mp_parent;
  }
  
  inline void 
    CBoardInterface::setParent(CNodeInterface* p_parent)
  {
    mp_parent = p_parent;
  }
  
  inline void 
    CBoardInterface::updateParent(CNodeInterface* p_parent)
  {
    setParent(p_parent);
  }
  
  inline const CNodeInterface* 
    CBoardInterface::getChild() const
  {
    return mp_child;
  }
  
  inline void 
    CBoardInterface::setChild(CNodeInterface* p_child)
  {
    mp_child = p_child;
  }
  inline void 
    CBoardInterface::updateChild(CNodeInterface* p_child)
  {
    setChild(p_child);
  }
}

#endif // __BOARDINTERFACE_H
