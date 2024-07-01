//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "boardnode.h"
#include "board.h"

namespace NGame {
  CBoardNode::CBoardNode()
    : CRootNode(), mp_parent(0)
  {
  }
  
  CBoardNode::~CBoardNode()
  {
    delete [] mp_parent;
  }
  
  const int 
    CBoardNode::getSize() const
  {
    const CBoardInterface* p_parent = getParent();
    return p_parent == 0 ? 0 : p_parent->getChildSize();
  }
  
  CBoardInterface& 
    CBoardNode::createBoard(const enum minmaxE who) throw (std::bad_alloc)
  {
    const CBoardInterface* p_parent = getParent();
    
    const int c_childSize = getSize();
    CBoard* p_board = new CBoard[c_childSize];
    for (int i = 0; i < c_childSize; i++) {
      p_board[i].copy(*p_parent);
      p_board[i].updateParent(this);
      p_board[i].updateIdentity(who);
    }
    setBoard(p_board);
    
    return *p_board;
  }
  
  void
    CBoardNode::markMin()
  {
    int i = 0;
    for (int row = 0; row < c_gameSize; row++) {
      for (int col = 0; col < c_gameSize; col++) {
        if (! board(i).isFilled(row, col)) {
          board(i++).data(row, col, min);
          if (i >= getSize()) return;
        }
      }
    }
  }
  
  void
    CBoardNode::markMax()
  {
    int i = 0;
    for (int row = 0; row < c_gameSize; row++) {
      for (int col = 0; col < c_gameSize; col++) {
        if (! board(i).isFilled(row, col)) {
          board(i++).data(row, col, max);
          if (i >= getSize()) return;
        }
      }
    }
  }
  
  CBoardInterface&
    CBoardNode::find(const CBoardInterface& likeMe, 
    const bool deep) const throw (NException::CFindFailed)
  {
    for (int i = 0; i < getSize(); i++) {
      CBoardInterface& currboard = const_cast<CBoardInterface&>(board(i));
      if (likeMe == currboard) return currboard;
      
      if (deep == true) {
        try { 
          return currboard.find(likeMe, deep);
        }
        catch (NException::CFindFailed) { // failed search
        } // no rethrow till later
      }
    }
    
    throw NException::CFindFailed();
    return const_cast<CBoardInterface&>(likeMe); // compiler helper.
  }
  
  CBoardInterface&
    CBoardNode::findBestMin() const
  {
    int util = c_maxUtil;
    CBoardInterface* p_bestBoard = 0;
    for (int i = 0; i < getSize(); i++) {
      CBoardInterface& currboard = const_cast<CBoardInterface&>(board(i));
      int currUtil = currboard.getUtility();
      if (currUtil < util) {
        util = currUtil;
        p_bestBoard = &currboard;
      }
    }

    assert(p_bestBoard);
    return *p_bestBoard;
  }
  
  CBoardInterface&
    CBoardNode::findBestMax() const
  {
    int util = c_minUtil;
    CBoardInterface* p_bestBoard = 0;
    for (int i = 0; i < getSize(); i++) {
      CBoardInterface& currboard = const_cast<CBoardInterface&>(board(i));
      int currUtil = currboard.getUtility();
      if (currUtil > util) {
        util = currUtil;
        p_bestBoard = &currboard;
      }
    }

    assert(p_bestBoard);
    return *p_bestBoard;
  }
  
  const int
    CBoardNode::buildMinMaxTree() throw (std::bad_alloc)
  {
    assert(0);
    return 0;
  }
  
  const int
    CBoardNode::buildMinMaxTree(const enum minmaxE who) throw (std::bad_alloc)
  {
    register int thisUtility = 0;
    register int i = 0;

    createBoard(who);
    switch (who) // toggle enum type.
    {
    case empty: assert(0); break;
    case min: 
      markMin(); 
      
      thisUtility = c_maxUtil; 
      for (i = 0; i < getSize(); i++) {
        board(i).buildMinMaxTree();
        thisUtility = NUtil::min(thisUtility, board(i).getUtility()); 
      }
      break;
    case max: 
      markMax(); 
      
      thisUtility = c_minUtil; 
      for (i = 0; i < getSize(); i++) {
        board(i).buildMinMaxTree();
        thisUtility = NUtil::max(thisUtility, board(i).getUtility()); 
      }  
      break;
    };
    
    return thisUtility;
  }
  
  const int
    CBoardNode::buildAlphaBetaTree() throw (std::bad_alloc)
  {
    assert(0);
    return 0;
  }
  
  const int
    CBoardNode::buildAlphaBetaTree(const enum minmaxE who, 
    int alpha, int beta) throw (std::bad_alloc)
  {
    register int thisUtility = 0;
    register int i = 0;

    createBoard(who);
    switch (who) // toggle enum type.
    {
    case empty: assert(0); break;
    case min: 
      markMin(); 
      
      thisUtility = beta; 
      for (i = 0; i < getSize(); i++) {
        board(i).buildAlphaBetaTree(alpha, thisUtility);
        thisUtility = NUtil::min(thisUtility, board(i).getUtility()); 
        if (thisUtility < alpha) break; // max will not choose this.
      }
      break;
    case max: 
      markMax(); 
      
      thisUtility = alpha; 
      for (i = 0; i < getSize(); i++) {
        board(i).buildAlphaBetaTree(thisUtility, beta);
        thisUtility = NUtil::max(thisUtility, board(i).getUtility()); 
        if (thisUtility > beta) break; // min will not choose this.
      }  
      break;
    };
    
    return thisUtility;
  }
  
  void 
    CBoardNode::trace(const int indent, std::ostream& o, 
    int deep) const
  {
  }
  
  void 
    CBoardNode::dump(const int indent, std::ostream& o, 
    int deep) const
  {
    int indent0(indent);
    int indent1(indent0 + NUtil::c_indentIncr);
    
    o << NUtil::indent(indent0) 
      << "BOARD_NODE {" << std::endl;
    o << NUtil::indent(indent1) 
      << "SIZE " << sizeof(*this) << std::endl;
    if (getBoard() == 0) {
      o << NUtil::indent(indent1) 
        << "BOARD <null>" << std::endl;
    } else {
      for (int i = 0; i < getSize(); i++) {
        board(i).dump(indent1, o, deep);
        /*
        o << NUtil::indent(indent1) 
        << "IDENTITY " << enum2string(board(i).identity()) << std::endl;
        board(i).draw(indent1, o);
        */
      }
    }
    
    o << NUtil::indent(indent0) << "}" << std::endl;
  }
}
