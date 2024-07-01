/*
 * port.hpp
 *
 *  Created on: Feb 16, 2009
 *      Author: rohit
 */

#ifndef PORT_HPP_
#define PORT_HPP_

#include "gate.hpp"

namespace nl {

class Port : public BaseGate {
public: // Typedefs
	typedef std::pair<std::string, Port*> _Pair;

public: // Constructors/Destructors
	explicit Port(Netlist* nlst) :
		BaseGate(nlst), m_index(std::numeric_limits<int>::max()) {}
	Port(Netlist* nlst, unsigned int& idx) :
		BaseGate(nlst), m_index(idx) { idx++;}
	Port(const std::string& n, Netlist* nlst) :
		BaseGate(n, nlst), m_index(std::numeric_limits<int>::max()) {}
	~Port() {}

public: // Utility
	_Pair pair() { return _Pair(name(), this); }
	Pin* connect(Net* n, Pin::Type t);
	bool isPort() { return true; }
	const Pin* pin() const { return _pin(); }

	// ROBDD functions
	unsigned int index() const { return m_index; }
	void index(unsigned int i) { m_index = i; }
	bool robdd(bdd::Manager& m, _PinQueue& _q);
	bool finalrobdd(bdd::Manager& m, _PinQueue& _q);
	bool buildrobdd(bdd::Manager& m);
	bool optimize(bdd::Manager& m, _PinQueue& _q) { return false; }
	bool cleanup() { return false; };

public: // Writers
	std::ostream& instance(std::ostream& o) const;
	std::ostream& declaration(std::ostream& o) const;
	std::ostream& module(std::ostream& o) const;
	std::istream& instance(std::istream& i);
	std::istream& declaration(std::istream& i) const;
	std::ostream& robdd(std::ostream& o) const;

private: // Utility
	Pin* pin() { return const_cast<Pin*>(_pin()); }
	const Pin* _pin() const;

public: // Data
	unsigned int m_index;
};
}

#endif /* PORT_HPP_ */
