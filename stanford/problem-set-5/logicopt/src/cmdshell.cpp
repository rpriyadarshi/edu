/*
 * cmdshell.cpp
 *
 *  Created on: Feb 21, 2009
 *      Author: rohit
 */

#include "cmdshell.hpp"
#include "logicopt.hpp"
#include <stdlib.h>

namespace opt {

CmdShell::CmdShell() :
	m_cmdRead("read_design"),
	m_cmdWrite("write_design"),
	m_cmdWriteRobdd("write_robdd"),
	m_cmdCheck("check_design"),
	m_cmdSimplify("simplify"),
	m_cmdCleanup("clean_design"),
	m_cmdHelp("help"),
	m_cmdExit("exit"),
	m_cmdQuit("quit"),
	m_cmdPwd("pwd"),
	m_cmdLs("ls"),
	m_logicOpt(0)
{
	logicOpt(createLogicOpt());
}

CmdShell::~CmdShell() {
	delete logicOpt();
}

LogicOpt*
CmdShell::createLogicOpt() {
	return new LogicOpt();
}

int
CmdShell::run(std::istream& i, std::ostream& o) {
	o << "*****************************************" << std::endl;
	o << "* Welcome to the SIMPLE-LOGIC-OPTIMIZER *" << std::endl;
	o << "*****************************************" << std::endl;
	while (i) {
		o << "LogicOpt> ";
		std::string cmd;
		i >> cmd;
		if (cmd == cmdRead()) {
			runCmdRead(i, o);
		}
		else if (cmd == cmdWrite()) {
			runCmdWrite(i, o);
		}
		else if (cmd == cmdWriteRobdd()) {
			runCmdWriteRobdd(i, o);
		}
		else if (cmd == cmdCheck()) {
			runCmdCheck(i, o);
		}
		else if (cmd == cmdSimplify()) {
			runCmdSimplify(i, o);
		}
		else if (cmd == cmdCleanup()) {
			runCmdCleanup(i, o);
		}
		else if (cmd == cmdPwd() || cmd == cmdLs()) {
			system(cmd.c_str());
		}
		else if (cmd == cmdExit() || cmd == cmdQuit()) {
			o << "Goodbye!" << std::endl;
			return 0;
		}
		else if (cmd == cmdHelp()) {
			help(o);
		}
		else {
			o << "Unknown Command \"" << cmd << "\"" << std::endl;
			o << "But here is what you can use:" << std::endl;
			help(o);
		}
	}
	return 0;
}

int
CmdShell::runCmdRead(std::istream& i, std::ostream& o) {
	std::string f;
	i >> f;
	o << "RUNNING: " << cmdRead() << " <" << f << ">" << std::endl;
	logicOpt()->readDesign(f);
	return 0;
}

int
CmdShell::runCmdWrite(std::istream& i, std::ostream& o) {
	std::string f;
	i >> f;
	o << "RUNNING: " << cmdWrite() << " <" << f << ">" << std::endl;
	logicOpt()->writeDesign(f);
	return 0;
}

int
CmdShell::runCmdWriteRobdd(std::istream& i, std::ostream& o) {
	std::string f;
	i >> f;
	o << "RUNNING: " << cmdWriteRobdd() << " <" << f << ">" << std::endl;
	logicOpt()->writeRobdd(f);
	return 0;
}

int
CmdShell::runCmdCheck(std::istream& i, std::ostream& o) {
	std::string f;
	i >> f;
	o << "RUNNING: " << cmdCheck() << " <" << f << ">" << std::endl;
	logicOpt()->checkDesign(f);
	return 0;
}

int
CmdShell::runCmdSimplify(std::istream& i, std::ostream& o) {
	o << "RUNNING: " << cmdSimplify() << std::endl;
	logicOpt()->simplify();
	return 0;
}

int
CmdShell::runCmdCleanup(std::istream& i, std::ostream& o) {
	o << "RUNNING: " << cmdCleanup() << std::endl;
	logicOpt()->cleanup();
	return 0;
}

void
CmdShell::help(std::ostream& o) {
	o << cmdRead();
	o << " <file-name>" << std::endl;
	o << "	The read design command reads the specified text file containing the " << std::endl;
	o << "	gate level design and creates a run time model in memory" << std::endl;
	o << std::endl;
	o << cmdWrite();
	o << " <file-name>" << std::endl;
	o << "	The write design command writes the current netlist in memory to a file. " << std::endl;
	o << std::endl;
	o << cmdWriteRobdd();
	o << " <file-name>" << std::endl;
	o << "	The write robdd command writes the current robdd in memory to a file. " << std::endl;
	o << std::endl;
	o << cmdCheck();
	o << " <file-name>" << std::endl;
	o << "	The check design command checks the netlist for such errors as dangling " << std::endl;
	o << "	nets, unconnected pins and the presence of combinational loops. " << std::endl;
	o << "	The command will print warnings indicating the problem and the " << std::endl;
	o << "	location of the problem. " << std::endl;
	o << std::endl;
	o << cmdSimplify() << std::endl;
	o << "	(a) Constructs the BDD for each driver pin in the design. If the BDD for " << std::endl;
	o << "	    a driver pin shows that it is a constant function, then the pins " << std::endl;
	o << "	    driven by the driver pins are all disconnected from the driver pin " << std::endl;
	o << "	    and are connected to the appropriate constant pin. " << std::endl;
	o << "	(c') Constants must be propagated. " << std::endl;
	o << std::endl;
	o << cmdCleanup() << std::endl;
	o << "	(b) Any gates whose outputs are not connected must be deleted " << std::endl;
	o << "	    (recursively). " << std::endl;
	o << "	(c') Gates that are not needed should be eliminated. " << std::endl;
	o << std::endl;
	o << cmdHelp() << std::endl;
	o << "	This help message " << std::endl;
}

}
