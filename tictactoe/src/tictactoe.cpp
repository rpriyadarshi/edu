//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
// Program: The Tic Tac Toe game.
// 
// Purpose: To let user enjoy playing tic tac toe! 
//        : To implement and learn min-max search algorithm.
//        : To implement and learn alpha-beta pruning.
// 
// Author : Rohit Priyadarshi
// Date   : 01/24/2002
//////////////////////////////////////////////////////////////////////////////

#include <fstream.h>
#include "stdafx.h"
#include "board.h"
#include "boardnode.h"
#include "rootnode.h"
#include "textinterface.h"

//////////////////////////////////////////////////////////////////////////////
// Preconditions : None.
// Postconditions: You have quit the program, or program crashed!
//////////////////////////////////////////////////////////////////////////////
int 
main(int argc, char* argv[])
{
  NGame::CRootNode rn;
  NUserInterface::CTextInterface tui(rn);
  try {
    tui.command(std::cin, std::cout);
  }
  catch (NException::CQuit) {
    return 0;
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Preconditions : None.
// Postconditions: Debug dump of the variables defined in the board class.
//////////////////////////////////////////////////////////////////////////////
void 
test0()
{
  std::cout << "Constant Variables dump..." << std::endl;
  std::cout << "NGame::c_gameSize = " << NGame::c_gameSize << std::endl;
  std::cout << "NGame::c_bitWidth = " << NGame::c_bitWidth << std::endl;
  std::cout << "NGame::c_dataMask = " << NGame::c_dataMask << std::endl;
  std::cout << "NGame::c_filledMask = " << NGame::c_filledMask << std::endl;
  std::cout << "NGame::c_size = " << NGame::c_size << std::endl;
  std::cout << "NGame::c_maxSize = " << NGame::c_maxSize << std::endl;
  std::cout << "NGame::c_dataSize = " << NGame::c_dataSize << std::endl;
  std::cout << "NGame::c_utilSize = " << NGame::c_utilSize << std::endl;
}

//////////////////////////////////////////////////////////////////////////////
// Preconditions : None.
// Postconditions: Exercised the board class.
//////////////////////////////////////////////////////////////////////////////
void
test1()
{
  test0();
  NGame::CBoard b;
  b.dump(0, std::cout);
  
  b.draw(0, std::cout);
  b.updateUtility(b.utility());
  std::cout << "Utility: " << b.getUtility() << std::endl;
  std::cout << "Empty: " 
    << NGame::enum2string(b.isEmpty()) << std::endl;
  std::cout << "Terminal: " 
    << NGame::enum2string(b.terminal()) << std::endl;
  std::cout << std::endl;
  
  b.data(0, 0, NGame::min);
  b.data(0, 1, NGame::max);
  b.data(0, 2, NGame::min);
  
  b.data(1, 0, NGame::max);
  b.data(1, 1, NGame::min);
  b.data(1, 2, NGame::max);
  
  b.data(2, 0, NGame::min);
  b.data(2, 1, NGame::max);
  b.data(2, 2, NGame::min);
  
  b.draw(0, std::cout);
  b.updateUtility(b.utility());
  std::cout << "Utility: " << b.getUtility() << std::endl;
  std::cout << "Empty: " 
    << NGame::enum2string(b.isEmpty()) << std::endl;
  std::cout << "Terminal: " 
    << NGame::enum2string(b.terminal()) << std::endl;
  std::cout << std::endl;
  
  b.dump(0, std::cout);
  b.reset();
  b.dump(0, std::cout);
  b.data(0, 0, NGame::max);
  b.data(0, 1, NGame::max);
  b.data(0, 2, NGame::min);
  
  b.data(1, 0, NGame::min);
  b.data(1, 1, NGame::max);
  b.data(1, 2, NGame::min);
  
  b.data(2, 0, NGame::min);
  b.data(2, 1, NGame::max);
  b.data(2, 2, NGame::min);
  
  b.draw(0, std::cout);
  b.updateUtility(b.utility());
  std::cout << "Utility: " << b.getUtility() << std::endl;
  std::cout << "Empty: " 
    << NGame::enum2string(b.isEmpty()) << std::endl;
  std::cout << "Terminal: " 
    << NGame::enum2string(b.terminal()) << std::endl;
  std::cout << std::endl;
}

//////////////////////////////////////////////////////////////////////////////
// Preconditions : None.
// Postconditions: Exercised the board class.
//////////////////////////////////////////////////////////////////////////////
void 
test2()
{
  NGame::CBoard board(NGame::max);
  board.data(0, 0, NGame::min);
  board.data(0, 1, NGame::max);
  board.data(0, 2, NGame::min);
  
  board.data(1, 0, NGame::max);
  board.data(1, 1, NGame::min);
  board.data(1, 2, NGame::min);
  
  board.data(2, 0, NGame::max);
  board.data(2, 1, NGame::max);
  board.data(2, 2, NGame::min);
  
  board.dump(0, std::cout);
  
  NGame::CRootNode rn;
  
  //  rn.data(0, 0, NGame::max);
  //  rn.data(0, 1, NGame::max);
  //  rn.data(0, 2, NGame::max);
  
  //  rn.data(1, 0, NGame::max);
  //  rn.data(1, 1, NGame::max);
  //  rn.data(1, 2, NGame::max);
  
  //  rn.data(2, 0, NGame::max);
  //  rn.data(2, 1, NGame::max);
  //  rn.data(2, 2, NGame::max);
  
  rn.buildMinMaxTree(NGame::max);
  //  rn.dump(0, std::cout);
  try {
    NGame::CBoardInterface& fboard = rn.find(board);
    fboard.draw(0, std::cout);
  }
  
  catch (NException::CFindFailed) {
    std::cout << "Failed!" << std::endl;
  }
}

