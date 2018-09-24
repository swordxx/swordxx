#include <swordxx/utilstr.h>

#include <cctype>
#include "AllAsciiChars.h"
#include "TestAssert.h"


static_assert(std::is_same<decltype(swordxx::charIsUpperTable),
                           bool const[256]>::value, "");
static_assert(std::is_same<decltype(swordxx::charIsUpper(std::declval<char>())),
                           bool>::value, "");

int main() {
    for (auto const c : allAsciiChars) {
        auto const uc(static_cast<unsigned char>(c));
        TEST_ASSERT(swordxx::charIsUpperTable[uc]
                    == static_cast<bool>(std::isupper(uc)));
        TEST_ASSERT(swordxx::charIsUpper(c) == swordxx::charIsUpperTable[uc]);
    }
}
