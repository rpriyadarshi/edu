//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
// Program: The Tic Tac Toe game.
// 
// Purpose: Implement the text based interface to the tic tac toe game.
// 
// Author : Rohit Priyadarshi
// Date   : 01/24/2002
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "textinterface.h"
#include "board.h"
#include "utl.h"

namespace NUserInterface {
  std::string CTextInterface::ms_prompt = "#";
  bool CTextInterface::ms_trace = false;
  
  CTextInterface::CTextInterface(NGame::CRootNode& root)
    : m_root(root)
  {
  }
  
  CTextInterface::~CTextInterface()
  {
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : Should have let user know how to play.
  // Postconditions: The minmax tree is built
  ////////////////////////////////////////////////////////////////////////////
  void
    CTextInterface::buildMinMaxTree() throw (std::bad_alloc)
  {
    getRoot().buildMinMaxTree();
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : Should have let user know how to play.
  // Postconditions: The minmax tree with alpha-beta pruning is built
  ////////////////////////////////////////////////////////////////////////////
  void
    CTextInterface::buildAlphaBetaTree() throw (std::bad_alloc)
  {
    getRoot().buildAlphaBetaTree();
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : none.
  // Postconditions: Printed the welcome message to the user.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::printWelcome(std::ostream& o) const
  {
    o << "*****************************************************" << std::endl;
    o << "*                                                   *" << std::endl;
    o << "*       Welcome to the world of Tic Tac Toe!        *" << std::endl;
    o << "*                                                   *" << std::endl;
    o << "*****************************************************" << std::endl;
    o << std::endl;
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : Finished playing one round of the game.
  // Postconditions: Printed the winner.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::printWinner(std::ostream& o) const
  {
    const NGame::CBoardInterface* p_board = getRoot().getCurrBoard();
    assert(p_board);
    int util = p_board->getUtility();
    
    o << std::endl;
    o << "*****************************************************" << std::endl;
    
    if (util > 0) {
      o << "* You win!                                          *" 
        << std::endl;
    } else if (util < 0) {
      o << "* I win!                                            *" 
        << std::endl;
    } else {
      o << "* It is a draw!                                     *" 
        << std::endl;
    }
    o << "*****************************************************" << std::endl;
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Printed error message.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::printUsedEntry(std::ostream& o) const
  {
    o << "*** ERROR: Used entry!" << std::endl;
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : The tree is built and user has placed a move.
  // Postconditions: Generated an automatic move.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::automaticMove(std::ostream& o)
    throw (NException::CWin)
  {
    o << std::endl;
    
    // create and find the corresponding board.
    try {
      getRoot().automaticMove();
      getRoot().getCurrBoard()->draw(0, o);
      if (getTrace()) getRoot().getCurrBoard()->dump(0, o, 2);
    }
    catch (NException::CWin& w) {
      getRoot().getCurrBoard()->draw(0, o);
      if (getTrace()) getRoot().getCurrBoard()->dump(0, o, 2);
      throw w;
    }
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : The tree is built and computer may have placed a move.
  // Postconditions: Accepted a move by user.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::manualMove(std::istream& i, std::ostream& o)
    throw (NException::CFindFailed, NException::CWin)
  {
    std::string rowStr, colStr;
    int row, col;
    do {
      o << std::endl;
      i >> rowStr;
      i >> colStr;
      
      row = atoi(rowStr.c_str());
      col = atoi(colStr.c_str());
    } while (row < 0 || row > NGame::c_gameSize-1 || 
      col < 0 || col > NGame::c_gameSize-1);
    
    // create and find the corresponding board.
    try {
      getRoot().manualMove(row, col);
      getRoot().getCurrBoard()->draw(0, o);
      if (getTrace()) getRoot().getCurrBoard()->dump(0, o, 2);
    }
    catch (NException::CWin& w) {
      getRoot().getCurrBoard()->draw(0, o);
      if (getTrace()) getRoot().getCurrBoard()->dump(0, o, 2);
      throw w;
    }
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : The tree is built.
  // Postconditions: Applied a move command.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::moveCmd(std::istream& i, std::ostream& o)
    throw (NException::CWin)
  {
    try {
      manualMove(i, o);
    }
    catch (NException::CUsedEntry&) {
      printUsedEntry(o);
      return;
    }
    catch (NException::CWin&) {
      try {
        automaticMove(o);
      }
      catch (NException::CWin&) {
        throw NException::CWin();
      }
    }
    try {
      automaticMove(o);
    }
    catch (NException::CWin&)  {
      throw NException::CWin();
    }
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : The tree is built.
  // Postconditions: Applied a command.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::command(std::istream& i, std::ostream& o)
    throw (NException::CQuit)
  {
    printWelcome(o);
    help(o);
    while (1) {
      prompt(o);
      switch (parse(i)) {
      case NGame::badCmd: badCmd(i, o); break;
      case NGame::newCmd: newCmd(i, o); break;
      case NGame::pieceCmd: pieceCmd(i, o); break;
      case NGame::moveCmd: 
        try {
          moveCmd(i, o);
        }
        catch(NException::CWin&) {
          printWinner(o);
          resetGame();
          getRoot().getCurrBoard()->draw(0, o);
        }
        break;
      case NGame::drawCmd: drawCmd(i, o); break;
      case NGame::dumpCmd: dumpCmd(i, o); break;
      case NGame::quitCmd: quitCmd(i, o); break;
      case NGame::helpCmd: helpCmd(i, o); break;
      case NGame::promptCmd: promptCmd(i, o); break;
      case NGame::buildCmd: buildCmd(i, o); break;
      case NGame::traceCmd: traceCmd(i, o); break;
      };
    }
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Print promot to the user.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::prompt(std::ostream& o)
  {
    o << getPrompt() << " ";
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Applied quit command.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::quitCmd(std::istream& i, std::ostream& o)
    throw (NException::CQuit)
  {
    throw NException::CQuit();
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Printed an error on bad command.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::badCmd(std::istream& i, std::ostream& o)
  {
    o << "*** ERROR: Unrecognized command." << std::endl;
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Resetted the game.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::newCmd(std::istream& i, std::ostream& o)
  {
    resetGame();
    getRoot().getCurrBoard()->draw(0, o);
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Let user choose the game piece.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::pieceCmd(std::istream& i, std::ostream& o)
  {
    char piece(0);
    
    do {
      i >> piece;
    } while (piece != 'X' && piece != 'x' && piece != 'o' && piece != 'O');
    
    // update the game piece type.
    switch (piece)
    {
    case 'x':
    case 'X':
      getRoot().updateIdentity(NGame::min); break;
    case 'o':
    case 'O':
      getRoot().updateIdentity(NGame::max); break;
    default:
      assert(0);
    };
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Draw the board.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::drawCmd(std::istream& i, std::ostream& o)
  {
    getRoot().getCurrBoard()->draw(0, o);
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Dump the contents of the memory.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::dumpCmd(std::istream& i, std::ostream& o)
  {
    std::string buf;
    i >> buf;
    const int depth = atoi(buf.c_str());
    getRoot().getCurrBoard()->dump(0, o, depth);
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Printed help message command.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::helpCmd(std::istream& i, std::ostream& o)
  {
    help(o);
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Print help message.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::help(std::ostream& o) const
  {
    o << "  " << NGame::c_newCmd;
    o << " - Start a new game." << std::endl;

    o << "  " << NGame::c_pieceCmd << " <x|o>";
    o << " - Choose your play piece." << std::endl;

    o << "  " << NGame::c_moveCmd << " <row> <col>";
    o << " - Execute your move." << std::endl;

    o << "  " << NGame::c_promptCmd << " <string>";
    o << " - Change prompt" << std::endl;

    o << "  " << NGame::c_buildCmd << " <minmax|alphabeta>";
    o << " - Build game tree." << std::endl;

    o << "  " << NGame::c_traceCmd << " <on|off>";
    o << " - Set trace." << std::endl;

    o << "  " << NGame::c_dumpCmd << " <depth>";
    o << " - Debug dump." << std::endl;

    o << "  " << NGame::c_drawCmd;
    o << " - Draw current board." << std::endl;

    o << "  " << NGame::c_quitCmd;
    o << " - Quit game." << std::endl;

    o << "  " << NGame::c_helpCmd;
    o << " - This help" << std::endl;
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Changed the prompt.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::promptCmd(std::istream& i, std::ostream& o)
  {
    std::string buf;
    i >> buf;
    
    getPrompt() = buf;
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Built the minmax/alpha-beta tree..
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::buildCmd(std::istream& i, std::ostream& o)
  {
    if (getRoot().getCurrBoard()->identity() == NGame::empty) {
      o << "*** ERROR: Game piece not yet set!" << std::endl;
      return;
    }
    
    std::string buf;
    i >> buf;

    if (buf == NGame::c_minmax) {
      buildMinMaxTree();
    } else if (buf == NGame::c_alphabeta) {
      buildAlphaBetaTree();
    } else {
      o << "*** ERROR: Unrecognized build option!" << std::endl;
    }
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Turned trace on or off.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::traceCmd(std::istream& i, std::ostream& o)
  {
    std::string buf;
    i >> buf;

    if (buf == NGame::c_on) {
      setTrace(true);
    } else if (buf == NGame::c_off) {
      setTrace(false);
    } else {
      o << "*** ERROR: Unrecognized trace option!" << std::endl;
    }
  }

  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Parsed the command.
  ////////////////////////////////////////////////////////////////////////////
  const enum NGame::commandE 
    CTextInterface::parse(std::istream& i) const
  {
    std::string buf;
    i >> buf;
    
    if (buf == NGame::c_moveCmd) {
      return NGame::moveCmd;
    } else if (buf == NGame::c_drawCmd) {
      return NGame::drawCmd;
    } else if (buf == NGame::c_dumpCmd) {
      return NGame::dumpCmd;
    } else if (buf == NGame::c_pieceCmd) {
      return NGame::pieceCmd;
    } else if (buf == NGame::c_newCmd) {
      return NGame::newCmd;
    } else if (buf == NGame::c_quitCmd) {
      return NGame::quitCmd;
    } else if (buf == NGame::c_helpCmd) {
      return NGame::helpCmd;
    } else if (buf == NGame::c_promptCmd) {
      return NGame::promptCmd;
    } else if (buf == NGame::c_buildCmd) {
      return NGame::buildCmd;
    } else if (buf == NGame::c_traceCmd) {
      return NGame::traceCmd;
    }
    
    return NGame::badCmd;
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Resetted the game.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::resetGame()
  {
    getRoot().resetGame();
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Applied the trace functionality.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::trace(const int indent, std::ostream& o,
    int deep) const
  {
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // Preconditions : None.
  // Postconditions: Dumped the memory contents in human readable form.
  ////////////////////////////////////////////////////////////////////////////
  void 
    CTextInterface::dump(const int indent, std::ostream& o,
    int deep) const
  {
    int indent0(indent);
    int indent1(indent0 + NUtil::c_indentIncr);
    
    o << NUtil::indent(indent0) 
      << "TEXT_INTERFACE {" << std::endl;
    getRoot().dump(indent1, o, deep);
    o << NUtil::indent(indent0) << "}" << std::endl;
  }
}