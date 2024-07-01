#ifndef __BOARDNODE_H
#define __BOARDNODE_H

//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "rootnode.h"

namespace NGame {
  class CBoardNode : public CRootNode {
  public: // functions
    explicit CBoardNode();
    ~CBoardNode();
    
    // get functions.
    virtual const CBoardInterface* getParent() const;
    virtual const int getSize() const;
    
    // create functions.
    virtual CBoardInterface& createBoard(const enum minmaxE who) 
      throw (std::bad_alloc);
    
    // algorithms 
    virtual void markMin();
    virtual void markMax();
    
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
    
    // update functions.
    void updateParent(CBoardInterface* p_parent);
    
    // debug interface
    virtual void trace(const int indent, std::ostream& o, 
      int deep = 0) const;
    virtual void dump(const int indent, std::ostream& o, 
      int deep = 0) const;
    
  protected: // functions
    // set functions
    void setParent(CBoardInterface* p_parent);
    
  private: // data
    CBoardInterface* mp_parent;
  };
  
  inline const CBoardInterface* 
    CBoardNode::getParent() const
  {
    return mp_parent;
  }
  
  inline void 
    CBoardNode::setParent(CBoardInterface* p_parent)
  {
    mp_parent = p_parent;
  }
  
  inline void 
    CBoardNode::updateParent(CBoardInterface* p_parent)
  {
    setParent(p_parent);
  }
}

#endif // __BOARDNODE_H
