// ex: ts=4 sw=4 ft=cpp et indentexpr=
/**
 * \file
 * \brief Boolean Expressions parser - AST
 * \author Dmitriy Kargapolov
 * \since 18 December 2023
 */
#pragma once

#include <boost/spirit/home/x3/string/symbols.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include "ast_common.hpp"

namespace lexen { namespace ast {

namespace x3 = boost::spirit::x3;

using Vars = x3::symbols<VarIdx>;

struct BoolVal {
    bool value;
    BoolVal(bool v = false) : value(v) {}
};

inline bool operator==(const BoolVal& a, const BoolVal& b) {
    return a.value == b.value;
}

enum class UnaryOp {
    IsNull,
    IsNotNull,
    IsEmpty
};

struct UnaryExpr {
    UnaryOp op;
    VarIdx var;
};

inline bool operator==(const UnaryExpr& a, const UnaryExpr& b) {
    return a.op == b.op && a.var == b.var;
}

// numeric literal value
struct NumVal : x3::variant<int, double> {
    using base_type::base_type;
    using base_type::operator=;
};

struct num_val_eq_visitor : boost::static_visitor<bool> {
    template<typename T>
    bool operator()(T a, T b) const {
        return a == b;
    }
    template<typename A, typename B>
    bool operator()(const A&, const B&) const {
        return false;
    }
};

inline bool operator==(const NumVal& a, const NumVal& b) {
    return boost::apply_visitor(num_val_eq_visitor(), a, b);
}

// comparison operations
enum class CompOp {
    Gt, Ge, Lt, Le, Eq, Ne
};

// numeric comparison
struct NumComp {
    VarIdx var;
    NumVal val;
    CompOp cmp;
};

inline bool operator==(const NumComp& a, const NumComp& b) {
    return a.var == b.var && a.val == b.val && a.cmp == b.cmp;
}

// string comparison (only Eq and Ne are used)
struct StrComp {
    VarIdx var;
    std::string val;
    CompOp cmp;
};

inline bool operator==(const StrComp& a, const StrComp& b) {
    return a.var == b.var && a.val == b.val && a.cmp == b.cmp;
}

enum class SetOp { In, NotIn };

template<typename T>
struct VarInSet {
    VarInSet() {}
    VarInSet(VarIdx v, SetOp o, const std::vector<T>& s) : var(v), op(o), set(s) {}
    VarIdx var;
    SetOp op;
    std::vector<T> set;
};

template<typename T>
inline bool operator==(const VarInSet<T>& a, const VarInSet<T>& b) {
    return a.var == b.var && a.op == b.op && a.set == b.set;
}

template<typename T>
struct ValInSet {
    ValInSet() {}
    ValInSet(const T& v, SetOp o, VarIdx s) : val(v), op(o), set(s) {}
    T val;
    SetOp op;
    VarIdx set;
};

template<typename T>
inline bool operator==(const ValInSet<T>& a, const ValInSet<T>& b) {
    return a.val == b.val && a.op == b.op && a.set == b.set;
}

struct SetExpr : x3::variant<
    ValInSet<int>,
    ValInSet<std::string>,
    VarInSet<int>,
    VarInSet<std::string>
> {
    using base_type::base_type;
    using base_type::operator=;
};

struct set_expr_eq_visitor : boost::static_visitor<bool> {
    template<typename T>
    bool operator()(const T& a, const T& b) const { return a == b; }
    template<typename A, typename B>
    bool operator()(const A&, const B&) const { return false; }
};

inline bool operator==(const SetExpr& a, const SetExpr& b) {
    return boost::apply_visitor(set_expr_eq_visitor(), a, b);
}

enum class ListOp { OneOf, AllOf, NoneOf };

template<typename T>
struct VarVsSet {
    VarVsSet() {}
    VarVsSet(VarIdx v, ListOp o, const std::vector<T>& s) : var(v), op(o), set(s) {}
    VarIdx var;
    ListOp op;
    std::vector<T> set;
};

template<typename T>
inline bool operator==(const VarVsSet<T>& a, const VarVsSet<T>& b) {
    return a.var == b.var && a.op == b.op && a.set == b.set;
}

struct ListExpr : x3::variant<
    VarVsSet<int>,
    VarVsSet<std::string>
> {
    using base_type::base_type;
    using base_type::operator=;
};

struct list_expr_eq_visitor : boost::static_visitor<bool> {
    template<typename T>
    bool operator()(const T& a, const T& b) const { return a == b; }
    template<typename A, typename B>
    bool operator()(const A&, const B&) const { return false; }
};

inline bool operator==(const ListExpr& a, const ListExpr& b) {
    return boost::apply_visitor(list_expr_eq_visitor(), a, b);
}

struct Conjunction;
struct Disjunction;
struct Negation;

struct Expression : x3::variant<
    BoolVal,
    VarIdx, 
    NumComp,
    StrComp,
    UnaryExpr,
    SetExpr,
    ListExpr,
#ifdef PREDICATE_EXTENSION_AST_TYPE
    PREDICATE_EXTENSION_AST_TYPE,
#endif
    x3::forward_ast<Conjunction>,
    x3::forward_ast<Disjunction>,
    x3::forward_ast<Negation>
> {
    using base_type::base_type;
    using base_type::operator=;
};

struct expr_eq_visitor : boost::static_visitor<bool> {
    template<typename T>
    bool operator()(const T& a, const T& b) const {
        return a == b;
    }
    template<typename A, typename B>
    bool operator()(const A&, const B&) const {
        return false;
    }
};

inline bool operator==(const Expression& a, const Expression& b) {
    return boost::apply_visitor(expr_eq_visitor(), a, b);
}

struct Conjunction {
    std::vector<Expression> items;
};

inline bool operator==(const Conjunction& a, const Conjunction& b) {
    return a.items == b.items;
}

struct Disjunction {
    std::vector<Expression> items;
};

inline bool operator==(const Disjunction& a, const Disjunction& b) {
    return a.items == b.items;
}

struct Negation {
    Expression expr;
};

inline bool operator==(const Negation& a, const Negation& b) {
    return a.expr == b.expr;
}

} } // lexen::ast
