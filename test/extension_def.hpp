// ex: ts=4 sw=4 ft=cpp et indentexpr=
/**
 * \file
 * \brief Boolean Expressions parser - extension definition
 * \author Dmitriy Kargapolov
 * \since 22 December 2023
 */
#pragma once

#include <boost/fusion/include/adapt_struct.hpp>

#include "extension_ast.hpp"
#include "be_decl.hpp"

namespace ext { namespace parser {
using boost::spirit::x3::rule;
// "root" extension rule tu plug in into the core parser
rule<class predicate_extension, ast::PredicateExtension> predicate_extension = "predicate_extension";
} }

// let core parser know the extension rule name
#define PREDICATE_EXTENSION_RULE ::ext::parser::predicate_extension

BOOST_FUSION_ADAPT_STRUCT(ext::ast::FitExpr, var, val)

namespace ext { namespace parser {

namespace x3 = boost::spirit::x3;

using x3::char_;
using x3::lexeme;
using x3::lit;
using x3::rule;
using lexen::parser::str_var;
using lexen::parser::str_val;

// specific rules comprising the "root" one
rule<class fits, ast::FitExpr> fits = "fits";

auto predicate_extension_def =
    fits
    // | <something else>
;

auto fits_def =
    lit("?") >> str_var >> "fits" >> str_val
;

} } // ext::parser

namespace ext { namespace parser {
BOOST_SPIRIT_DEFINE(
    fits, predicate_extension
)

} }
