/******************************************************************************
 *
 * Copyright Jaak Ristioja
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#include <swordxx/Flags.h>

#include <cstring>
#include <type_traits>
#include <utility>
#include "TestAssert.h"


#define SA(...) static_assert(__VA_ARGS__, "")
#define SASAME(...) SA(std::is_same_v<__VA_ARGS__>)
#define SARETNOEXCEPT(R,...) \
    SASAME(R, decltype(__VA_ARGS__)); \
    SA(noexcept(__VA_ARGS__))
#define D(...) std::declval<__VA_ARGS__>()

template <typename T>
bool memEqual(T const & a, T const & b) noexcept
{ return std::memcmp(&a, &b, sizeof(T)) == 0; }

template <typename T, typename V>
constexpr auto testAssignReturn(T && obj, V && v) noexcept
{ return &(obj = std::forward<V>(v)) == &obj; }

using U = unsigned short;
enum T : U {
    Z = 0,
    B1 = 0b001,
    B2 = 0b010,
    B3 = 0b100
};
constexpr U toU(T v) noexcept { return static_cast<U>(v); }
using F = swordxx::Flags<T>;
using FC = F const;
static constexpr auto const unset = F::Unset;
using UnsetT = decltype(unset);

SA(sizeof(F) == sizeof(U));
SA(std::is_standard_layout_v<F>);
SA(std::is_trivially_default_constructible_v<F>);
SA(std::is_trivially_copy_constructible_v<F>);
SA(std::is_trivially_move_constructible_v<F>);
SA(std::is_trivially_destructible_v<F>);
SA(std::is_trivially_copyable_v<F>);
SA(std::is_trivially_copy_assignable_v<F>);
SA(std::is_trivially_move_assignable_v<F>);

SA(std::is_same_v<typename F::EnumType, T>);
SA(std::is_same_v<typename F::UnderlyingType, U>);

SA(std::is_nothrow_default_constructible_v<F>);
SA(std::is_nothrow_constructible_v<F, UnsetT>);
SA(std::is_nothrow_constructible_v<F, T>);
SA(std::is_nothrow_constructible_v<F, U>);
SA(std::is_nothrow_copy_constructible_v<F>);
SA(std::is_nothrow_move_constructible_v<F>);

SA(std::is_nothrow_copy_assignable_v<F>);
SA(std::is_nothrow_move_assignable_v<F>);
SA(std::is_nothrow_assignable_v<F, UnsetT>);
SA(std::is_nothrow_assignable_v<F, T>);
SASAME(F &, decltype(D(F &) = D(F const &)));
SASAME(F &, decltype(D(F &) = D(F &&)));
SASAME(F &, decltype(D(F &) = D(T const &)));
SASAME(F &, decltype(D(F &) = F::Unset));

SARETNOEXCEPT(U, D(F const &).underlying());

SA(noexcept(static_cast<T>(D(F const &))));
SA(noexcept(static_cast<bool>(D(F const &))));
SA(noexcept(static_cast<U>(D(F const &))));

SARETNOEXCEPT(F, ~D(F const &));

SARETNOEXCEPT(F &, D(F &) &= D(F const &));
SARETNOEXCEPT(F &, D(F &) |= D(F const &));
SARETNOEXCEPT(F &, D(F &) ^= D(F const &));
SARETNOEXCEPT(F &, D(F &) &= D(T const &));
SARETNOEXCEPT(F &, D(F &) |= D(T const &));
SARETNOEXCEPT(F &, D(F &) ^= D(T const &));

SARETNOEXCEPT(F, D(F const &) & D(F const &));
SARETNOEXCEPT(F, D(F const &) | D(F const &));
SARETNOEXCEPT(F, D(F const &) ^ D(F const &));
SARETNOEXCEPT(F, D(F const &) & D(T const &));
SARETNOEXCEPT(F, D(F const &) | D(T const &));
SARETNOEXCEPT(F, D(F const &) ^ D(T const &));


SARETNOEXCEPT(bool, D(F const &) < D(F const &));
SARETNOEXCEPT(bool, D(F const &) < D(T const &));
SARETNOEXCEPT(bool, D(F const &) < F::Unset);
SARETNOEXCEPT(bool, D(F const &) < D(F const &));
SARETNOEXCEPT(bool, D(T const &) < D(F const &));
SARETNOEXCEPT(bool, F::Unset < D(F const &));

#define T(op) \
    SARETNOEXCEPT(bool, D(F const &) op D(F const &)); \
    SARETNOEXCEPT(bool, D(F const &) op D(T const &)); \
    SARETNOEXCEPT(bool, D(F const &) op F::Unset); \
    SARETNOEXCEPT(bool, D(F const &) op D(F const &)); \
    SARETNOEXCEPT(bool, D(T const &) op D(F const &)); \
    SARETNOEXCEPT(bool, F::Unset op D(F const &))
    T(<); T(<=); T(==); T(!=); T(>=); T(>);
#undef T

template <typename F_, T v>
void testWithValue() {
    SA(std::is_same_v<F_, F> || std::is_same_v<F_, FC>);
    using FO = std::conditional_t<std::is_const_v<F_>, FC, F>;
    static constexpr auto const u = toU(v);

    // Test construction and underlying():
    SA(F_(v).underlying() == u);
    SA(F_(u).underlying() == u);

    // Test assignment:
    if constexpr (!std::is_const_v<F_>) {
        SA(testAssignReturn(F_(v), unset));
        SA(!(F_(v) = unset).underlying());
        SA(testAssignReturn(F_(v), v));
        SA((F_(v) = v).underlying() == u);
    }

    // Test casting operators:
    SA(static_cast<T>(F_(v)) == v);
    SA(static_cast<U>(F_(v)) == u);
    SA(static_cast<bool>(F_(v)) == static_cast<bool>(u));

    // Test bitwise arithmetic:
    SA((~F_(v)).underlying() == U(~u));
    #define T(op) \
        if constexpr (!std::is_const_v<F_>) { \
            SA((F_(v) op ## = F_(v)).underlying() == (u op u)); \
            SA((F_(v) op ## = FO(v)).underlying() == (u op u)); \
            SA((F_(v) op ## = v).underlying() == (u op u)); \
        } \
        SA((F_(v) op F_(v)).underlying() == (u op u)); \
        SA((F_(v) op FO(v)).underlying() == (u op u)); \
        SA((F_(v) op v).underlying() == (u op u)); \
        SA((v op F_(v)).underlying() == (u op u)); \
        SA((F_(v) op F::Unset).underlying() == (u op U(0))); \
        SA((F_::Unset op F_(v)).underlying() == (U(0) op u))
    T(&); T(|); T(^);
    #undef T

    // Test comparisons:
    #define T(op) \
        SA((F_(v) op F_(v)) == (u op u)); \
        SA((F_(v) op v) == (u op u)); \
        SA((v op F_(v)) == (u op u)); \
        SA((F_(v) op F_::Unset) == (u op U(0b0))); \
        SA((F_::Unset op F_(v)) == (U(0b0) op u))
    T(<); T(<=); T(==); T(!=); T(>=); T(>);
    #undef T
}

template <T a, T b>
void testWithValues() {
    static constexpr auto const ua = toU(a);
    static constexpr auto const ub = toU(b);

    // Test bitwise ops:
    #define T(op) \
        SA((F(a) op ## = F(b)) == F(ua op ub)); \
        SA((F(a) op ## = FC(b)) == F(ua op ub)); \
        SA((F(a) op ## = b) == F(ua op ub)); \
        SA((F(a) op F(b)) == (ua op ub)); \
        SA((F(a) op FC(b)) == (ua op ub)); \
        SA((FC(a) op F(b)) == (ua op ub)); \
        SA((FC(a) op FC(b)) == (ua op ub)); \
        SA((F(a) op b) == (ua op ub)); \
        SA((FC(a) op b) == (ua op ub))
    T(&); T(|); T(^);
    #undef T

    // Test comparisons:
    #define T(op) \
        SA((F(a) op F(b)) == (a op b)); \
        SA((F(a) op FC(b)) == (a op b)); \
        SA((FC(a) op F(b)) == (a op b)); \
        SA((FC(a) op FC(b)) == (a op b)); \
        SA((F(a) op b) == (a op b)); \
        SA((FC(a) op b) == (a op b)); \
        SA((a op F(b)) == (a op b)); \
        SA((a op FC(b)) == (a op b))
    T(<); T(<=); T(==); T(!=); T(>=); T(>);
    #undef T
}

SA(F(F::Unset) == Z);
SA(F(F(F::Unset)) == Z);

int main() {
    testWithValue<F, Z>();
    testWithValue<F, B1>();
    testWithValue<F, B2>();
    testWithValue<F, B3>();
    testWithValue<FC, Z>();
    testWithValue<FC, B1>();
    testWithValue<FC, B2>();
    testWithValue<FC, B3>();

    testWithValues<Z, Z>();
    testWithValues<Z, B1>();
    testWithValues<Z, B2>();
    testWithValues<Z, B3>();
    testWithValues<B1, Z>();
    testWithValues<B1, B1>();
    testWithValues<B1, B2>();
    testWithValues<B1, B3>();
    testWithValues<B2, Z>();
    testWithValues<B2, B1>();
    testWithValues<B2, B2>();
    testWithValues<B2, B3>();
    testWithValues<B3, Z>();
    testWithValues<B3, B1>();
    testWithValues<B3, B2>();
    testWithValues<B3, B3>();
}
