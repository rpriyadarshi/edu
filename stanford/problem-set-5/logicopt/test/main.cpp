/*
 * main.cpp
 *
 *  Created on: Feb 21, 2009
 *      Author: rohit
 */

#include "logicopt.hpp"
#include "cmdshell.hpp"

int
main(int argc, const char** argv) {
	opt::CmdShell cs;
	cs.run(std::cin, std::cout);
	return 0;
}
