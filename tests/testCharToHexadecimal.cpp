#include <swordxx/utilstr.h>

#include <cctype>
#include "AllAsciiChars.h"
#include "TestAssert.h"


static_assert(std::is_same<decltype(swordxx::charToHexadecimalTable),
                           int const[256]>::value, "");
static_assert(std::is_same<decltype(swordxx::charToHexadecimal(
                                        std::declval<char>())),
                           int>::value, "");

int main() {
    for (auto const c : allAsciiChars) {
        auto const uc(static_cast<unsigned char>(c));
        int const expectedValue = !std::isxdigit(uc) ? -1 :
                                  std::isdigit(uc) ? (uc - 48u) :
                                  std::isupper(uc) ? (uc - 65u + 0xau)
                                                   : (uc - 97u + 0xau);
        TEST_ASSERT(swordxx::charToHexadecimalTable[uc] == expectedValue);
        TEST_ASSERT(swordxx::charToHexadecimal(c)
                    == swordxx::charToHexadecimalTable[uc]);
    }
}
