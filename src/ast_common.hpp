// ex: ts=4 sw=4 ft=cpp et indentexpr=
/**
 * \file
 * \brief Boolean Expressions parser - AST common stuff
 * \author Dmitriy Kargapolov
 * \since 27 December 2023
 */
#pragma once

namespace lexen { namespace ast {

struct VarIdx {
    int index;
    VarIdx(int i = 0) : index(i) {}
    void inc() { ++index; }
};

inline bool operator==(const VarIdx& a, const VarIdx& b) {
    return a.index == b.index;
}

} } // lexen::ast
