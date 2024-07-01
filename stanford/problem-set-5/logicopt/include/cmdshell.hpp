/*
 * cmdshell.hpp
 *
 *  Created on: Feb 21, 2009
 *      Author: rohit
 */

#ifndef CMDSHELL_HPP_
#define CMDSHELL_HPP_

#include <string>
#include <iostream>

namespace opt {
class LogicOpt;

class CmdShell {
public: // Constructors/Destructors
	CmdShell();
	~CmdShell();

public: // Accessors
	const std::string& cmdRead() const { return m_cmdRead; }
	const std::string& cmdWrite() const { return m_cmdWrite; }
	const std::string& cmdWriteRobdd() const { return m_cmdWriteRobdd; }
	const std::string& cmdCheck() const { return m_cmdCheck; }
	const std::string& cmdSimplify() const { return m_cmdSimplify; }
	const std::string& cmdCleanup() const { return m_cmdCleanup; }
	const std::string& cmdHelp() const { return m_cmdHelp; }
	const std::string& cmdExit() const { return m_cmdExit; }
	const std::string& cmdQuit() const { return m_cmdQuit; }
	const std::string& cmdPwd() const { return m_cmdPwd; }
	const std::string& cmdLs() const { return m_cmdLs; }
	const LogicOpt* logicOpt() const { return m_logicOpt; }

public: // Execution
	int run(std::istream& i, std::ostream& o);
	int runCmdRead(std::istream& i, std::ostream& o);
	int runCmdWrite(std::istream& i, std::ostream& o);
	int runCmdWriteRobdd(std::istream& i, std::ostream& o);
	int runCmdCheck(std::istream& i, std::ostream& o);
	int runCmdSimplify(std::istream& i, std::ostream& o);
	int runCmdCleanup(std::istream& i, std::ostream& o);

public: // Creation
	LogicOpt* createLogicOpt();

public: // Utility
	void help(std::ostream& o);

private: // Accessors
	LogicOpt* logicOpt() { return m_logicOpt; }
	void logicOpt(LogicOpt* lo) { m_logicOpt = lo; }

private: // Data
	const std::string	m_cmdRead;
	const std::string	m_cmdWrite;
	const std::string	m_cmdWriteRobdd;
	const std::string	m_cmdCheck;
	const std::string	m_cmdSimplify;
	const std::string	m_cmdCleanup;
	const std::string	m_cmdHelp;
	const std::string	m_cmdExit;
	const std::string	m_cmdQuit;
	const std::string	m_cmdPwd;
	const std::string	m_cmdLs;
	LogicOpt*			m_logicOpt;
};
}

#endif /* CMDSHELL_HPP_ */
