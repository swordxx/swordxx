/******************************************************************************
 *
 * Copyright Jaak Ristioja
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#include <swordxx/XmlBuilder.h>

#include <sstream>
#include <swordxx/utilstr.h>
#include <type_traits>
#include <utility>
#include "TestAssert.h"


namespace {

using namespace std::string_view_literals;
using X = swordxx::XmlBuilder;

static_assert(std::is_default_constructible_v<X>, "");
static_assert(std::is_copy_constructible_v<X>, "");
static_assert(std::is_nothrow_move_constructible_v<X>, "");
static_assert(std::is_constructible_v<X, std::string_view>, "");
static_assert(std::is_nothrow_destructible_v<X>, "");
static_assert(std::is_copy_assignable_v<X>, "");
static_assert(std::is_nothrow_move_assignable_v<X>, "");

#define D(...) std::declval<__VA_ARGS__>()
#define TEST_NOEXCEPT(neg,...) static_assert(neg noexcept(__VA_ARGS__), "");
#define TEST_RET(t,...) \
    static_assert(std::is_same_v<t,decltype(__VA_ARGS__)>, "");

TEST_RET(bool, D(X &).inContent());
TEST_RET(bool, D(X const &).inContent());
TEST_NOEXCEPT(,D(X &).inContent());
TEST_NOEXCEPT(,D(X const &).inContent());

TEST_RET(bool, D(X &).isComplete());
TEST_RET(bool, D(X const &).isComplete());
TEST_NOEXCEPT(,D(X &).isComplete());
TEST_NOEXCEPT(,D(X const &).isComplete());

TEST_RET(X &, D(X &).e(D(std::string_view)));
TEST_RET(X &, D(X &).a(D(std::string_view), D(std::string_view)));
TEST_RET(X &, D(X &).c(D(std::string_view)));
TEST_RET(X &, D(X &).up());
TEST_RET(X &, D(X &).finish());
TEST_RET(std::string::size_type, D(X &).predictedSizeToFinish());
TEST_NOEXCEPT(,D(X &).predictedSizeToFinish());
TEST_RET(std::string::size_type, D(X const &).predictedSizeToFinish());
TEST_NOEXCEPT(,D(X const &).predictedSizeToFinish());

template <bool CharNoExcept, bool StringViewNoExcept>
struct Nop {
    void operator()(char) noexcept(CharNoExcept);
    void operator()(std::string_view) noexcept(StringViewNoExcept);
};
TEST_RET(void, D(X &).finishTo(D(Nop<true, true>)));
TEST_RET(void, D(X &).finishTo(D(Nop<true, false>)));
TEST_RET(void, D(X &).finishTo(D(Nop<false, true>)));
TEST_RET(void, D(X &).finishTo(D(Nop<false, false>)));
TEST_NOEXCEPT(,D(X &).finishTo(D(Nop<true, true>)));
TEST_NOEXCEPT(!,D(X &).finishTo(D(Nop<true, false>)));
TEST_NOEXCEPT(!,D(X &).finishTo(D(Nop<false, true>)));
TEST_NOEXCEPT(!,D(X &).finishTo(D(Nop<false, false>)));

TEST_RET(std::string const &, D(X const &).asString());
TEST_NOEXCEPT(,D(X const &).asString());
TEST_RET(std::string, D(X const &).toString());


#define TESTCASES \
    T((X())); \
    T(X("a"sv)); \
    T(X().e("a"sv)); \
    T(X("a"sv).e("b"sv)); \
    T(X("a"sv).e("b"sv).e("c"sv)); \
    T(X("a"sv).a("x"sv, "y"sv)); \
    T(X().e("a"sv).a("x"sv, "y"sv)); \
    T(X("a"sv).e("b"sv).a("x"sv, "y"sv)); \
    T(X("a"sv).e("b"sv).e("c"sv).a("x"sv, "y"sv)); \
    T(X().c("z<&>z"sv)); \
    T(X("a"sv).c("z<&>z"sv)); \
    T(X().e("a"sv).c("z<&>z"sv)); \
    T(X("a"sv).e("b"sv).c("z<&>z"sv)); \
    T(X("a"sv).e("b"sv).e("c"sv).c("z<&>z"sv)); \
    T(X("a"sv).a("x"sv, "y"sv).c("z<&>z"sv)); \
    T(X().e("a"sv).a("x"sv, "y"sv).c("z<&>z"sv)); \
    T(X("a"sv).e("b"sv).a("x"sv, "y"sv).c("z<&>z"sv)); \
    T(X("a"sv).e("b"sv).e("c"sv).a("x"sv, "y"sv).c("z<&>z"sv)); \
    T(X().up()); \
    T(X("a"sv).up()); \
    T(X().e("a"sv).up()); \
    T(X("a"sv).e("b"sv).up()); \
    T(X("a"sv).e("b"sv).e("c"sv).up()); \
    T(X("a"sv).a("x"sv, "y"sv).up()); \
    T(X().e("a"sv).a("x"sv, "y"sv).up()); \
    T(X("a"sv).e("b"sv).a("x"sv, "y"sv).up()); \
    T(X("a"sv).e("b"sv).e("c"sv).a("x"sv, "y"sv).up()); \
    T(X().c("z<&>z"sv).up()); \
    T(X("a"sv).c("z<&>z"sv).up()); \
    T(X().e("a"sv).c("z<&>z"sv).up()); \
    T(X("a"sv).e("b"sv).c("z<&>z"sv).up()); \
    T(X("a"sv).e("b"sv).e("c"sv).c("z<&>z"sv).up()); \
    T(X("a"sv).a("x"sv, "y"sv).c("z<&>z"sv).up()); \
    T(X().e("a"sv).a("x"sv, "y"sv).c("z<&>z"sv).up()); \
    T(X("a"sv).e("b"sv).a("x"sv, "y"sv).c("z<&>z"sv).up()); \
    T(X("a"sv).e("b"sv).e("c"sv).a("x"sv, "y"sv).c("z<&>z"sv).up());

X const empty;

bool identical(X const & a, X const & b) noexcept {
    return (a.inContent() == b.inContent())
           && (a.isComplete() == b.isComplete())
           && (a.predictedSizeToFinish() == b.predictedSizeToFinish())
           && (a.asString() == b.asString())
           && (a.toString() == b.toString());
}

void testCopyAssignment(X const & original, X copiedTo) {
    X const copiedFrom(original);
    TEST_ASSERT(identical(copiedFrom, original));
    X & r = (copiedTo = copiedFrom);
    TEST_ASSERT(std::addressof(r) == &copiedTo);
    TEST_ASSERT(identical(copiedFrom, original));
    TEST_ASSERT(identical(copiedTo, copiedFrom));
}

void testMoveAssignment(X const & original, X movedTo) {
    X movedFrom(original);
    TEST_ASSERT(identical(movedFrom, original));
    X & r = (movedTo = std::move(movedFrom));
    TEST_ASSERT(std::addressof(r) == &movedTo);
    TEST_ASSERT(identical(movedFrom, empty));
    TEST_ASSERT(identical(movedTo, original));
}

void testCopyAndMove(X const original, X const unusedCopyOfOriginal) {
    TEST_ASSERT(identical(original, unusedCopyOfOriginal));

    { // Move construction
        X movedFrom(original);
        TEST_ASSERT(identical(movedFrom, original));
        X movedTo(std::move(movedFrom));
        TEST_ASSERT(identical(movedFrom, empty));
        TEST_ASSERT(identical(movedTo, original));
    }
    { // Copy construction
        X const copiedFrom(original);
        TEST_ASSERT(identical(copiedFrom, original));
        X copiedTo(copiedFrom);
        TEST_ASSERT(identical(copiedFrom, original));
        TEST_ASSERT(identical(copiedTo, copiedFrom));
    } \
    // Move assignment
    #define T(...) testMoveAssignment(original, __VA_ARGS__)
    TESTCASES
    #undef T

    // Copy assignment
    #define T(...) testCopyAssignment(original, __VA_ARGS__)
    TESTCASES
    #undef T

    // Check that original has not changed
    TEST_ASSERT(identical(original, unusedCopyOfOriginal));
}

} // anonymous namespace

int main() {
    { // Default construction
        TEST_ASSERT(X().inContent());
        TEST_ASSERT(X().isComplete());
        TEST_ASSERT(X().asString().empty());
        TEST_ASSERT(X().toString().empty());
    }

    /** Move and copy constructors and assignment operators and
        predictedSizeToFinish(), inContent(), isComplete() and finishTo(): */
    #define T(...) \
        do { \
            testCopyAndMove(__VA_ARGS__, __VA_ARGS__); \
            { \
                X x(__VA_ARGS__); \
                auto const p(x.predictedSizeToFinish()); \
                auto const as(x.asString()); \
                auto const s(x.toString()); \
                TEST_ASSERT(swordxx::startsWith(s, as)); \
                TEST_ASSERT(p == (s.size() - as.size())); \
                std::string finishedTo; \
                x.finishTo( \
                    [&finishedTo](auto && v) \
                    { finishedTo += std::forward<decltype(v)>(v); }); \
                { \
                    std::string finishedTo2; \
                    x.finishTo(finishedTo2); \
                    TEST_ASSERT(finishedTo == finishedTo2); \
                }{ \
                    std::ostringstream oss; \
                    x.finishTo(static_cast<std::ostream &>(oss)); \
                    TEST_ASSERT(finishedTo == oss.str()); \
                } \
                x.finish(); \
                auto const fp(x.predictedSizeToFinish()); \
                auto const & fas(x.asString()); \
                auto const fs(x.toString()); \
                TEST_ASSERT(fas == fs); \
                TEST_ASSERT(as + finishedTo == fs); \
                TEST_ASSERT(fp == 0u); \
            }{ /* Test #1 for inContent(), test #1 for isComplete(): */ \
                X x(__VA_ARGS__); \
                x.e("haha"sv); \
                TEST_ASSERT(!x.inContent()); \
                TEST_ASSERT(!x.isComplete()); \
            }{ /* Test #2 for inContent(): */ \
                X x(__VA_ARGS__); \
                x.c("asdf"sv); \
                TEST_ASSERT(x.inContent()); \
            }{ /* Test #3 for inContent(): */ \
                X x(__VA_ARGS__); \
                x.up(); \
                TEST_ASSERT(x.inContent()); \
            }{ /* Test #4 for inContent(), test #2 for isComplete(): */ \
                X x(__VA_ARGS__); \
                x.finish(); \
                TEST_ASSERT(x.inContent()); \
                TEST_ASSERT(x.isComplete()); \
            } \
        } while (false)
    TESTCASES
    #undef T

    { // Argument constructor
        TEST_ASSERT(!X("a"sv).inContent());
        TEST_ASSERT(!X("a"sv).isComplete());
        TEST_ASSERT(X("a"sv).asString() == "<a"sv);
        TEST_ASSERT(X("a"sv).toString() == "<a/>"sv);
        TEST_ASSERT(X("a"sv).predictedSizeToFinish() == 2u);
        TEST_ASSERT(!X("asdf"sv).inContent());
        TEST_ASSERT(!X("asdf"sv).isComplete());
        TEST_ASSERT(X("asdf"sv).asString() == "<asdf"sv);
        TEST_ASSERT(X("asdf"sv).toString() == "<asdf/>"sv);
        TEST_ASSERT(X("asdf"sv).predictedSizeToFinish() == 2u);
    }

    { // .e()
        X x;
        TEST_ASSERT(std::addressof(x.e("a"sv)) == std::addressof(x));
        TEST_ASSERT(!x.inContent());
        TEST_ASSERT(!x.isComplete());
        TEST_ASSERT(x.asString() == "<a");
        TEST_ASSERT(x.predictedSizeToFinish() == 2u);
        TEST_ASSERT(x.toString() == "<a/>");
        x.c(""sv);
        TEST_ASSERT(x.inContent());
        TEST_ASSERT(x.asString() == "<a>");
        TEST_ASSERT(x.predictedSizeToFinish() == 4u);
        x.e("b"sv);
        TEST_ASSERT(!x.inContent());
        TEST_ASSERT(!x.isComplete());
        TEST_ASSERT(x.asString() == "<a><b");
        TEST_ASSERT(x.predictedSizeToFinish() == 6u);
        TEST_ASSERT(x.toString() == "<a><b/></a>");
    }{
        X x("a"sv);
        TEST_ASSERT(std::addressof(x.e("b"sv)) == std::addressof(x));
        x.a("x"sv, "y"sv);
        X y(x);
        TEST_ASSERT(x.asString() == "<a><b x=\"y\""sv);
        try {
            x.e(std::string_view("", std::size_t(-1)));
            TEST_ASSERT(false && "Expected exception but nothing was thrown!");
        } catch (std::bad_alloc const &) {
            TEST_ASSERT(identical(x, y));
        } catch (std::length_error const &) {
            TEST_ASSERT(identical(x, y));
        } catch (...) {
            TEST_ASSERT(false && "Unexpected exception thrown!");
        }
    }

    { // .c()
        X x("a");
        TEST_ASSERT(std::addressof(x.a("x"sv, "y<&>\"'y"sv)) == std::addressof(x));
        TEST_ASSERT(!x.inContent());
        TEST_ASSERT(!x.isComplete());
        TEST_ASSERT(x.asString() == "<a x=\"y<&>&quot;'y\"");
        TEST_ASSERT(x.predictedSizeToFinish() == 2u);
        TEST_ASSERT(x.toString() == "<a x=\"y<&>&quot;'y\"/>");
        TEST_ASSERT(std::addressof(x.a("z"sv, "w\"w"sv)) == std::addressof(x));
        TEST_ASSERT(!x.inContent());
        TEST_ASSERT(!x.isComplete());
        TEST_ASSERT(x.asString() == "<a x=\"y<&>&quot;'y\" z=\'w\"w\'");
        TEST_ASSERT(x.predictedSizeToFinish() == 2u);
        TEST_ASSERT(x.toString() == "<a x=\"y<&>&quot;'y\" z=\'w\"w\'/>");
        X y(x);
        try {
            /* This test relies on the fact that the attribute name is directly
               appended to the XmlBuilder::m_contents std::string. */
            x.a(std::string_view("", std::size_t(-1)), "z"sv);
            TEST_ASSERT(false && "Expected exception but nothing was thrown!");
        } catch (std::bad_alloc const &) {
            TEST_ASSERT(identical(x, y));
        } catch (std::length_error const &) {
            TEST_ASSERT(identical(x, y));
        } catch (...) {
            TEST_ASSERT(false && "Unexpected exception thrown!");
        }
        /* Unfortunately, testing the appending of the attribute value throwing
           an exception is a not so trivial task, because the algorithm first
           traverses the value to find any quotes it has to escape. Therefore
           no test for this. */
    }

    { // .c()
        X x;
        TEST_ASSERT(std::addressof(x.c("asdf"sv)) == std::addressof(x));
        TEST_ASSERT(x.inContent());
        TEST_ASSERT(x.isComplete());
        TEST_ASSERT(x.predictedSizeToFinish() == 0u);
        TEST_ASSERT(x.asString() == "asdf"sv);
        TEST_ASSERT(x.toString() == "asdf"sv);
    }{
        X x("a"sv);
        TEST_ASSERT(std::addressof(x.c("asdf"sv)) == std::addressof(x));
        TEST_ASSERT(x.inContent());
        TEST_ASSERT(!x.isComplete());
        TEST_ASSERT(x.predictedSizeToFinish() == 4u);
        TEST_ASSERT(x.asString() == "<a>asdf"sv);
        TEST_ASSERT(x.toString() == "<a>asdf</a>"sv);
        TEST_ASSERT(X().c("asdf<asdf&asdf>asdf"sv).asString()
                    == "asdf&lt;asdf&amp;asdf&gt;asdf"sv);
    }{
        X x;
        TEST_ASSERT(std::addressof(x.c("asdf<asdf&asdf>asdf"sv))
                    == std::addressof(x));
        TEST_ASSERT(x.inContent());
        TEST_ASSERT(x.isComplete());
        TEST_ASSERT(x.predictedSizeToFinish() == 0u);
        TEST_ASSERT(x.asString() == "asdf&lt;asdf&amp;asdf&gt;asdf"sv);
        TEST_ASSERT(x.toString() == "asdf&lt;asdf&amp;asdf&gt;asdf"sv);
    }
    /* Unfortunately, testing the appending of the value throwing an exception
       is a not so trivial task, because the algorithm first traverses the
       value to find anything it has to escape. Therefore no test for this. */

    { // .up()
        X x("a"sv);
        x.e("b"sv).e("c");
        TEST_ASSERT(std::addressof(x.up()) == std::addressof(x));
        TEST_ASSERT(x.inContent());
        TEST_ASSERT(!x.isComplete());
        TEST_ASSERT(x.predictedSizeToFinish() == 8u);
        TEST_ASSERT(x.asString() == "<a><b><c/>"sv);
        TEST_ASSERT(x.toString() == "<a><b><c/></b></a>"sv);
        TEST_ASSERT(std::addressof(x.up()) == std::addressof(x));
        TEST_ASSERT(x.inContent());
        TEST_ASSERT(!x.isComplete());
        TEST_ASSERT(x.predictedSizeToFinish() == 4u);
        TEST_ASSERT(x.asString() == "<a><b><c/></b>"sv);
        TEST_ASSERT(x.toString() == "<a><b><c/></b></a>"sv);
        TEST_ASSERT(std::addressof(x.up()) == std::addressof(x));
        TEST_ASSERT(x.inContent());
        TEST_ASSERT(x.isComplete());
        TEST_ASSERT(x.predictedSizeToFinish() == 0u);
        TEST_ASSERT(x.asString() == "<a><b><c/></b></a>"sv);
        TEST_ASSERT(x.toString() == "<a><b><c/></b></a>"sv);
        TEST_ASSERT(std::addressof(x.up()) == std::addressof(x));
        TEST_ASSERT(x.inContent());
        TEST_ASSERT(x.isComplete());
        TEST_ASSERT(x.predictedSizeToFinish() == 0u);
        TEST_ASSERT(x.asString() == "<a><b><c/></b></a>"sv);
        TEST_ASSERT(x.toString() == "<a><b><c/></b></a>"sv);
        // Tests for exceptions would be too difficult to implement, skipping.
    }

    { // .finish()
        X x("a"sv);
        x.e("b"sv).e("c");
        TEST_ASSERT(std::addressof(x.finish()) == std::addressof(x));
        TEST_ASSERT(x.inContent());
        TEST_ASSERT(x.isComplete());
        TEST_ASSERT(x.predictedSizeToFinish() == 0u);
        TEST_ASSERT(x.asString() == "<a><b><c/></b></a>"sv);
        TEST_ASSERT(x.toString() == "<a><b><c/></b></a>"sv);
        // Tests for exceptions would be too difficult to implement, skipping.
    }
}
