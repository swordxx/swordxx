#include <swordxx/utilxml.h>

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <swordxx/utilstr.h>
#include <utility>
#include "TestAssert.h"


using namespace std::literals::string_view_literals;

#define TEST_SIGNATURE(...) \
    static_assert( \
        std::is_same< \
            decltype( \
                swordxx::formatXmlAttributeValue(std::declval<__VA_ARGS__>())),\
            std::string \
        >::value, "")
TEST_SIGNATURE(std::string_view &);
TEST_SIGNATURE(std::string_view const &);
TEST_SIGNATURE(std::string &);
TEST_SIGNATURE(std::string const &);
TEST_SIGNATURE(char *);
TEST_SIGNATURE(char const *);
TEST_SIGNATURE(char [42]);
TEST_SIGNATURE(char const [42]);
TEST_SIGNATURE(char []);
TEST_SIGNATURE(char const []);

std::string backAgain(std::string quotedXmlAttribute) {
    auto const size = quotedXmlAttribute.size();
    TEST_ASSERT(size >= 2u);
    TEST_ASSERT(quotedXmlAttribute.front() == quotedXmlAttribute.back());
    char const quote = quotedXmlAttribute.front();
    TEST_ASSERT((quote == '"') || (quote == '\''));
    auto sv(std::string_view(quotedXmlAttribute).substr(1u, size - 2u));
    std::string r;
    while (!sv.empty()) {
        auto const c = sv.front();
        TEST_ASSERT(c != quote);
        if (c == '&') {
            static constexpr auto const escape("&quot;"sv);
            TEST_ASSERT(quote == '"');
            TEST_ASSERT(swordxx::startsWith(sv, escape));
            r += quote;
            sv.remove_prefix(escape.size());
        } else {
            r += c;
            sv.remove_prefix(1u);
        }
    }
    return r;
}

constexpr std::array<std::string_view, 5u> const alphabet =
        { ""sv, "x"sv, "xy"sv, "'"sv, "\""sv };

template <typename T>
constexpr T pow(T const base, T const exp) noexcept {
    return (exp == 0u) ? 1u :
           (exp == 1u) ? base :
            base * pow(base, exp - 1u);
}
static_assert(pow(2u, 0u) == 1u, "");
static_assert(pow(2u, 1u) == 2u, "");
static_assert(pow(2u, 2u) == 4u, "");
static_assert(pow(2u, 3u) == 8u, "");

template <typename T>
constexpr auto maxElementSize(T begin, T end, std::size_t max = 0u) {
    return (begin == end) ? 0u :
           (begin->size() > max) ? begin->size() : max;
}

constexpr auto const maxPartSize =
        maxElementSize(alphabet.begin(), alphabet.end());
static constexpr std::size_t const numParts = 5u;

int main() {
    std::string input;
    input.reserve(numParts * maxPartSize);
    for (std::size_t i = 0u;
         i < pow(alphabet.size(), numParts);
         ++i, input.clear())
    {
        auto j = i;
        for (std::size_t part = 0u;
             part < numParts;
             ++part, j /= alphabet.size())
            input += alphabet[j % alphabet.size()];
        auto result(swordxx::formatXmlAttributeValue(input));
        //std::cout << input << ':' << result << std::endl;
        TEST_ASSERT(input == backAgain(std::move(result)));
        input.clear();
    }
}
