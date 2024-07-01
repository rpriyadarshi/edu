/*
 * logicopt.cpp
 *
 *  Created on: Feb 21, 2009
 *      Author: rohit
 */

// Netlist includes
#include "gate.hpp"
#include "net.hpp"
#include "netlist.hpp"
#include "pin.hpp"
#include "port.hpp"

// ROBDD includes
#include "manager.hpp"

#include "logicopt.hpp"

#include <fstream>

namespace opt {

nl::Netlist*
LogicOpt::createNetlist() {
	return new nl::Netlist();
}

nl::Netlist*
LogicOpt::createNetlist(const std::string& n) {
	nl::Netlist* nlst = new nl::Netlist(n);
	netlists().insert(nlst->pair());
	return nlst;
}

bdd::Manager*
LogicOpt::createRobdd() {
	return new bdd::Manager();
}

// The check design command checks the netlist for such errors as dangling
// nets, unconnected pins and the presence of combinational loops.
// The command should print warnings indicating the problem and the
// location of the problem.
bool
LogicOpt::checkDesign(const std::string& file) const {
	bool baddesign = false;
	std::ofstream o(file.c_str());
	// Netlists
	for (_Netlists::const_iterator itrN = netlists().begin();
		itrN != netlists().end(); itrN++) {
		const nl::Netlist* nlst = ((*itrN).second);
		baddesign |= nlst->check(o);
		baddesign |= nlst->loops(o);
	}
	return baddesign;
}

// (a) Using the shared reduced BDD class you designed in HW3, construct
//     the BDD for each driver pin in the design. If the BDD for
//     a driver pin shows that it is a constant function, then the pins
//     driven by the driver pins are all disconnected from the driver pin
//     and are connected to the appropriate constant pin.
// (c') Constants must be propagated.
bool
LogicOpt::simplify() {
	robdd(createRobdd());
	// Netlists
	for (_Netlists::iterator itrN = netlists().begin();
		itrN != netlists().end(); itrN++) {
		((*itrN).second)->robdd(*robdd());
	}
	return false;
}

// (b) Any gates whose outputs are not connected must be deleted
//     (recursively).
// (c') Gates that are not needed should be eliminated.
bool
LogicOpt::cleanup() {
	robdd(createRobdd());
	// Netlists
	for (_Netlists::iterator itrN = netlists().begin();
		itrN != netlists().end(); itrN++) {
		((*itrN).second)->cleanup();
	}
	return false;
}

// The read design command reads the specified text file containing the
// gate level design and creates a run time model in memory using the
// netlist class you designed in HW4.
bool
LogicOpt::readDesign(const std::string& file) {
	std::ifstream i(file.c_str());
	verilog(i);
	return false;
}

std::istream&
LogicOpt::verilog(std::istream& i) {
	unsigned int idx = 0;
	while (i) {
		nl::Netlist* nlst = createNetlist();
		nlst->verilog(i, idx);
		if (nlst->valid()) {
			netlists().insert(nlst->pair());
		}
		else {
			delete nlst;
		}
	}
	return i;
}

// The write design command writes the current netlist in memory to a file.
bool
LogicOpt::writeDesign(const std::string& file) const {
	std::ofstream o(file.c_str());
	verilog(o);
	return false;
}

// The write robdd command writes the current robdd in memory to a file.
bool
LogicOpt::writeRobdd(const std::string& file) const {
	std::ofstream o(file.c_str());
	robdd(o);
	if (!robdd())
		return false;
	o << *robdd();
	return true;
}

std::ostream&
LogicOpt::verilog(std::ostream& o) const {
	// Netlists
	for (_Netlists::const_iterator itrN = netlists().begin();
		itrN != netlists().end(); itrN++) {
		((*itrN).second)->verilog(o);
	}
	return o;
}

std::ostream&
LogicOpt::robdd(std::ostream& o) const {
	// Netlists
	for (_Netlists::const_iterator itrN = netlists().begin();
		itrN != netlists().end(); itrN++) {
		((*itrN).second)->robdd(o);
	}
	return o;
}

}
