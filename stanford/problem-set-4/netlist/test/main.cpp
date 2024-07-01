/*
 * main.cpp
 *
 *  Created on: Feb 16, 2009
 *      Author: rohit
 */

#include "gate.hpp"
#include "net.hpp"
#include "pin.hpp"
#include "port.hpp"
#include "netlist.hpp"

int
main(int argc, const char** argv) {
	nl::Netlist netlist("test");

	// Gates
	nl::BaseGate* Not = netlist.instantiateGate(nl::BaseGate::Not, "NOT");
	nl::BaseGate* And2 = netlist.instantiateGate(nl::BaseGate::And2, "AND2");
	nl::BaseGate* Or2 = netlist.instantiateGate(nl::BaseGate::Or2, "OR2");
	nl::BaseGate* Zero = netlist.instantiateGate(nl::BaseGate::Zero, "Zero");
	nl::BaseGate* One = netlist.instantiateGate(nl::BaseGate::One, "One");

	// Nets
	nl::Net* n1 = netlist.instantiateNet("n1");
	nl::Net* n2 = netlist.instantiateNet("n2");
	nl::Net* n3 = netlist.instantiateNet("n3");
	nl::Net* n4 = netlist.instantiateNet("n4");
	nl::Net* n5 = netlist.instantiateNet("n5");

	// Connect-up
	nl::BaseGate* g = netlist.findGate("NOT");

	nl::Pin* p = netlist.getOutputPin(g);
	nl::Net* n = netlist.findNet("n1");
	n->connect(p, false);
	p = netlist.getInputPin(g, "i");
	n = netlist.findNet("n1");
	n->connect(p, true);

	// Dump
	netlist.verilog(std::cout);
	return 0;
}
