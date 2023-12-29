// ex: ts=4 sw=4 ft=cpp et indentexpr=
/**
 * \file
 * \brief Boolean Expressions parser - AST to string
 * \author Dmitriy Kargapolov
 * \since 25 December 2023
 */
#pragma once

#include "ast.hpp"
#include "ast_common_io.hpp"

namespace lexen { namespace ast {

namespace x3 = boost::spirit::x3;

inline std::ostream& operator<<(std::ostream& os, const BoolVal& v) {
    return os << std::boolalpha << v.value;
}

inline std::ostream& operator<<(std::ostream& os, UnaryOp op) {
    switch (op) {
        case UnaryOp::IsNull:    return os << "is-null";
        case UnaryOp::IsNotNull: return os << "is-not-null";
        case UnaryOp::IsEmpty:   return os << "is-empty";
    }
    assert(false);
}

inline std::ostream& operator<<(std::ostream& os, CompOp op) {
    switch (op) {
        case CompOp::Gt: return os << ">";
        case CompOp::Ge: return os << ">=";
        case CompOp::Lt: return os << "<";
        case CompOp::Le: return os << "<=";
        case CompOp::Eq: return os << "==";
        case CompOp::Ne: return os << "!=";
    }
    assert(false);
}

inline std::ostream& operator<<(std::ostream& os, const NumVal& v) {
    // boost::apply_visitor([&os] (auto x) { os << x; }, v);
    struct print_visitor : boost::static_visitor<void> {
        print_visitor(std::ostream& o) : os(o) {}
        void operator()(int x) const { os << "int(" << x << ")"; }
        void operator()(double x) const { os << "double(" << x << ")"; }
        std::ostream& os;
    };
    boost::apply_visitor(print_visitor(os), v);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const NumComp& x) {
    return os << x.var << " " << x.cmp << " " << x.val;
}

inline std::ostream& operator<<(std::ostream& os, const StrComp& x) {
    return os << x.var << " " << x.cmp << " " << x.val;
}

inline std::ostream& operator<<(std::ostream& os, const UnaryExpr& x) {
    return os << x.op << "(" << x.var << ")";
}

inline std::ostream& operator<<(std::ostream& os, SetOp op) {
    switch (op) {
        case SetOp::In:    return os << "in";
        case SetOp::NotIn: return os << "not in";
    }
    assert(false);
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const ValInSet<T>& v) {
    return os << v.val << " " << v.op << " " << v.set;
}

template<typename T>
void print_vector(std::ostream& os, const T& v, const char *op, bool is_or);

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const VarInSet<T>& v) {
    os << v.var << " " << v.op << " ";
    print_vector(os, v.set, ", ", true);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const SetExpr& v) {
    boost::apply_visitor([&os] (auto x) { os << x; }, v);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, ListOp op) {
    switch (op) {
        case ListOp::OneOf:  return os << "one of";
        case ListOp::AllOf:  return os << "all of";
        case ListOp::NoneOf: return os << "none of";
    }
    assert(false);
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const VarVsSet<T>& v) {
    os << v.var << " " << v.op << " ";
    print_vector(os, v.set, ", ", true);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ListExpr& v) {
    boost::apply_visitor([&os] (auto x) { os << x; }, v);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Conjunction& v) {
    os << "C( ";
    print_vector(os, v.items, " and ", false);
    os << " )";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Disjunction& v) {
    os << "D( ";
    print_vector(os, v.items, " or ", true);
    os << " )";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Expression& expr);

inline std::ostream& operator<<(std::ostream& os, const Negation& v) {
    return os << "not " << v.expr;
}

inline std::ostream& operator<<(std::ostream& os, const Expression& expr) {
    boost::apply_visitor([&os] (auto x) { os << x; }, expr);
    return os;
}

template<typename T>
inline void print_vector(std::ostream& os, const T& v, const char *op, bool is_or) {
    if (v.empty()) return;
    if (v.size() > 1 && is_or) os << "(";
    os << v[0];
    for (auto it = v.begin() + 1; it < v.end(); ++it) { os << op << *it; }
    if (v.size() > 1 && is_or) os << ")";
}

} } // lexen::ast
