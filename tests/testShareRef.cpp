#include <swordxx/ShareRef.h>

#include <type_traits>
#include "TestAssert.h"


template <typename Expected, typename T>
std::shared_ptr<Expected> test(T & ref) {
    auto s(swordxx::shareRef(ref));
    static_assert(std::is_same_v<decltype(s), std::shared_ptr<Expected>>, "");
    TEST_ASSERT(s.use_count() == 0); // no managed object
    TEST_ASSERT(s.get() == &ref);
    return s;
}

#define TEST_(qual,T,init,...) \
    do { \
        T qual obj init; \
        auto s(test<T qual>(obj)); \
        TEST_ASSERT(__VA_ARGS__); \
    } while(false)
#define TEST(...) \
    do { \
        TEST_(,__VA_ARGS__); \
        TEST_(const,__VA_ARGS__); \
        TEST_(volatile,__VA_ARGS__); \
        TEST_(const volatile,__VA_ARGS__); \
    } while(false)

int main() {
    TEST(int,=42,*s == 42);
    struct X { int i = 42; };
    TEST(X,,s->i == 42);
}
