//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "rootnode.h"
#include "board.h"

namespace NGame {
  CRootNode::CRootNode()
    : CRootInterface(), mp_currBoard(getBoard())
  {
  }
  
  CRootNode::~CRootNode()
  {
  }
  
  void
    CRootNode::markMin()
  {
  }
  
  void
    CRootNode::markMax()
  {
  }
  
  void
    CRootNode::automaticMove() throw (NException::CWin)
  {
    CBoardInterface* p_currBoard = 
      const_cast<CBoardInterface*>(getCurrBoard());
    assert(p_currBoard);
    
    const CBoardInterface* p_newBoard = 0;
    switch (p_currBoard->identity()) // toggle enum type.
    {
    case empty: assert(0); break;
    case min: p_newBoard = &p_currBoard->findBestMax(); break;
    case max: p_newBoard = &p_currBoard->findBestMin(); break;
    }
    
    assert(p_newBoard);
    updateCurrBoard(p_newBoard);
    
    if (p_newBoard->getChild() == 0) throw NException::CWin();
  }
  
  void
    CRootNode::manualMove(const int row, const int col) 
    throw (NException::CFindFailed, NException::CWin, NException::CUsedEntry)
  {
    CBoardInterface* p_currBoard = 
      const_cast<CBoardInterface*>(getCurrBoard());
    assert(p_currBoard);
    
    CBoard seed;
    seed.copy(*p_currBoard);
    switch (p_currBoard->identity()) // toggle enum type.
    {
    case empty: assert(0); break;
    case min: seed.data(row, col, max); break;
    case max: seed.data(row, col, min); break;
    }
    
    const CBoardInterface* p_newBoard = &p_currBoard->find(seed);
    assert(p_newBoard);
    updateCurrBoard(p_newBoard);
    
    if (p_newBoard->getChild() == 0) throw NException::CWin();
  }
  
  void 
    CRootNode::resetGame() 
  {
    updateCurrBoard(getBoard());
  }
  
  void 
    CRootNode::trace(const int indent, std::ostream& o,
    int deep) const
  {
  }
  
  void 
    CRootNode::dump(const int indent, std::ostream& o,
    int deep) const
  {
    int indent0(indent);
    int indent1(indent0 + NUtil::c_indentIncr);
    
    o << NUtil::indent(indent0) 
      << "ROOT_NODE {" << std::endl;
    o << NUtil::indent(indent1) 
      << "SIZE " << sizeof(*this) << std::endl;
    if (getBoard() == 0) {
      o << NUtil::indent(indent1) 
        << "BOARD <null>" << std::endl;
    } else {
      getBoard()->dump(indent1, o, deep);
    }
    
    o << NUtil::indent(indent0) << "}" << std::endl;
  }
}
