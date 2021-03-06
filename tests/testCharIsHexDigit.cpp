#include <swordxx/utilstr.h>

#include <cctype>
#include <type_traits>
#include "AllAsciiChars.h"
#include "TestAssert.h"


static_assert(std::is_same<decltype(swordxx::charIsHexDigitTable),
                           bool const[256]>::value, "");
static_assert(std::is_same<decltype(swordxx::charIsHexDigit(
                                        std::declval<char>())),
                           bool>::value, "");

int main() {
    for (auto const c : allAsciiChars) {
        auto const uc(static_cast<unsigned char>(c));
        TEST_ASSERT(swordxx::charIsHexDigitTable[uc]
                    == static_cast<bool>(std::isxdigit(uc)));
        TEST_ASSERT(swordxx::charIsHexDigit(c)
                    == swordxx::charIsHexDigitTable[uc]);
    }
}
