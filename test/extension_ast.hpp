// ex: ts=4 sw=4 ft=cpp et indentexpr=
/**
 * \file
 * \brief Boolean Expressions parser - extension ast
 * \author Dmitriy Kargapolov
 * \since 22 December 2023
 */
#pragma once

#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include "ast_common.hpp"

namespace ext { namespace ast {

namespace x3 = boost::spirit::x3;

using lexen::ast::VarIdx;

struct FitExpr {
    VarIdx var;
    std::string val;
};

inline bool operator==(const FitExpr& a, const FitExpr& b) {
    return a.var == b.var && a.val == b.val;
}

struct PredicateExtension : x3::variant<
    FitExpr
> {
    using base_type::base_type;
    using base_type::operator=;
};

struct pred_ex_eq_visitor : boost::static_visitor<bool> {
    template<typename T>
    bool operator()(const T& a, const T& b) const {
        return a == b;
    }
    template<typename A, typename B>
    bool operator()(const A&, const B&) const {
        return false;
    }
};

inline bool operator==(const PredicateExtension& a, const PredicateExtension& b) {
    return boost::apply_visitor(pred_ex_eq_visitor(), a, b);
}

} } // ext::ast

#define PREDICATE_EXTENSION_AST_TYPE ::ext::ast::PredicateExtension
#include "ast.hpp"
