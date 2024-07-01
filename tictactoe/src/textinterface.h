#ifndef __TEXTINTERFACE_H
#define __TEXTINTERFACE_H

//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 Rohit Priyadarshi. This source code is the sole 
// property of Rohit Priyadarshi. Any unauthorized copying, distribution
// and use can only be done with written permission from the author.
//////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "rootnode.h"
#include "usedentry.h"
#include "quit.h"

namespace NUserInterface {  
  class CTextInterface {
  public: // functions
    explicit CTextInterface(NGame::CRootNode& root);
    ~CTextInterface();
    
    // const access functions.
    const NGame::CRootNode& getRoot() const;
    const std::string& getPrompt() const;
    const bool getTrace() const;
    
    // build functions.
    void buildMinMaxTree() throw (std::bad_alloc);
    void buildAlphaBetaTree() throw (std::bad_alloc);
    
    // user inteface
    void printWelcome(std::ostream& o) const;
    void printWinner(std::ostream& o) const;
    void printUsedEntry(std::ostream& o) const;
    void automaticMove(std::ostream& o) throw (NException::CWin);
    void manualMove(std::istream& i, std::ostream& o)
      throw (NException::CFindFailed, NException::CWin);
    void help(std::ostream& o) const;
    
    // command center
    void prompt(std::ostream& o);
    const enum NGame::commandE parse(std::istream& i) const;
    void command(std::istream& i, std::ostream& o) throw (NException::CQuit);
    void quitCmd(std::istream& i, std::ostream& o) throw (NException::CQuit);
    void moveCmd(std::istream& i, std::ostream& o) throw (NException::CWin);
    void badCmd(std::istream& i, std::ostream& o);
    void newCmd(std::istream& i, std::ostream& o);
    void pieceCmd(std::istream& i, std::ostream& o);
    void drawCmd(std::istream& i, std::ostream& o);
    void dumpCmd(std::istream& i, std::ostream& o);
    void helpCmd(std::istream& i, std::ostream& o);
    void promptCmd(std::istream& i, std::ostream& o);
    void buildCmd(std::istream& i, std::ostream& o);
    void traceCmd(std::istream& i, std::ostream& o);
    
    // reset data
    void resetGame();
    
    // debug interface
    virtual void trace(const int indent, std::ostream& o, 
      int deep = 0) const;
    virtual void dump(const int indent, std::ostream& o, 
      int deep = 0) const;
    
  private: // functions
    // non-const access functions.
    NGame::CRootNode& getRoot();
    std::string& getPrompt();
    void setTrace(bool value);
    
  private: // data
    NGame::CRootNode& m_root;
    static std::string ms_prompt;
    static bool ms_trace;
  };
  
  inline const NGame::CRootNode& 
    CTextInterface::getRoot() const
  {
    return m_root;
  }
  
  inline const std::string& 
    CTextInterface::getPrompt() const
  {
    return ms_prompt;
  }
  
  inline const bool 
    CTextInterface::getTrace() const
  {
    return ms_trace;
  }
  
  inline NGame::CRootNode& 
    CTextInterface::getRoot()
  {
    return m_root;
  }
  
  inline std::string& 
    CTextInterface::getPrompt()
  {
    return ms_prompt;
  }

  inline void 
    CTextInterface::setTrace(bool value)
  {
    ms_trace = value;
  }
}

#endif // __TEXTINTERFACE_H
