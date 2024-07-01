/*
 * logicopt.hpp
 *
 *  Created on: Feb 21, 2009
 *      Author: rohit
 */

#ifndef LOGICOPT_HPP_
#define LOGICOPT_HPP_

#include <string>
#include <iostream>
#include <map>

namespace bdd {
class Manager;
}

namespace nl {
class Netlist;
}

namespace opt {

// Write a simple logic optimization system
class LogicOpt {
private: // Typedefs
	typedef std::map<std::string, nl::Netlist*, std::less<std::string> > _Netlists;

public: // Constructors/Destructors
	LogicOpt() {}
	~LogicOpt() {}

public: // Accessors
	const _Netlists& netlists() const { return m_netlists; }
	const bdd::Manager* robdd() const { return m_robdd; }

public: // Optimization functions
	// The check design command checks the netlist for such errors as dangling
	// nets, unconnected pins and the presence of combinational loops.
	// The command should print warnings indicating the problem and the
	// location of the problem.
	bool checkDesign(const std::string& file) const;
	// (a) Using the shared reduced BDD class you designed in HW3, construct
	//     the BDD for each driver pin in the design. If the BDD for
	//     a driver pin shows that it is a constant function, then the pins
	//     driven by the driver pins are all disconnected from the driver pin
	//     and are connected to the appropriate constant pin.
	// (c') Constants must be propagated.
	bool simplify();
	// (b) Any gates whose outputs are not connected must be deleted
	//     (recursively).
	// (c') Gates that are not needed should be eliminated.
	bool cleanup();

public: // Read/write
	// The read design command reads the specified text file containing the
	// gate level design and creates a run time model in memory using the
	// netlist class you designed in HW4.
	bool readDesign(const std::string& file);
	std::istream& verilog(std::istream& i);
	// The write design command writes the current netlist in memory to a file.
	bool writeDesign(const std::string& file) const;
	bool writeRobdd(const std::string& file) const;
	std::ostream& verilog(std::ostream& o) const;
	std::ostream& robdd(std::ostream& o) const;

public: // Utility
	nl::Netlist* createNetlist();
	nl::Netlist* createNetlist(const std::string& n);
	bdd::Manager* createRobdd();

private: // Accessors
	_Netlists& netlists() { return m_netlists; }
	bdd::Manager* robdd() { return m_robdd; }
	void robdd(bdd::Manager* m) { m_robdd = m; }

private: // Data
	_Netlists		m_netlists;
	bdd::Manager*	m_robdd;
};

}

#endif /* LOGICOPT_HPP_ */
