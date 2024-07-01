//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "board.h"
#include "rootinterface.h"

namespace NGame {
  CRootInterface::CRootInterface()
    : mp_board(0)
  {
    createBoard(empty); // the root board.
  }
  
  CRootInterface::~CRootInterface()
  {
    delete mp_board;
  }
  
  CBoardInterface& 
    CRootInterface::operator[](const int index) const
  {
    return board(index);
  }
  
  CBoardInterface& 
    CRootInterface::board(const int index) const
  {
    const CBoard* p_board = dynamic_cast<const CBoard*>(getBoard());
    return const_cast<CBoard&>(p_board[index]);
  }
  
  CBoardInterface& // unused who
    CRootInterface::createBoard(const enum minmaxE who) throw (std::bad_alloc)
  {
    CBoardInterface* p_board = new CBoard();
    p_board->updateParent(this);
    setBoard(p_board);
    
    return *p_board;
  }
  
  CBoardInterface&
    CRootInterface::find(const CBoardInterface& likeMe, 
    const bool deep) const throw (NException::CFindFailed)
  {
    CBoardInterface* p_board = const_cast<CBoardInterface*>(getBoard());
    assert(p_board);
    
    return p_board->find(likeMe, deep); // can throw exception
  }
  
  CBoardInterface&
    CRootInterface::findBestMin() const
  {
    CBoardInterface* p_board = const_cast<CBoardInterface*>(getBoard());
    assert(p_board);
    
    return *p_board;
  }
  
  CBoardInterface&
    CRootInterface::findBestMax() const
  {
    CBoardInterface* p_board = const_cast<CBoardInterface*>(getBoard());
    assert(p_board);
    
    return *p_board;
  }
  
  const int
    CRootInterface::buildMinMaxTree() throw (std::bad_alloc)
  {
    CBoardInterface* p_board = const_cast<CBoardInterface*>(getBoard());
    assert(p_board);
    return p_board->buildMinMaxTree();
  }
  
  const int
    CRootInterface::buildMinMaxTree(const enum minmaxE who) 
    throw (std::bad_alloc)
  {
    CBoardInterface* p_board = const_cast<CBoardInterface*>(getBoard());
    assert(p_board);
    return p_board->buildMinMaxTree();
  }
  
  const int
    CRootInterface::buildAlphaBetaTree() throw (std::bad_alloc)
  {
    CBoardInterface* p_board = const_cast<CBoardInterface*>(getBoard());
    assert(p_board);

    int alpha = c_minUtil, beta = c_maxUtil;
    return p_board->buildAlphaBetaTree(alpha, beta);
  }
  
  const int
    CRootInterface::buildAlphaBetaTree(const enum minmaxE who, 
    int alpha, int beta) throw (std::bad_alloc)
  {
    CBoardInterface* p_board = const_cast<CBoardInterface*>(getBoard());
    assert(p_board);
    return p_board->buildAlphaBetaTree(alpha, beta);
  }
  
  void 
    CRootInterface::updateIdentity(const enum minmaxE value)
  {
    CBoardInterface* p_board = const_cast<CBoardInterface*>(getBoard());
    assert(p_board);
    p_board->updateIdentity(value);
  }
  
  void 
    CRootInterface::trace(const int indent, std::ostream& o,
    int deep) const
  {
  }
  
  void 
    CRootInterface::dump(const int indent, std::ostream& o,
    int deep) const
  {
    int indent0(indent);
    int indent1(indent0 + NUtil::c_indentIncr);
    
    o << NUtil::indent(indent0) 
      << "ROOT_INTERFACE {" << std::endl;
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
