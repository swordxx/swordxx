#include <swordxx/utilstr.h>

#include <cctype>
#include "AllAsciiChars.h"
#include "TestAssert.h"


static_assert(std::is_same<decltype(swordxx::charIsLowerTable),
                           bool const[256]>::value, "");
static_assert(std::is_same<decltype(swordxx::charIsLower(std::declval<char>())),
                           bool>::value, "");

int main() {
    for (auto const c : allAsciiChars) {
        auto const uc(static_cast<unsigned char>(c));
        TEST_ASSERT(swordxx::charIsLowerTable[uc]
                    == static_cast<bool>(std::islower(uc)));
        TEST_ASSERT(swordxx::charIsLower(c) == swordxx::charIsLowerTable[uc]);
    }
}
