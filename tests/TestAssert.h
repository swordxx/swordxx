#ifndef SWORDXX_TESTS_TESTASSERT_H
#define SWORDXX_TESTS_TESTASSERT_H

#include <cstdlib>
#include <iostream>


#define TEST_ASSERT(...) \
        if (!(__VA_ARGS__)) { \
            std::cerr << __FILE__ ":" << __LINE__ << ": " \
                      << __PRETTY_FUNCTION__ \
                      << ": Test assertion '" #__VA_ARGS__ "' failed!" \
                      << std::endl; \
            std::abort(); \
        } else (void) 0

#endif /* SWORDXX_TESTS_TESTASSERT_H */
