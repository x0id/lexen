// ex: ts=4 sw=4 ft=cpp et indentexpr=
/**
 * \file
 * \brief Boolean Expressions parser - AST common stuff to string
 * \author Dmitriy Kargapolov
 * \since 28 December 2023
 */
#pragma once

#include "ast_common.hpp"
#include <iostream>

namespace lexen { namespace ast {

inline std::ostream& operator<<(std::ostream& os, const VarIdx& v) {
    return os << "var<" << v.index << ">";
}

} } // lexen::ast
