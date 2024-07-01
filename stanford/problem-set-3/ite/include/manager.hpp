/*
 * manager.hpp
 *
 *  Created on: Feb 1, 2009
 *      Author: rohit rpiyadarshi
 */

#ifndef MANAGER_HPP_
#define MANAGER_HPP_

#include <map>
#include <iostream>
#include "boolfunc.hpp"

////////////////////////////////////////////////////////////////////////////////
// Unique table:
//   Hash table that stores an element keyed by triple {v, G, H}
//   This means that the hash() = fn(v, G, H) where:
//       v = index of the node
//       G,H = pointers (pointers to what data structure?)
//   Element is the pointer to the node with index v and children G and H
//
// While going bottom up, we would first find G and H and then want to insert
// node v with children G and H. The query in the Unique table should tell us
// if we need do that or not. We would create a new entry only if there is none
// already existing.
//
// Computed table:
//   Hash table that stores keyed by the triple {F, G, H}. At each step of
//   recursion, we check first if the triple {F, G, H} is already computed
//
// BoolFunc is the key!!!
//
////////////////////////////////////////////////////////////////////////////////

namespace bdd {

// Funny namespace needed for GNU. using is a bad way,
// but don't want to use this funny name everywhere.
// using namespace __gnu_cxx;

// Forward declarations
class NodeBase;
class Sink;
class Node;

////////////////////////////////////////////////////////////////////////////////
// Extracted from stl and modified. This change forces that boolfunc.hpp
// be included
////////////////////////////////////////////////////////////////////////////////
template<typename _Tp>
struct Less : public std::binary_function<_Tp, _Tp, bool>
{
	bool operator()(const _Tp& __x, const _Tp& __y) const {
		return (*__x) < (*__y);
	}
};

////////////////////////////////////////////////////////////////////////////////
// Unique table as mentioned in the notes above
// Complete object BoolFunc is specified instead of pointer as it avoids the
// need to manage objects allocated by using new. Hence could needs full
// header boolfunc.hpp inclusion
////////////////////////////////////////////////////////////////////////////////
class UniqueTable {
public: // Typedefs
	typedef std::map<const BoolFunc*, const NodeBase*,
		Less<const BoolFunc*> > _Table;

public: // Constructors/Destructors
	UniqueTable() {}
	~UniqueTable() {}

public: // Accesors
	const _Table& table() const { return m_table; }

public: // Creation functions
	const BoolFunc* createBoolFunc(unsigned int v,
			const NodeBase* t, const NodeBase* e);
	const Sink* createSink(unsigned int v);
	const Node* createNode(unsigned int i,
			const NodeBase* o, const NodeBase* z);

public: // Search functions
	const NodeBase* find(const BoolFunc* bf) const;
	const NodeBase* find(unsigned int v, const NodeBase* t,
			const NodeBase* e) const ;
	const NodeBase* findOrCreateAndInsert(unsigned int v, const NodeBase* t,
			const NodeBase* e);
	const BoolFunc* findFunc(const BoolFunc* bf) const;
	const BoolFunc* findFunc(unsigned int v, const NodeBase* t,
			const NodeBase* e) const ;

public: // Management functions
	void insert(const BoolFunc* bf, const NodeBase* i);
	void insert(unsigned int v, const NodeBase* t, const NodeBase* e,
			const NodeBase* i);

public: // Utility functions
	std::ostream& dump(std::ostream& o) const;

private:// Accessors
	_Table& table() { return m_table; }

private: // Data
	_Table 	m_table;
};

////////////////////////////////////////////////////////////////////////////////
// Computed table representation
////////////////////////////////////////////////////////////////////////////////
class ComputedTable {
public: // Typedefs
	typedef std::map<const BoolFuncTuple*, const NodeBase*,
		Less<const BoolFuncTuple*> > _Table;

public: // Constructors/Destructors
	ComputedTable() {}
	~ComputedTable() {}

public: // Accesors
	const _Table& table() const { return m_table; }

public: // Creation functions
	const BoolFuncTuple* createBoolFuncTuple(const NodeBase* f,
			const NodeBase* g, const NodeBase* h);

public: // Search functions
	const NodeBase* find(const BoolFuncTuple* bf) const;
	const NodeBase* find(const NodeBase* f, const NodeBase* g,
			const NodeBase* h) const ;

public: // Management functions
	void insert(const BoolFuncTuple* bf, const NodeBase* i);
	void insert(const NodeBase* f, const NodeBase* g, const NodeBase* h,
			const NodeBase* i);

public: // Utility functions
	std::ostream& dump(std::ostream& o) const;

private:// Accessors
	_Table& table() { return m_table; }

private: // Data
	_Table 	m_table;
};

////////////////////////////////////////////////////////////////////////////////
// ROBDD Manager that holds shared ROBDDs
////////////////////////////////////////////////////////////////////////////////
class Manager {
public: // Constructors/Destructors
	Manager();
	~Manager();

public: // Accessors
	const Sink* one() const { return m_one; }
	const Sink* zero() const { return m_zero; }
	const UniqueTable& uniqueTable() const { return m_uniqueTable; }
	const ComputedTable& computedTable() const { return m_computedTable; }

public: // Accessors
	const Sink* one() { return m_one; }
	const Sink* zero() { return m_zero; }
	UniqueTable& uniqueTable() { return m_uniqueTable; }
	ComputedTable& computedTable() { return m_computedTable; }

public: // Execution
	unsigned int findTopVar(const NodeBase* f, const NodeBase* g,
			const NodeBase* h) const;
	const NodeBase* cofactorThen(unsigned int topVar, const NodeBase* f);
	const NodeBase* cofactorElse(unsigned int topVar, const NodeBase* f);
	const NodeBase* terminalCase(const NodeBase* f, const NodeBase* g,
			const NodeBase* h) const;
	const NodeBase* ite(const NodeBase* f, const NodeBase* g,
			const NodeBase* h);
	const NodeBase* ite(const NodeBase* f, const NodeBase* g, bool hval);
	const NodeBase* ite(const NodeBase* f, bool gval, const NodeBase* h);
	const NodeBase* ite(const NodeBase* f, bool gval, bool hval);
	const NodeBase* ite(bool fval, const NodeBase* g, const NodeBase* h);
	const NodeBase* ite(bool fval, const NodeBase* g, bool hval);
	const NodeBase* ite(bool fval, bool gval, const NodeBase* h);
	const NodeBase* ite(bool fval, bool gval, bool hval);

public: // Utility functions
	std::ostream& dump(std::ostream& o) const;

public: // Creation functions
	const BoolFunc* createBoolFunc(unsigned int v, const NodeBase* t,
			const NodeBase* e) const;
	const Sink* createSink(unsigned int v) const;
	const Node* createNode(unsigned int i, const NodeBase* o,
			const NodeBase* z) const;

public:// Manual builders
	const bdd::NodeBase* buildVar(unsigned int v, const bdd::NodeBase* o,
			const bdd::NodeBase* z);
	const bdd::NodeBase* buildVar(unsigned int v);
	const bdd::NodeBase* buildAnd(unsigned int i, unsigned int j);
	const bdd::NodeBase* buildOr(unsigned int i, unsigned int j);

public: // Ite builders
	const bdd::NodeBase* var(unsigned int v) { return buildVar(v); }
	const bdd::NodeBase* var(const bdd::NodeBase* v) { return v; }

	// NOT: ite(F, 0, 1)/ite(G, 0, 1)
	const bdd::NodeBase* iteNot(unsigned int i) {
		return ite(var(i), false, true);
	}
	const bdd::NodeBase* iteNot(const bdd::NodeBase* i) {
		return ite(var(i), false, true);
	}

	// AND: ite(F, G, 0)
	const bdd::NodeBase* iteAnd(unsigned int i, unsigned int j) {
		return ite(var(i), var(j), false);
	}
	const bdd::NodeBase* iteAnd(const bdd::NodeBase* i, unsigned int j) {
		return ite(var(i), var(j), false);
	}
	const bdd::NodeBase* iteAnd(unsigned int i, const bdd::NodeBase* j) {
		return ite(var(i), var(j), false);
	}
	const bdd::NodeBase* iteAnd(const bdd::NodeBase* i, const bdd::NodeBase* j) {
		return ite(var(i), var(j), false);
	}

	// OR: ite(F, 1, G)
	const bdd::NodeBase* iteOr(unsigned int i, unsigned int j) {
		return ite(var(i), true, var(j));
	}
	const bdd::NodeBase* iteOr(const bdd::NodeBase* i, unsigned int j) {
		return ite(var(i), true, var(j));
	}
	const bdd::NodeBase* iteOr(unsigned int i, const bdd::NodeBase* j) {
		return ite(var(i), true, var(j));
	}
	const bdd::NodeBase* iteOr(const bdd::NodeBase* i, const bdd::NodeBase* j) {
		return ite(var(i), true, var(j));
	}

	// NOR: ite(F, 0, G')
	const bdd::NodeBase* iteNor(unsigned int i, unsigned int j) {
		return ite(var(i), false, iteNot(j));
	}
	const bdd::NodeBase* iteNor(const bdd::NodeBase* i, unsigned int j) {
		return ite(var(i), false, iteNot(j));
	}
	const bdd::NodeBase* iteNor(unsigned int i, const bdd::NodeBase* j) {
		return ite(var(i), false, iteNot(j));
	}
	const bdd::NodeBase* iteNor(const bdd::NodeBase* i, const bdd::NodeBase* j) {
		return ite(var(i), false, iteNot(j));
	}

	// NAND: ite(F, G', 1)
	const bdd::NodeBase* iteNand(unsigned int i, unsigned int j) {
		return ite(var(i), iteNot(j), true);
	}
	const bdd::NodeBase* iteNand(const bdd::NodeBase* i, unsigned int j) {
		return ite(var(i), iteNot(j), true);
	}
	const bdd::NodeBase* iteNand(unsigned int i, const bdd::NodeBase* j) {
		return ite(var(i), iteNot(j), true);
	}
	const bdd::NodeBase* iteNand(const bdd::NodeBase* i, const bdd::NodeBase* j) {
		return ite(var(i), iteNot(j), true);
	}

	// XOR: ite(F, G', G)
	const bdd::NodeBase* iteXor(unsigned int i, unsigned int j) {
		return ite(var(i), iteNot(j), var(j));
	}
	const bdd::NodeBase* iteXor(const bdd::NodeBase* i, unsigned int j) {
		return ite(var(i), iteNot(j), var(j));
	}
	const bdd::NodeBase* iteXor(unsigned int i, const bdd::NodeBase* j) {
		return ite(var(i), iteNot(j), var(j));
	}
	const bdd::NodeBase* iteXor(const bdd::NodeBase* i, const bdd::NodeBase* j) {
		return ite(var(i), iteNot(j), var(j));
	}

	// XNOR: ite(F, G, G')
	const bdd::NodeBase* iteXnor(unsigned int i, unsigned int j) {
		return ite(var(i), var(j), iteNot(j));
	}
	const bdd::NodeBase* iteXnor(const bdd::NodeBase* i, unsigned int j) {
		return ite(var(i), var(j), iteNot(j));
	}
	const bdd::NodeBase* iteXnor(unsigned int i, const bdd::NodeBase* j) {
		return ite(var(i), var(j), iteNot(j));
	}
	const bdd::NodeBase* iteXnor(const bdd::NodeBase* i, const bdd::NodeBase* j) {
		return ite(var(i), var(j), iteNot(j));
	}

	// F > G: ite(F, G', 0)
	const bdd::NodeBase* iteGt(unsigned int i, unsigned int j) {
		return ite(var(i), iteNot(j),false);
	}
	const bdd::NodeBase* iteGt(const bdd::NodeBase* i, unsigned int j) {
		return ite(var(i), iteNot(j),false);
	}
	const bdd::NodeBase* iteGt(unsigned int i, const bdd::NodeBase* j) {
		return ite(var(i), iteNot(j),false);
	}
	const bdd::NodeBase* iteGt(const bdd::NodeBase* i, const bdd::NodeBase* j) {
		return ite(var(i), iteNot(j),false);
	}

	// F < G: ite(F, 0, G)
	const bdd::NodeBase* iteLt(unsigned int i, unsigned int j) {
		return ite(var(i), false, var(j));
	}
	const bdd::NodeBase* iteLt(const bdd::NodeBase* i, unsigned int j) {
		return ite(var(i), false, var(j));
	}
	const bdd::NodeBase* iteLt(unsigned int i, const bdd::NodeBase* j) {
		return ite(var(i), false, var(j));
	}
	const bdd::NodeBase* iteLt(const bdd::NodeBase* i, const bdd::NodeBase* j) {
		return ite(var(i), false, var(j));
	}

	// F >= G: ite(F, 1, G')
	const bdd::NodeBase* iteGtEq(unsigned int i, unsigned int j) {
		return ite(var(i), true, iteNot(j));
	}
	const bdd::NodeBase* iteGtEq(const bdd::NodeBase* i, unsigned int j) {
		return ite(var(i), true, iteNot(j));
	}
	const bdd::NodeBase* iteGtEq(unsigned int i, const bdd::NodeBase* j) {
		return ite(var(i), true, iteNot(j));
	}
	const bdd::NodeBase* iteGtEq(const bdd::NodeBase* i, const bdd::NodeBase* j) {
		return ite(var(i), true, iteNot(j));
	}

	// F <= G: ite(F, G, 1)
	const bdd::NodeBase* iteLtEq(unsigned int i, unsigned int j) {
		return ite(var(i), var(j), true);
	}
	const bdd::NodeBase* iteLtEq(const bdd::NodeBase* i, unsigned int j) {
		return ite(var(i), var(j), true);
	}
	const bdd::NodeBase* iteLtEq(unsigned int i, const bdd::NodeBase* j) {
		return ite(var(i), var(j), true);
	}
	const bdd::NodeBase* iteLtEq(const bdd::NodeBase* i, const bdd::NodeBase* j) {
		return ite(var(i), var(j), true);
	}

private: // Data
	const Sink*		m_one;
	const Sink*		m_zero;
	UniqueTable		m_uniqueTable;
	ComputedTable	m_computedTable;
};

}

////////////////////////////////////////////////////////////////////////////////
// Global operators
////////////////////////////////////////////////////////////////////////////////

inline std::ostream&
operator<<(std::ostream& o, const bdd::UniqueTable& u) {
	return u.dump(o);
}

inline std::ostream&
operator<<(std::ostream& o, const bdd::ComputedTable& c) {
	return c.dump(o);
}

inline std::ostream&
operator<<(std::ostream& o, const bdd::Manager& m) {
	return m.dump(o);
}

#endif /* MANAGER_HPP_ */
