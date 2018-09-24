#include <swordxx/utilstr.h>

#include "AllAsciiChars.h"
#include "TestAssert.h"


static_assert(std::is_same<decltype(swordxx::charIsSpaceTable),
                           bool const[256]>::value, "");
static_assert(std::is_same<decltype(swordxx::charIsSpace(std::declval<char>())),
                           bool>::value, "");

int main() {
    for (auto const c : allAsciiChars) {
        auto const uc(static_cast<unsigned char>(c));
        TEST_ASSERT(swordxx::charIsSpaceTable[uc]
                    == ((c == ' ') || (c == '\n') || (c == '\r')
                        || (c == '\t')));
        TEST_ASSERT(swordxx::charIsSpace(c) == swordxx::charIsSpaceTable[uc]);
    }
}
