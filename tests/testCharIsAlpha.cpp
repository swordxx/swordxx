#include <swordxx/utilstr.h>

#include <cctype>
#include "AllAsciiChars.h"
#include "TestAssert.h"


static_assert(std::is_same<decltype(swordxx::charIsAlphaTable),
                           bool const[256]>::value, "");
static_assert(std::is_same<decltype(swordxx::charIsAlpha(std::declval<char>())),
                           bool>::value, "");

int main() {
    for (auto const c : allAsciiChars) {
        auto const uc(static_cast<unsigned char>(c));
        TEST_ASSERT(swordxx::charIsAlphaTable[uc]
                    == static_cast<bool>(std::isalpha(uc)));
        TEST_ASSERT(swordxx::charIsAlpha(c) == swordxx::charIsAlphaTable[uc]);
    }
}
