#include <swordxx/utilstr.h>

#include <cctype>
#include "AllAsciiChars.h"
#include "TestAssert.h"


static_assert(std::is_same<decltype(swordxx::charToUpperTable),
                           char const[256]>::value, "");
static_assert(std::is_same<decltype(swordxx::charToUpper(std::declval<char>())),
                           char>::value, "");

int main() {
    for (auto const c : allAsciiChars) {
        auto const uc(static_cast<unsigned char>(c));
        char const expectedValue =
                std::islower(uc) ? allAsciiChars[uc - 97u + 65u] : c;
        TEST_ASSERT(swordxx::charToUpperTable[uc] == expectedValue);
        TEST_ASSERT(swordxx::charToUpper(c) == swordxx::charToUpperTable[uc]);
    }
}
