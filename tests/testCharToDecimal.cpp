#include <swordxx/utilstr.h>

#include <cctype>
#include "AllAsciiChars.h"
#include "TestAssert.h"


static_assert(std::is_same<decltype(swordxx::charToDecimalTable),
                           int const[256]>::value, "");
static_assert(std::is_same<decltype(swordxx::charToDecimal(
                                        std::declval<char>())),
                           int>::value, "");

int main() {
    for (auto const c : allAsciiChars) {
        auto const uc(static_cast<unsigned char>(c));
        int const expectedValue = std::isdigit(uc) ? (uc - 48u) : -1;
        TEST_ASSERT(swordxx::charToDecimalTable[uc] == expectedValue);
        TEST_ASSERT(swordxx::charToDecimal(c)
                    == swordxx::charToDecimalTable[uc]);
    }
}
