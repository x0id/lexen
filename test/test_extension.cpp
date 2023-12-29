// ex: ts=4 sw=4 ft=cpp et indentexpr=
/**
 * \file
 * \brief Boolean Expressions parser - extension tests
 * \author Dmitriy Kargapolov
 * \since 27 December 2023
 */

#include "extension_ast_io.hpp"
#include "be.hpp"
#include "test_utils.hpp"

#include <boost/test/unit_test.hpp>

using ext::ast::FitExpr;
using ext::ast::PredicateExtension;

inline Exp Fits(VarIdx var, std::string val) {
    return Exp(PredicateExtension(FitExpr{var, val}));
}

BOOST_AUTO_TEST_SUITE( extension_tests )

BOOST_AUTO_TEST_CASE( extension_parser_test )
{
    auto on = add_var("on",  var_type::boolean); auto On = Exp(on);
    auto user = add_var("user", var_type::string); auto User = Exp(user);

    CHECK_PARSE("on", On)
    CHECK_PARSE("\"lalala\" <> user", StrCmp(user, CompOp::Ne, "lalala"))
    CHECK_PARSE("user = 'pepepe'", StrCmp(user, CompOp::Eq, "pepepe"))
    CHECK_PARSE("? user fits 'pepepe'", Fits(user, "pepepe"))
}

BOOST_AUTO_TEST_SUITE_END()
