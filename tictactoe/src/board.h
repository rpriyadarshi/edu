#ifndef __BOARD_H
#define __BOARD_H

//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "boardInterface.h"

namespace NGame {  
  class CBoard : public CBoardInterface {
  public: // functions
    CBoard();
    explicit CBoard(const enum minmaxE who);
    ~CBoard();
    
    // get interface functions.
    virtual const enum minmaxE data(const int row, const int col) const;
    virtual const int getData() const;
    virtual const int getSize() const;
    virtual const int getChildSize() const;
    virtual const int getUtility() const;
    virtual const int getIdentity() const;
    
    // operators.
    virtual const bool operator==(const CBoardInterface& board) const;
    
    // duplication etc.
    virtual void copy(const CBoardInterface& board); 
    
    // set interface functions.
    virtual void data(const int row, const int col, const enum minmaxE who)
      throw (NException::CUsedEntry);
    
    // drawing function
    virtual void draw(const int indent, std::ostream& o) const;
    
    // create functions.
    virtual CNodeInterface& createChild() throw (std::bad_alloc);
    
    // searches.
    virtual CBoardInterface& find(const CBoardInterface& likeMe, 
      const bool deep = false) throw (NException::CFindFailed);
    virtual CBoardInterface& findBestMin();
    virtual CBoardInterface& findBestMax();
    
    // build functions.
    virtual const int buildMinMaxTree() throw (std::bad_alloc);
    virtual const int buildAlphaBetaTree(int& alpha, int& beta) 
      throw (std::bad_alloc);
    
    // utilities
    virtual const bool isFilled(const int row, const int col) const;
    virtual const bool isEmpty() const;
    virtual const bool isMovesRemaining() const;
    virtual const bool terminal() const;
    virtual const int utility() const;
    virtual enum minmaxE identity() const;
    
    // debug interface
    virtual void trace(const int indent, std::ostream& o, 
      int deep = 0) const;
    virtual void dump(const int indent, std::ostream& o, 
      int deep = 0) const;
    
    // reset data
    virtual void reset();
    
    // set interface functions.
    void updateUtility(const int value);
    virtual void updateIdentity(const enum minmaxE value);
    
  protected: // functions
    const int calculateUtility(const enum minmaxE who, const int value) const;
    
    // set interface functions.
    void setData(const int value);
    void setChildSize(const int value);
    void setUtility(const int value);
    void setIdentity(const int value);
    void identity(const enum minmaxE value);
    
  private: // data
    unsigned m_data:c_dataSize;
    unsigned m_childSize:c_size; // range is 1-9
    signed   m_utility:c_utilSize;
    unsigned m_identity:c_identitySize;
    
  };
  
  inline const int 
    CBoard::getData() const
  {
    return m_data;
  }
  
  inline const int 
    CBoard::getChildSize() const
  {
    return m_childSize;
  }
  
  inline const int 
    CBoard::getUtility() const
  {
    return m_utility;
  }
  
  inline const int 
    CBoard::getIdentity() const
  {
    return m_identity;
  }
  
  inline void 
    CBoard::setData(const int value)
  {
    assert(value < c_maxData);
    m_data = value;
  }
  
  inline void 
    CBoard::setChildSize(const int value)
  {
    assert(value < c_maxSize);
    m_childSize = value;
  }
  
  inline void 
    CBoard::setUtility(const int value)
  {
    assert(value < c_utilSize);
    m_utility = value;
  }
  
  inline void 
    CBoard::setIdentity(const int value)
  {
    m_identity = value;
  }
  
  inline void 
    CBoard::updateUtility(const int value)
  {
    setUtility(value);
  }
  
  inline void 
    CBoard::updateIdentity(enum minmaxE value)
  {
    setIdentity(minmax2unsigned(value));
  }
  
  inline enum minmaxE 
    CBoard::identity() const
  {
    return unsigned2minmax(getIdentity());
  }
  
  inline void 
    CBoard::identity(enum minmaxE value)
  {
    updateIdentity(value);
  }
}

#endif // __BOARD_H
