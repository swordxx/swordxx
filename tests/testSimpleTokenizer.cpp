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

#include <swordxx/SimpleTokenizer.h>

#include <cassert>
#include <string>
#include <vector>
#include "TestAssert.h"


using Result = std::vector<std::string>;

template <
    swordxx::SplitBehavior splitBehavior =
            swordxx::SimpleTokenizer<>::splitBehavior>
struct TestTokenizer {

    using Tokenizer = swordxx::SimpleTokenizer<splitBehavior>;

    template <typename Input, typename Splitter>
    static Result test(Input && input, Splitter && splitter)
    {
        Result r;
        for (auto const & token
             : Tokenizer::tokenize(std::forward<Input>(input),
                                   std::forward<Splitter>(splitter)))
            r.emplace_back(token);
        return r;
    }
};

template <swordxx::SplitBehavior splitBehavior, typename Splitter>
void test(std::size_t /* level */,
          Splitter splitter,
          std::string const & prefix,
          Result const & expectPrefix)
{
    for (auto const c : prefix)
        std::cout << static_cast<int>(c) << ' ';
    std::cout << std::endl;

    using TT = TestTokenizer<splitBehavior>;
    TEST_ASSERT(TT::test(prefix, static_cast<Splitter const &>(splitter))
                == expectPrefix);
    TEST_ASSERT(TT::test(prefix, Splitter(splitter)) == expectPrefix);
    using SV = std::string_view;
    TEST_ASSERT(TT::test(SV(prefix), static_cast<Splitter const &>(splitter))
                == expectPrefix);
    TEST_ASSERT(TT::test(SV(prefix), Splitter(splitter)) == expectPrefix);
    if (prefix.find('\0') == std::string::npos) {
        TEST_ASSERT(TT::test(prefix.c_str(),
                             static_cast<Splitter const &>(splitter))
                    == expectPrefix);
        TEST_ASSERT(TT::test(prefix.c_str(), Splitter(splitter))
                    == expectPrefix);
    }
}

template <swordxx::SplitBehavior splitBehavior,
          typename Splitter,
          typename ... Args>
void test(std::size_t level,
          Splitter && splitter,
          std::string prefix,
          Result expectPrefix,
          std::string_view next,
          Args && ... args)
{
    if (level)
        prefix += splitter;
    ++level;

    if constexpr (splitBehavior == swordxx::SplitBehavior::KeepEmptyTokens) {
        expectPrefix.emplace_back();
        assert(expectPrefix.back().empty());
        test<splitBehavior>(level, splitter, prefix, expectPrefix,
                            std::forward<Args>(args)...);
        expectPrefix.back().assign(1u, next.front());
    } else {
        static_assert(splitBehavior
                      == swordxx::SplitBehavior::SkipEmptyTokens, "");
        test<splitBehavior>(level, splitter, prefix, expectPrefix,
                            std::forward<Args>(args)...);
        expectPrefix.emplace_back(1u, next.front());
    }
    assert(expectPrefix.back().size() == 1u);
    test<splitBehavior>(level, splitter, prefix + next.front(), expectPrefix,
                        std::forward<Args>(args)...);

    expectPrefix.back().assign(next);
    assert(expectPrefix.back().size() == next.size());
    test<splitBehavior>(level, splitter, prefix + std::string(next),
                        expectPrefix, std::forward<Args>(args)...);
}

template <swordxx::SplitBehavior splitBehavior, typename Splitter>
void test_(Splitter && splitter) {
    Result const r;
    if constexpr (splitBehavior == swordxx::SplitBehavior::KeepEmptyTokens) {
        test<splitBehavior>(0u, splitter, "", Result{""});
    } else {
        static_assert(splitBehavior
                      == swordxx::SplitBehavior::SkipEmptyTokens, "");
        test<splitBehavior>(0u, splitter, "", r);
    }
    test<splitBehavior>(0u, splitter, "", r, "123");
    test<splitBehavior>(0u, splitter, "", r, "123", "234");
    test<splitBehavior>(0u, splitter, "", r, "123", "234", "345");
    test<splitBehavior>(0u, splitter, "", r, "123", "234", "345", "456");
    test<splitBehavior>(0u, splitter, "", r, "123", "234", "345", "456", "567");
}

template <typename Splitter>
void test(Splitter && s) {
    test_<swordxx::SplitBehavior::KeepEmptyTokens>(s);
    test_<swordxx::SplitBehavior::SkipEmptyTokens>(s);
}

int main() {
    test(' ');
    test('%');
    test('|');
    test('\n');
    test('\0');

    test(" ");
    test("%");
    test("|");
    test("\n");
    test("yo");
    test("bug");

    using namespace std::literals::string_literals;
    test(" "s);
    test("%"s);
    test("|"s);
    test("\n"s);
    test("\0"s);
    test("yo"s);
    test("bug"s);

    using namespace std::literals::string_view_literals;
    test(" "sv);
    test("%"sv);
    test("|"sv);
    test("\n"sv);
    test("\0"sv);
    test("yo"sv);
    test("bug"sv);
}
