// ex: ts=4 sw=4 ft=cpp et indentexpr=
/**
 * \file
 * \brief Boolean Expressions parser - unit tests
 * \author Dmitriy Kargapolov
 * \since 23 December 2023
 */

#include "ast_io.hpp"
#include "be.hpp"
#include "test_utils.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( parser_tests )

BOOST_AUTO_TEST_CASE( simple_test )
{
    auto on  = add_var("on",  var_type::boolean); auto On  = Exp(on);
    auto off = add_var("off", var_type::boolean); auto Off = Exp(off);

    auto width = add_var("width", var_type::integer);
    auto user = add_var("user", var_type::string);
    auto segments = add_var("segments", var_type::integers);
    auto nodes = add_var("nodes", var_type::strings);

    CHECK_PARSE("on", On)
    CHECK_PARSE("true", True)
    CHECK_PARSE("false", False)

    CHECK_PARSE("segments is null", IsNull(segments))
    CHECK_PARSE("segments is not null", NotNull(segments))
    CHECK_PARSE("segments is empty", Empty(segments))

    CHECK_PARSE("on is not null", NotNull(on))
    CHECK_PARSE("off is null", IsNull(off))

    CHECK_PARSE("not off", Neg(Off))
    CHECK_PARSE("not off is null", Neg(IsNull(off)))
    CHECK_PARSE("not off is not null", Neg(NotNull(off)))

    CHECK_PARSE("true and false", AND(True, False))
    CHECK_PARSE("not off is not null and not off is null", AND(
        Neg( NotNull(off) ), Neg( IsNull(off) )
    ))
    CHECK_PARSE("true and not off is not null and not off is null", AND(
        True, Neg( NotNull(off) ), Neg( IsNull(off) )
    ))

    CHECK_PARSE("on and off or true and false", OR(AND(On, Off), AND(True, False)))
    CHECK_PARSE("on and (off or true) and false", AND(On, OR(Off, True), False))
    CHECK_PARSE("on and not (off or true) and false", AND(On, Neg(OR(Off, True)), False))
    CHECK_PARSE("on && not (off || true) and false", AND(On, Neg(OR(Off, True)), False))

    CHECK_PARSE("width is null", IsNull(width))
    CHECK_PARSE("width > 5", NumCmp(width, CompOp::Gt, 5))
    CHECK_PARSE("5 < width", NumCmp(width, CompOp::Gt, 5))
    CHECK_PARSE("width <= 0.55e1", NumCmp(width, CompOp::Le, 5.5))
    CHECK_PARSE("1.7 > width", NumCmp(width, CompOp::Lt, 1.7))

    CHECK_PARSE("\"lalala\" <> user", StrCmp(user, CompOp::Ne, "lalala"))
    CHECK_PARSE("user = 'pepepe'", StrCmp(user, CompOp::Eq, "pepepe"))

    CHECK_PARSE("123 in segments", InSet(123, segments))
    CHECK_PARSE("width not in (1, 2, 3)", NotInSet(width, {1, 2, 3}))
    CHECK_PARSE("'xoxoxo' in nodes", InSet(s("xoxoxo"), nodes))
    CHECK_PARSE("user not in ('you', 'me')", NotInSet(user, {s("you"), s("me")}))

    CHECK_PARSE("segments one of (1, 2, 3)", OneOf(segments, {1, 2, 3}))
    CHECK_PARSE("segments all of (1, 2, 3)", AllOf(segments, {1, 2, 3}))
    CHECK_PARSE("segments none of (1, 2, 3)", NoneOf(segments, {1, 2, 3}))

    CHECK_PARSE("nodes one of ('abc', 'xyz', '123')", OneOf(nodes, {s("abc"), s("xyz"), s("123")}))
    CHECK_PARSE("nodes all of ('abc', 'xyz', '123')", AllOf(nodes, {s("abc"), s("xyz"), s("123")}))
    CHECK_PARSE("nodes none of ('abc', 'xyz', '123')", NoneOf(nodes, {s("abc"), s("xyz"), s("123")}))
}

BOOST_AUTO_TEST_SUITE_END()
