//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "board.h"
#include "boardnode.h"

namespace NGame {
  CBoard::CBoard() 
    : m_data(0), m_childSize(c_gameSize*c_gameSize), 
    m_utility(0), m_identity(empty)
  {
  }
  
  CBoard::CBoard(const enum minmaxE who) 
    : m_data(0), m_childSize(c_gameSize*c_gameSize), 
    m_utility(0), m_identity(who)
  {
  }
  
  CBoard::~CBoard() 
  {
  }
  
  const enum minmaxE 
    CBoard::data(const int row, const int col) const
  {
    int index = row*c_gameSize+col;
    int data(m_data);
    data = data >> (c_bitWidth*index);
    
    return unsigned2minmax(c_dataMask&data);
  }
  
  const int 
    CBoard::getSize() const
  {
    const CNodeInterface* p_parent = getParent();
    return p_parent == 0 ? 1 : p_parent->getSize();
  }
  
  const bool 
    CBoard::isFilled(const int row, const int col) const
  {
    return data(row, col) == empty ? false : true;
  }
  
  void 
    CBoard::data(const int row, const int col, const enum minmaxE who)
    throw (NException::CUsedEntry)
  {
    if (isFilled(row, col)) throw NException::CUsedEntry();
    else setChildSize(getChildSize() - 1);
        
    unsigned v = minmax2unsigned(who);
    int index = row*c_gameSize+col;
    v = v << (c_bitWidth*index);
    
    int mask(c_dataMask);
    mask = mask << (c_bitWidth*index);
    
    m_data = m_data&(~mask);
    m_data = m_data|v;
  }
  
  void 
    CBoard::draw(const int indent, std::ostream& o) const
  {
    for (int row = 0; row < c_gameSize-1; row++) {
      o << NUtil::indent(indent);
      for (int col = 0; col < c_gameSize-1; col++) {
        o << drawEnum(data(row, col));
        o << "|";
      }
      o << drawEnum(data(row, col));
      o << std::endl;
      o << NUtil::indent(indent);
      for (col = 0; col < c_gameSize-1; col++) {
        o << "-+";
      }
      o << "-";
      o << std::endl;
    }
    o << NUtil::indent(indent);
    for (int col = 0; col < c_gameSize-1; col++) {
      o << drawEnum(data(row, col));
      o << "|";
    }
    o << drawEnum(data(row, col));
    o << std::endl;
  }
  
  const bool 
    CBoard::isEmpty() const
  {
    return getChildSize() == 0 ? false : true;
  }
  
  const bool 
    CBoard::isMovesRemaining() const
  {
    return getSize()%2 == 0 ? false : true;
  }
  
  const bool 
    CBoard::terminal() const
  {
    register enum minmaxE testCol = empty;
    bool result = false;
    
    // column tests
    testCol = data(0, 0);
    if (data(1, 0) == testCol && data(2, 0) == testCol && 
      testCol != empty) result = true;
    
    testCol = data(0, 1);
    if (data(1, 1) == testCol && data(2, 1) == testCol && 
      testCol != empty) result = true;
    
    testCol = data(0, 2);
    if (data(1, 2) == testCol && data(2, 2) == testCol && 
      testCol != empty) result = true;
    
    // row tests.
    testCol = data(0, 0);
    if (data(0, 1) == testCol && data(0, 2) == testCol && 
      testCol != empty) result = true;
    
    testCol = data(1, 0);
    if (data(1, 1) == testCol && data(1, 2) == testCol && 
      testCol != empty) result = true;
    
    testCol = data(2, 0);
    if (data(2, 1) == testCol && data(2, 2) == testCol && 
      testCol != empty) result = true;
    
    // diagonal tests
    testCol = data(0, 0);
    if (data(1, 1) == testCol && data(2, 2) == testCol && 
      testCol != empty) result = true;
    
    testCol = data(0, 2);
    if (data(1, 1) == testCol && data(2, 0) == testCol && 
      testCol != empty) result = true;
    
    if (isMovesRemaining()) result = false;

    // if all the entries are full
    if (isEmpty() == false) result = true;
    
    return result;
  }
  
  const int 
    CBoard::calculateUtility(const enum minmaxE who, const int value) const
  {
    switch (who) {
    case empty: return value;
    case min: return value-1;
    case max: return value+1;
    };
    return value;
  }
  
  const int 
    CBoard::utility() const
  {
    register enum minmaxE testCol = empty;
    register int utilVal = 0;
    
    // column tests
    testCol = data(0, 0);
    if (data(1, 0) == testCol && data(2, 0) == testCol && 
      testCol != empty) { // winner
      utilVal = calculateUtility(testCol, utilVal);
    }
    
    testCol = data(0, 1);
    if (data(1, 1) == testCol && data(2, 1) == testCol && 
      testCol != empty) { // winner
      utilVal = calculateUtility(testCol, utilVal);
    }
    
    testCol = data(0, 2);
    if (data(1, 2) == testCol && data(2, 2) == testCol && 
      testCol != empty) { // winner
      utilVal = calculateUtility(testCol, utilVal);
    }
    
    // row tests.
    testCol = data(0, 0);
    if (data(0, 1) == testCol && data(0, 2) == testCol && 
      testCol != empty) { // winner
      utilVal = calculateUtility(testCol, utilVal);
    }
    
    testCol = data(1, 0);
    if (data(1, 1) == testCol && data(1, 2) == testCol && 
      testCol != empty) { // winner
      utilVal = calculateUtility(testCol, utilVal);
    }
    
    testCol = data(2, 0);
    if (data(2, 1) == testCol && data(2, 2) == testCol && 
      testCol != empty) { // winner
      utilVal = calculateUtility(testCol, utilVal);
    }
    
    // diagonal tests
    testCol = data(0, 0);
    if (data(1, 1) == testCol && data(2, 2) == testCol && 
      testCol != empty) { // winner
      utilVal = calculateUtility(testCol, utilVal);
    }
    
    testCol = data(0, 2);
    if (data(1, 1) == testCol && data(2, 0) == testCol && 
      testCol != empty) { // winner
      utilVal = calculateUtility(testCol, utilVal);
    }
    
    //return utilVal;
    if (utilVal > 0) {
      return 1;
    } else if (utilVal < 0) {
      return -1;
    }
    return 0;
  }
  
  const bool 
    CBoard::operator==(const CBoardInterface& board) const
  {
    if (&board == this) return true;
    if (getData() == board.getData()) return true;
    return false;
  }
  
  void 
    CBoard::copy(const CBoardInterface& board)
  {
    if (&board == this) return;
    
    setData(board.getData());
    setChildSize(board.getChildSize());
    setUtility(board.getUtility());
    setIdentity(board.getIdentity());
  }
  
  CNodeInterface&
    CBoard::createChild() throw (std::bad_alloc)
  {
    CBoardNode* p_child = new CBoardNode();
    p_child->updateParent(this);
    setChild(p_child);
    return *p_child;
  }
  
  CBoardInterface&
    CBoard::find(const CBoardInterface& likeMe, 
    const bool deep) throw (NException::CFindFailed)
  {
    if (likeMe == *this) return *this;
    
    const CNodeInterface* p_child = getChild();
    if (! p_child) throw NException::CFindFailed();
    
    return p_child->find(likeMe, deep);
  }
  
  CBoardInterface&
    CBoard::findBestMin()
  {
    const CNodeInterface* p_child = getChild();
    if (! p_child) return *this;
    
    return p_child->findBestMin();
  }
  
  CBoardInterface&
    CBoard::findBestMax()
  {
    const CNodeInterface* p_child = getChild();
    if (! p_child) return *this;
    
    return p_child->findBestMax();
  }
  
  const int
    CBoard::buildMinMaxTree() throw (std::bad_alloc)
  {
    if (terminal() == true) {
      setUtility(utility());
      return getUtility();
    }
    
    CNodeInterface& child = createChild();
    const enum minmaxE who = identity();
    register enum minmaxE newWho = empty;
    switch (who) // toggle enum type.
    {
    case empty: assert(0); break;
    case min: newWho = max; break;
    case max: newWho = min; break;
    }

    int utility = child.buildMinMaxTree(newWho);
    setUtility(utility);
    return utility;
  }
  
  const int
    CBoard::buildAlphaBetaTree(int& alpha, int& beta) throw (std::bad_alloc)
  {
    if (terminal() == true) {
      setUtility(utility());
      return getUtility();
    }
    
    CNodeInterface& child = createChild();
    const enum minmaxE who = identity();
    register enum minmaxE newWho = empty;
    switch (who) // toggle enum type.
    {
    case empty: assert(0); break;
    case min: newWho = max; break;
    case max: newWho = min; break;
    }

    int utility = child.buildAlphaBetaTree(newWho, alpha, beta);
    setUtility(utility);
    return utility;
  }
  
  void 
    CBoard::trace(const int indent, std::ostream& o, int deep) const
  {
  }
  
  void 
    CBoard::dump(const int indent, std::ostream& o, int deep) const
  {
    int indent0(indent);
    int indent1(indent0 + NUtil::c_indentIncr);
    int indent2(indent1 + NUtil::c_indentIncr);

    --deep;
    
    o << NUtil::indent(indent0) 
      << "BOARD {" << std::endl;
    o << NUtil::indent(indent1) 
      << "SIZE " << sizeof(*this) << std::endl;
    o << NUtil::indent(indent1) 
      << "IDENTITY " << enum2string(identity()) << std::endl;
    o << NUtil::indent(indent1) 
      << "UTILITY " << getUtility() << std::endl;
    o << NUtil::indent(indent1) 
      << "CHILD_SIZE " << getChildSize() << std::endl;
    o << NUtil::indent(indent1) 
      << "CALC_TERMINAL " << enum2string(terminal()) << std::endl;
    o << NUtil::indent(indent1) 
      << "CALC_UTILITY " << utility() << std::endl;
    o << NUtil::indent(indent1) 
      << "DATA {" << std::endl;
    draw(indent2, o);
    o << NUtil::indent(indent1) << "}" << std::endl;
    
    if (getChild() == 0) {
      o << NUtil::indent(indent1) 
        << "CHILD_NODE <null>" << std::endl;
    } else {
      if (deep > 0) {
        getChild()->dump(indent1, o, deep);
      } else {
      o << NUtil::indent(indent1) 
        << "CHILD_NODE <not traced>" << std::endl;
      }
    }
    
    o << NUtil::indent(indent0) << "}" << std::endl;
  }
  
  void 
    CBoard::reset()
  {
    m_data = 0;
    m_childSize = c_gameSize*c_gameSize;
    m_utility = 0;
  }
}