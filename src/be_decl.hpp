// ex: ts=4 sw=4 ft=cpp et indentexpr=
/**
 * \file
 * \brief Boolean Expressions parser - rules declaration
 * \author Dmitriy Kargapolov
 * \since 27 December 2023
 */
#pragma once

#include "ast.hpp"

#include <boost/spirit/home/x3.hpp>

namespace lexen { namespace parser {

using boost::spirit::x3::rule;

rule<class bool_const, ast::BoolVal> bool_const = "bool_const";
rule<class str_val, std::string> str_val = "str_val";
rule<class int_list, std::vector<int>> int_list = "int_list";
rule<class str_list, std::vector<std::string>> str_list = "str_list";
rule<class is_null, ast::UnaryExpr> is_null = "is_null";
rule<class is_empty, ast::UnaryExpr> is_empty = "is_empty";
rule<class cmp, ast::CompOp> cmp = "cmp";
rule<class eqne, ast::CompOp> eqne = "eqne";
rule<class num_comp, ast::NumComp> num_comp = "num_comp";
rule<class str_comp, ast::StrComp> str_comp = "str_comp";
rule<class set_op, ast::SetOp> set_op = "set_op";
rule<class set_expr, ast::SetExpr> set_expr = "set_expr";
rule<class list_op, ast::ListOp> list_op = "list_op";
rule<class list_expr, ast::ListExpr> list_expr = "list_expr";
rule<class predicate, ast::Expression> predicate = "predicate";
rule<class negation, ast::Negation> negation = "negation";
rule<class factor, ast::Expression> factor = "factor";
rule<class conjunct, ast::Conjunction> conjunct = "conjunct";
rule<class and_expr, ast::Expression> and_expr = "and_expr";
rule<class disjunct, ast::Disjunction> disjunct = "disjunct";
rule<class or_expr, ast::Expression> or_expr = "or_expr";
rule<class expression, ast::Expression> expression = "expression";
rule<class be, ast::Expression> be = "be";

ast::Vars bool_var;
ast::Vars int_var;
ast::Vars num_var;
ast::Vars str_var;
ast::Vars int_list_var;
ast::Vars str_list_var;
ast::Vars list_var;
ast::Vars var;

} } // lexen::parser
