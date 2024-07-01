/*
 * boolfunc.hpp
 *
 *  Created on: Feb 7, 2009
 *      Author: rohit rpiyadarshi
 */

#ifndef BOOLFUNC_HPP_
#define BOOLFUNC_HPP_

#include <utility>

namespace bdd {
class NodeBase;

////////////////////////////////////////////////////////////////////////////////
// Boolean function abstraction
////////////////////////////////////////////////////////////////////////////////
class BoolFunc {
public: // Typedefs
	typedef std::pair<const BoolFunc*, const NodeBase*> _Pair;

public: // Constructors/ Destructors
	BoolFunc() : m_topVar(0), m_thenChild(0), m_elseChild(0) {}
	BoolFunc(unsigned int tv, const NodeBase* tc, const NodeBase* ec)
		: m_topVar(tv), m_thenChild(tc), m_elseChild(ec) {}
	~BoolFunc() {}

public: // Accessors
	unsigned int topVar() const { return m_topVar; }
	const NodeBase* thenChild() const { return m_thenChild; }
	const NodeBase* elseChild() const { return m_elseChild; }

public: // Operators
	bool operator==(const BoolFunc& f) const {
		return topVar() == f.topVar() &&		// value compare
			thenChild() == f.thenChild() && 	// pointer compare
			elseChild() == f.elseChild();		// pointer compare
	}
	bool operator!=(const BoolFunc& f) const {
		return topVar() != f.topVar() ||		// value compare
			thenChild() != f.thenChild() || 	// pointer compare
			elseChild() != f.elseChild();		// pointer compare
	}
	bool operator<(const BoolFunc& f) const {
		return ((topVar() < f.topVar()) ||
				((topVar() == f.topVar()) &&
					(thenChild() < f.thenChild())) ||
				((topVar() == f.topVar()) &&
					(thenChild() == f.thenChild()) &&
						(elseChild() < f.elseChild())));
	}
	bool operator>(const BoolFunc& f) const {
		return ((topVar() > f.topVar()) ||
				((topVar() == f.topVar()) &&
					(thenChild() > f.thenChild())) ||
				((topVar() == f.topVar()) &&
					(thenChild() == f.thenChild()) &&
						(elseChild() > f.elseChild())));
	}

public: // Utility
	_Pair pair(const NodeBase* n) const { return _Pair(this, n); }

private: // Accessors
	unsigned int topVar() { return m_topVar; }
	const NodeBase* thenChild() { return m_thenChild; }
	const NodeBase* elseChild() { return m_elseChild; }

private:// Data
	unsigned int 	m_topVar;
	const NodeBase*	m_thenChild;
	const NodeBase*	m_elseChild;
};

////////////////////////////////////////////////////////////////////////////////
// Global operators
////////////////////////////////////////////////////////////////////////////////
#if 0
inline bool
operator==(const BoolFunc& f1, const BoolFunc& f2) {
	return f1.operator==(f2);
}
inline bool
operator<(const BoolFunc& f1, const BoolFunc& f2) {
	return f1.operator<(f2);
}
inline bool
operator>(const BoolFunc& f1, const BoolFunc& f2) {
	return f1.operator>(f2);
}
#endif

////////////////////////////////////////////////////////////////////////////////
// Boolean function tuple
////////////////////////////////////////////////////////////////////////////////
class BoolFuncTuple {
public: // Typedefs
	typedef std::pair<const BoolFuncTuple*, const NodeBase*> _Pair;

public: // Constructors/Destructors
	BoolFuncTuple() : m_f(0), m_g(0), m_h(0) {}
	BoolFuncTuple(const NodeBase* f, const NodeBase* g, const NodeBase* h)
		: m_f(f), m_g(g), m_h(h) {}
	~BoolFuncTuple() {}

public: // Accessors
	const NodeBase* f() const { return m_f; }
	const NodeBase* g() const { return m_g; }
	const NodeBase* h() const { return m_h; }

public: // Operators
	bool operator==(const BoolFuncTuple& t) const {
		return f() == t.f() &&	// pointer compare
			g() == t.g() && 	// pointer compare
			h() == t.h();		// pointer compare
	}
	bool operator!=(const BoolFuncTuple& t) const {
		return f() != t.f() ||	// pointer compare
			g() != t.g() || 	// pointer compare
			h() != t.h();		// pointer compare
	}
	bool operator<(const BoolFuncTuple& t) const {
		return ((f() < t.f()) ||
				(f() == t.f() && g() < t.g()) ||
				(f() == t.f() && g() == t.g() && h() < t.h()));
		//return f() < t.f() || g() < t.g() || h() < t.h();
	}
	bool operator>(const BoolFuncTuple& t) const {
		return ((f() > t.f()) ||
				(f() == t.f() && g() > t.g()) ||
				(f() == t.f() && g() == t.g() && h() > t.h()));
		//return f() > t.f() || g() > t.g() || h() > t.h();
	}

public: // Utility
	_Pair pair(const NodeBase* n) const { return _Pair(this, n); }

private: // Accessors
	const NodeBase* f() { return m_f; }
	const NodeBase* g() { return m_g; }
	const NodeBase* h() { return m_h; }

private: // Data
	const NodeBase*		m_f;
	const NodeBase*		m_g;
	const NodeBase*		m_h;
};

}
#endif /* BOOLFUNC_HPP_ */
