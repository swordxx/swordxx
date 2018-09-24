#include <swordxx/utilstr.h>

#include <cctype>
#include "AllAsciiChars.h"
#include "TestAssert.h"


static_assert(std::is_same<decltype(swordxx::charToLowerTable),
                           char const[256]>::value, "");
static_assert(std::is_same<decltype(swordxx::charToLower(std::declval<char>())),
                           char>::value, "");

int main() {
    for (auto const c : allAsciiChars) {
        auto const uc(static_cast<unsigned char>(c));
        char const expectedValue =
                std::isupper(uc) ? allAsciiChars[uc - 65u + 97u] : c;
        TEST_ASSERT(swordxx::charToLowerTable[uc] == expectedValue);
        TEST_ASSERT(swordxx::charToLower(c) == swordxx::charToLowerTable[uc]);
    }
}
