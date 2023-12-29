// ex: ts=4 sw=4 ft=cpp et indentexpr=
/**
 * \file
 * \brief Boolean Expressions parser - extension ast print
 * \author Dmitriy Kargapolov
 * \since 25 December 2023
 */
#pragma once

#include "extension_ast.hpp"
#include "ast_common_io.hpp"

namespace ext { namespace ast {

namespace x3 = boost::spirit::x3;

inline std::ostream& operator<<(std::ostream& os, const FitExpr& v) {
    return os << "{" << v.var << " fits " << v.val << "}";
}

inline std::ostream& operator<<(std::ostream& os, const PredicateExtension& v) {
    boost::apply_visitor([&os] (auto x) { os << x; }, v);
    return os;
}

} } // ext::ast

#include "ast_io.hpp"
