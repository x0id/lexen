// ex: ts=4 sw=4 ft=cpp et indentexpr=
/**
 * \file
 * \brief Boolean Expressions parser - rules definition
 * \author Dmitriy Kargapolov
 * \since 18 December 2023
 */
#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include "ast.hpp"
#include "be.hpp"

BOOST_FUSION_ADAPT_STRUCT(lexen::ast::UnaryExpr, var, op)
BOOST_FUSION_ADAPT_STRUCT(lexen::ast::Conjunction, items)
BOOST_FUSION_ADAPT_STRUCT(lexen::ast::Disjunction, items)
BOOST_FUSION_ADAPT_STRUCT(lexen::ast::Negation, expr)

namespace lexen { namespace parser {

namespace x3 = boost::spirit::x3;

using x3::_attr;
using x3::_val;
using x3::attr;
using x3::char_;
using x3::int_;
using x3::real_parser;
using x3::strict_real_policies;
using x3::lit;
using x3::lexeme;
using x3::eoi;

auto bool_const_def =
    lit("true")  >> attr(true)
  | lit("false") >> attr(false)
;

auto is_null_def = var >> (
    lit("is null")     >> attr(ast::UnaryOp::IsNull)
  | lit("is not null") >> attr(ast::UnaryOp::IsNotNull)
);

auto is_empty_def = list_var >> "is empty" >> attr(ast::UnaryOp::IsEmpty);

#define VAR ( [] (auto& ctx) { _val(ctx).var = std::move(_attr(ctx)); } )
#define VAL ( [] (auto& ctx) { _val(ctx).val = std::move(_attr(ctx)); } )
#define CMP ( [] (auto& ctx) { _val(ctx).cmp = std::move(_attr(ctx)); } )
#define PMC ( [] (auto& ctx) { _val(ctx).cmp = std::move(mirror(_attr(ctx))); } )

inline ast::CompOp mirror(ast::CompOp x) {
    switch (x) {
        case ast::CompOp::Gt: return ast::CompOp::Lt;
        case ast::CompOp::Ge: return ast::CompOp::Le;
        case ast::CompOp::Lt: return ast::CompOp::Gt;
        case ast::CompOp::Le: return ast::CompOp::Ge;
        case ast::CompOp::Eq: return ast::CompOp::Eq;
        case ast::CompOp::Ne: return ast::CompOp::Ne;
    }
    BOOST_ASSERT_MSG(false, "can't mirror unsupported op");
}

auto eqne_def =
    "="  >> attr(ast::CompOp::Eq)
  | "<>" >> attr(ast::CompOp::Ne)
;

auto cmp_def =
    eqne
  | ">=" >> attr(ast::CompOp::Ge)
  | ">"  >> attr(ast::CompOp::Gt)
  | "<=" >> attr(ast::CompOp::Le)
  | "<"  >> attr(ast::CompOp::Lt)
;

static real_parser<double, strict_real_policies<double>> strict_double;

auto num_comp_def =
    num_var [VAR] >> cmp [CMP] >> (strict_double | int_) [VAL]
  | (strict_double | int_) [VAL] >> cmp [PMC] >> num_var [VAR]
;

auto str_val_def =
    lexeme[ '"' >> *(char_ -  '"') >>  '"']
  | lexeme['\'' >> *(char_ - '\'') >> '\'']
;

auto str_comp_def =
    str_var [VAR] >> eqne [CMP] >> str_val [VAL]
  | str_val [VAL] >> eqne [PMC] >> str_var [VAR]
;

auto int_list_def = "(" >> int_ % "," >> ")";
auto str_list_def = "(" >> str_val % "," >> ")";

#define A(N) boost::fusion::at_c<(N)>(_attr(ctx))
#define VALxSET(T) ([] (auto& ctx) { _val(ctx) = ast::ValInSet<T>(A(0), A(1), A(2)); })
#define VARxSET(T) ([] (auto& ctx) { _val(ctx) = ast::VarInSet<T>(A(0), A(1), A(2)); })
#define SETxSET(T) ([] (auto& ctx) { _val(ctx) = ast::VarVsSet<T>(A(0), A(1), A(2)); })

auto set_op_def =
     "in"     >> attr(ast::SetOp::In   )
  |  "not in" >> attr(ast::SetOp::NotIn)
;

auto set_expr_def =
    (int_    >> set_op >> int_list_var) [VALxSET(int)]
  | (int_var >> set_op >> int_list    ) [VARxSET(int)]
  | (str_val >> set_op >> str_list_var) [VALxSET(std::string)]
  | (str_var >> set_op >> str_list    ) [VARxSET(std::string)]
;

auto list_op_def =
     "one of" >> attr(ast::ListOp::OneOf )
  |  "all of" >> attr(ast::ListOp::AllOf )
  | "none of" >> attr(ast::ListOp::NoneOf)
;

auto list_expr_def =
    (int_list_var >> list_op >> int_list) [SETxSET(int)]
  | (str_list_var >> list_op >> str_list) [SETxSET(std::string)]
;

auto predicate_def =
    is_null
  | is_empty
  | num_comp
  | str_comp
  | set_expr
  | list_expr
  | bool_const
  | bool_var
#ifdef PREDICATE_EXTENSION_RULE
  | PREDICATE_EXTENSION_RULE
#endif
;

auto expression_def = "(" >> or_expr >> ")" | predicate;

auto negation_def = "not" >> expression;

auto factor_def = negation | expression;

auto conjunct_def = factor % ( lit("&&") | "and" );

auto reduce = [] (auto& ctx) {
    if (_attr(ctx).items.size() == 1) {
        _val(ctx) = std::move(_attr(ctx).items[0]);
    } else {
        _val(ctx) = std::move(_attr(ctx));
    }
};

auto and_expr_def = conjunct [reduce];

auto disjunct_def = and_expr % ( lit("||") | "or" );

auto or_expr_def = disjunct [reduce];

auto be_def = or_expr >> eoi;

BOOST_SPIRIT_DEFINE(
    bool_const, str_val, is_null, is_empty, eqne, cmp, num_comp, str_comp,
    int_list, str_list, set_op, set_expr, list_op, list_expr, predicate,
    negation, factor, conjunct, disjunct, and_expr, or_expr, expression, be
)

} // lexen::parser

namespace {
ast::VarIdx idx(1);
}

ast::VarIdx add_var(const std::string& name, var_type type) {
    switch (type) {
    case var_type::boolean:
        parser::bool_var.add(name, idx);
        break;
    case var_type::integer:
        parser::int_var.add(name, idx);
        parser::num_var.add(name, idx);
        break;
    case var_type::realnum:
        parser::num_var.add(name, idx);
        break;
    case var_type::string:
        parser::str_var.add(name, idx);
        break;
    case var_type::integers:
        parser::int_list_var.add(name, idx);
        parser::list_var.add(name, idx);
        break;
    case var_type::strings:
        parser::str_list_var.add(name, idx);
        parser::list_var.add(name, idx);
        break;
    }
    auto ret = idx;
    parser::var.add(name, idx);
    idx.inc();
    return ret;
}

bool parse_str(const std::string& str, ast::Expression& v) {
    return boost::spirit::x3::phrase_parse(str.begin(), str.end(), lexen::parser::be, boost::spirit::x3::space, v);
}

} // lexen
