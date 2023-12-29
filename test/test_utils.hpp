// ex: ts=4 sw=4 ft=cpp et indentexpr=
/**
 * \file
 * \brief Boolean Expressions parser - test utils
 * \author Dmitriy Kargapolov
 * \since 27 December 2023
 */
#pragma once

using Exp = lexen::ast::Expression;
using Con = lexen::ast::Conjunction;
using Dis = lexen::ast::Disjunction;
using lexen::var_type;
using lexen::add_var;

inline Exp var_exp(const std::string& var, var_type type) {
    return Exp(add_var(var, type));
}

using lexen::ast::VarIdx;
using lexen::ast::BoolVal;
using lexen::ast::NumVal;
using lexen::ast::CompOp;
using lexen::ast::UnaryOp;
using lexen::ast::UnaryExpr;
using lexen::ast::Negation;
using lexen::ast::NumComp;
using lexen::ast::StrComp;
using lexen::ast::SetOp;
using lexen::ast::ValInSet;
using lexen::ast::VarInSet;
using lexen::ast::SetExpr;
using lexen::ast::ListOp;
using lexen::ast::VarVsSet;
using lexen::ast::ListExpr;

inline Exp Bool(bool x) { return Exp(BoolVal(x)); }
static Exp True = Bool(true);
static Exp False = Bool(false);

template<typename T>
Exp IsNull(T x) { return Exp(UnaryExpr{UnaryOp::IsNull, x}); }

template<typename T>
Exp NotNull(T x) { return Exp(UnaryExpr{UnaryOp::IsNotNull, x}); }

template<typename T>
Exp Empty(T x) { return Exp(UnaryExpr{UnaryOp::IsEmpty, x}); }

inline Exp Neg(Exp x) { return Exp(Negation{x}); }

template<typename T>
Exp NumCmp(VarIdx var, CompOp op, T x) { return Exp(NumComp{var, NumVal{x}, op}); }

template<typename T>
Exp StrCmp(VarIdx var, CompOp op, T x) { return Exp(StrComp{var, std::string(x), op}); }

template<typename T>
Exp InSet(T val, VarIdx var) { return Exp(SetExpr{ValInSet<T>(val, SetOp::In, var)}); }

template<typename T>
Exp NotInSet(T val, VarIdx var) { return Exp(SetExpr{ValInSet<T>(val, SetOp::NotIn, var)}); }

template<typename T>
Exp InSet(VarIdx var, std::initializer_list<T> set) {
    return Exp(SetExpr{VarInSet<T>(var, SetOp::In, std::vector<T>(set))});
}

template<typename T>
Exp NotInSet(VarIdx var, std::initializer_list<T> set) {
    return Exp(SetExpr{VarInSet<T>(var, SetOp::NotIn, std::vector<T>(set))});
}

template<typename T>
Exp OneOf(VarIdx var, std::initializer_list<T> set) {
    return Exp(ListExpr{VarVsSet<T>(var, ListOp::OneOf, std::vector<T>(set))});
}

template<typename T>
Exp AllOf(VarIdx var, std::initializer_list<T> set) {
    return Exp(ListExpr{VarVsSet<T>(var, ListOp::AllOf, std::vector<T>(set))});
}

template<typename T>
Exp NoneOf(VarIdx var, std::initializer_list<T> set) {
    return Exp(ListExpr{VarVsSet<T>(var, ListOp::NoneOf, std::vector<T>(set))});
}

template<typename T>
std::string s(T x) { return x; }

#define AND(...) Exp(Con{{__VA_ARGS__}})
#define OR(...) Exp(Dis{{__VA_ARGS__}})

#define CHECK_PARSE(source, expected) \
    { Exp result; \
      BOOST_REQUIRE(lexen::parse_str(source, result)); \
      BOOST_REQUIRE_EQUAL(expected, result); \
    }
