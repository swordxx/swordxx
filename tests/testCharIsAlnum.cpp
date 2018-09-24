#include <swordxx/utilstr.h>

#include <cctype>
#include "AllAsciiChars.h"
#include "TestAssert.h"


static_assert(std::is_same<decltype(swordxx::charIsAlnumTable),
                           bool const[256]>::value, "");
static_assert(std::is_same<decltype(swordxx::charIsAlnum(std::declval<char>())),
                           bool>::value, "");

int main() {
    for (auto const c : allAsciiChars) {
        auto const uc(static_cast<unsigned char>(c));
        TEST_ASSERT(swordxx::charIsAlnumTable[uc]
                    == static_cast<bool>(std::isalnum(uc)));
        TEST_ASSERT(swordxx::charIsAlnum(c) == swordxx::charIsAlnumTable[uc]);
    }
}
