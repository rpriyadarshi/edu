/*
 * netlist.hpp
 *
 *  Created on: Feb 16, 2009
 *      Author: rohit
 */

#ifndef NETLIST_HPP_
#define NETLIST_HPP_

#include <map>
#include <deque>
#include <vector>
#include <string>
#include <iostream>

namespace bdd {
class Manager;
}

namespace nl {
class BaseGate;
class Net;
class Pin;
class Port;

typedef std::deque<Pin*> _PinQueue;

class Netlist {
private: // Typedefs
	typedef std::vector<Port*> _Ports;
	typedef std::map<std::string, BaseGate*, std::less<std::string> > _Gates;
	typedef std::map<std::string, Net*, std::less<std::string> > _Nets;
	typedef std::pair<std::string, Netlist*> _Pair;

public: // Constructors/Destructors
	Netlist();
	explicit Netlist(const std::string& n);
	~Netlist();

public: // Accessors
	const std::string& name() const { return m_name; }
	const _Ports& ports() const { return m_ports; }
	const _Gates& gates() const { return m_gates; }
	const _Nets& nets() const { return m_nets; }
	const BaseGate* zero() const { return m_zero; }
	const BaseGate* one() const { return m_one; }

public: // Creation functions
	Port* createPort(unsigned int& idx);
	Port* createPort(const std::string& n);
	BaseGate* createGate(BaseGate::Type t);
	BaseGate* createGate(const std::string& n, BaseGate::Type t);
	Net* createNet();
	Net* createNet(Port* p, Pin::Type t);
	Net* createNet(const std::string& n);
	void buildConstants();

public: // Utility
	_Pair pair() { return _Pair(name(), this); }

	// Gate functions
	BaseGate* instantiateGate(BaseGate::Type gt, const std::string& n);
	void deleteGate(const BaseGate* g);
	BaseGate* findGate(const std::string& n);

	// Net functions
	Net* instantiateNet(const std::string& n);
	void deleteNet(const Net* n);
	Net* findNet(const std::string& n);
	Port* findPort(const std::string& n);
	void connectNet(Net* n, Pin* p,	bool is_driver);
	void disconnectNet(Net* n, Pin* p);

	// Pin functions
	Pin* getOutputPin(const BaseGate* g );
	Pin* getInputPin(const BaseGate* g, const std::string& n );
	Pin* findDriverPin(const Net* n);
	unsigned int findNumDrivenPins(const Net* n);
	Pin* findDrivenPin(const Net* n, int pin_num);

	// Propagation aid

	// ROBDD functions
	bool robdd(bdd::Manager& m);
	bool optimize(bdd::Manager& m, _PinQueue& _q, const Pin* pn);
	bool cleanupGates();
	bool cleanupNets();
	bool cleanup();

public: // Writers
	std::ostream& verilog(std::ostream& o) const;
	std::istream& verilog(std::istream& i, unsigned int& idx);
	std::istream& name(std::istream& i);
	std::istream& ports(std::istream& i, unsigned int& idx);
	std::istream& type(std::istream& i, Pin::Type t);
	std::istream& wires(std::istream& i);
	std::istream& assign(std::istream& i);
	std::istream& instances(std::istream& i);
	std::istream& comment(std::istream& i);
	std::ostream& robdd(std::ostream& o) const;
	std::ostream& reasonMsg(std::ostream& o, const Pin* pn, bool constant);

	// Debugging functions
	void printNetlist();

public: // Check validity
	bool valid() const { return !name().empty(); }
	bool valid() { return !name().empty(); }
	bool check(std::ostream& o) const;
	bool loops(std::ostream& o) const;

private: // Accessors
	std::string& name() { return m_name; }
	_Ports& ports() { return m_ports; }
	_Gates& gates() { return m_gates; }
	_Nets& nets() { return m_nets; }
	BaseGate* zero() { return m_zero; }
	BaseGate* one() { return m_one; }
	void zero(BaseGate* z) { m_zero = z; }
	void one(BaseGate* o) { m_one = o; }

private: // Data
	std::string	m_name;
	_Ports		m_ports;
	_Gates		m_gates;
	_Nets		m_nets;
	BaseGate*		m_zero;
	BaseGate*		m_one;
};
}

#endif /* NETLIST_HPP_ */
