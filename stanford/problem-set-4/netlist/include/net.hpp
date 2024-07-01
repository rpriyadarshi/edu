/*
 * net.hpp
 *
 *  Created on: Feb 16, 2009
 *      Author: rohit
 */

#ifndef NET_HPP_
#define NET_HPP_

#include <string>
#include <vector>
#include <deque>
#include <iostream>

namespace nl {
class Pin;
class Port;

const std::string N_OneNet("1'b1");
const std::string N_ZeroNet("1'b0");

typedef std::deque<Pin*> _PinQueue;

class Net {
public: // typedefs
	typedef std::vector<Pin*> _Pins;
	typedef std::pair<std::string, Net*> _Pair;

public: // Constructors/Destructors
	Net() : m_input(0) {}
	explicit Net(const std::string& n) : m_name(n), m_input(0) {}
	~Net() {}

public: // Accessors
	const std::string& name() const { return m_name; }
	const _Pins& outputs() const { return m_outputs; }
	const Pin* input() const { return m_input; }

public: // Utility
	_Pair pair() { return _Pair(name(), this); }
	void connect(Pin* p);
	void connect(Pin* p, bool is_driver);
	void connect(Port* po, Pin::Type t);
	void disconnect(Pin* p);
	void import(Net* np);
	bool robdd(bdd::Manager& m, _PinQueue& _q);
	bool requeue(_PinQueue& _q);
	bool cleanup();

public: // Writers
	std::ostream& wire(std::ostream& o) const;
	std::ostream& instance(std::ostream& o) const;
	std::istream& instance(std::istream& i);

public: // Checks
	bool valid() const { return !name().empty(); }
	bool valid() { return !name().empty(); }
	bool isOne() const;
	bool isZero() const;
	bool isOneStr() const { return name() == N_OneNet; }
	bool isZeroStr() const { return name() == N_ZeroNet; }
	bool isConstant() const { return isOne()||isZero(); }
	bool isConstantStr() const { return isOneStr()||isZeroStr(); }
	bool check(std::ostream& o) const;
	bool loops(std::ostream& o) const;

private: // Accessors
	std::string& name() { return m_name; }
	_Pins& outputs() { return m_outputs; }
	Pin* input() { return m_input; }
	void input(Pin* p) {
		if (p && input()) {
			std::cerr << "ERROR: Multidriver creation attempt net (\"";
			std::cerr << name() << "\") pin (\"";
			input()->instance(std::cerr);
			std::cerr << "\") and pin (\"";
			p->instance(std::cerr);
			std::cerr << "\")" << std::endl;
		}
		m_input = p;
	}

private: // Data
	std::string	m_name;
	Pin*		m_input;
	_Pins		m_outputs;
};
}


#endif /* NET_HPP_ */
