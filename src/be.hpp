// ex: ts=4 sw=4 ft=cpp et indentexpr=
/**
 * \file
 * \brief Boolean Expressions parser - API
 * \author Dmitriy Kargapolov
 * \since 19 December 2023
 */
#pragma once

namespace lexen {

enum class var_type {
    boolean,
    integer,
    realnum,
    string,
    integers,
    strings
};

extern ast::VarIdx add_var(const std::string& name, var_type type);
extern bool parse_str(const std::string& str, ast::Expression& v);

} // lexen
