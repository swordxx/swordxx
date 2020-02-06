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

#ifndef SWORDXX_FLAGS_H
#define SWORDXX_FLAGS_H

#include <type_traits>


namespace swordxx {

template <typename T>
class Flags {

    static_assert(std::is_enum<T>::value, "");

public: /* Types: */

    using EnumType = T;
    using UnderlyingType = std::underlying_type_t<T>;
    enum { Unset = 0b0 };

public: /* Methods: */

    constexpr Flags() noexcept = default;
    constexpr Flags(Flags &&) noexcept = default;
    constexpr Flags(Flags const &) noexcept = default;

    constexpr Flags(decltype(Unset) const) noexcept : m_value{} {}
    constexpr Flags(EnumType value) noexcept : m_value(value) {}
    constexpr Flags(UnderlyingType const value) noexcept : m_value(value) {}

    constexpr Flags & operator=(Flags &&) noexcept = default;
    constexpr Flags & operator=(Flags const &) noexcept = default;
    constexpr Flags & operator=(decltype(Unset)) noexcept {
        m_value = 0b0;
        return *this;
    }
    constexpr Flags & operator=(EnumType const value) noexcept {
        m_value = value;
        return *this;
    }

    constexpr UnderlyingType underlying() const noexcept { return m_value; }

    explicit constexpr operator EnumType () const noexcept
    { return static_cast<EnumType>(m_value); }

    explicit constexpr operator UnderlyingType() const noexcept
    { return m_value; }

    explicit constexpr operator bool() const noexcept
    { return m_value; }

    constexpr Flags operator~() const noexcept { return ~m_value; }

#define SWORDXX_FLAGS_H_(op) \
    constexpr Flags & operator op ## =(Flags const & rhs) noexcept { \
        m_value op ## = rhs.m_value; ;\
        return *this; \
    } \
    constexpr Flags & operator op ## =(EnumType const rhs) noexcept { \
        m_value op ## = static_cast<UnderlyingType>(rhs); \
        return *this; \
    } \
    friend constexpr Flags operator op(Flags const & lhs, Flags const & rhs) \
            noexcept \
    { return {static_cast<UnderlyingType>(lhs.m_value op rhs.m_value)}; } \
    friend constexpr Flags operator op(Flags const & lhs, EnumType const & rhs)\
            noexcept \
    { return {static_cast<UnderlyingType>(lhs.m_value op rhs)}; } \
    friend constexpr Flags operator op(EnumType const & lhs, Flags const & rhs)\
            noexcept \
    { return {static_cast<UnderlyingType>(lhs op rhs.m_value)}; } \
    friend constexpr Flags operator op(Flags const & lhs, \
                                       decltype(Unset) const &) noexcept \
    { return {static_cast<UnderlyingType>(lhs.m_value op 0b0)};  } \
    friend constexpr Flags operator op(decltype(Unset) const &, \
                                       Flags const & rhs) noexcept \
    { return {static_cast<UnderlyingType>(0b0 op rhs.m_value)}; }
    SWORDXX_FLAGS_H_(&)
    SWORDXX_FLAGS_H_(|)
    SWORDXX_FLAGS_H_(^)
#undef SWORDXX_FLAGS_H_

#define SWORDXX_FLAGS_H_(op) \
    friend constexpr bool operator op(Flags const & lhs, Flags const & rhs) \
            noexcept \
    { return lhs.m_value op rhs.m_value; } \
    friend constexpr bool operator op(Flags const & lhs, EnumType const & rhs) \
            noexcept \
    { return lhs.m_value op rhs; } \
    friend constexpr bool operator op(EnumType const & lhs, Flags const & rhs) \
            noexcept \
    { return lhs op rhs.m_value; }
SWORDXX_FLAGS_H_(<)
SWORDXX_FLAGS_H_(<=)
SWORDXX_FLAGS_H_(==)
SWORDXX_FLAGS_H_(!=)
SWORDXX_FLAGS_H_(>=)
SWORDXX_FLAGS_H_(>)
#undef SWORDXX_FLAGS_H_

private: /* Fields: */

    UnderlyingType m_value;

};

} /* namespace swordxx */

#endif /* SWORDXX_FLAGS_H */
