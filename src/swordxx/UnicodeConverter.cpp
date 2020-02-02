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

#include "UnicodeConverter.h"

#include <cassert>
#include <cstdint>
#include <mutex>
#include <new>
#include <stdexcept>
#include <unicode/ucnv.h>
#include <unicode/utypes.h>
#include "max_v.h"


namespace {

template <auto f> struct UnicodeConverterPrivateTraits;
#define SWORDXX_DEFINE_TRAITS(f) \
    template <> struct UnicodeConverterPrivateTraits<::f> { \
        [[noreturn]] static inline void throwRuntimeError() \
        { throw std::runtime_error(#f "() failed!"); } \
    }
SWORDXX_DEFINE_TRAITS(ucnv_toUChars);
SWORDXX_DEFINE_TRAITS(ucnv_fromUChars);
#undef SWORDXX_DEFINE_TRAITS

struct UnicodeConverterPrivate {

    UnicodeConverterPrivate(char const * const type)
        : m_converter(
            [type]() {
                ::UErrorCode err = ::U_ZERO_ERROR;
                auto const r = ::ucnv_open(type, &err);
                if (::U_FAILURE(err))
                    throw std::runtime_error("ucnv_open() failed!");
                return r;
            }())
    {}

    UnicodeConverterPrivate(UnicodeConverterPrivate &&) = delete;

    UnicodeConverterPrivate(UnicodeConverterPrivate const & copy)
        : m_converter(
            [&copy]() {
                assert(copy.m_converter);
                ::UErrorCode err = ::U_ZERO_ERROR;
                auto const r =
                    ::ucnv_safeClone(copy.m_converter, nullptr, nullptr, &err);
                if (::U_FAILURE(err))
                    throw std::runtime_error("ucnv_safeClone() failed!");
                return r;
            }())
    {}

    ~UnicodeConverterPrivate() noexcept { ::ucnv_close(m_converter); }

    UnicodeConverterPrivate & operator=(UnicodeConverterPrivate &&) = delete;
    UnicodeConverterPrivate & operator=(UnicodeConverterPrivate const &)
            = delete;

    template <typename To,
              auto resetFunction,
              auto conversionFunction,
              typename From>
    To convert(From sv) {
        using Traits = UnicodeConverterPrivateTraits<conversionFunction>;
        using swordxx::max_v;
        static_assert(max_v<typename To::size_type> >= max_v<std::int32_t>, "");
        if (sv.size() > max_v<std::int32_t>)
            throw std::bad_array_new_length();

        std::lock_guard<std::mutex> const guard(m_mutex);
        resetFunction(m_converter);

        std::int32_t destSize;
        {
            ::UErrorCode err = ::U_ZERO_ERROR;
            destSize = conversionFunction(m_converter,
                                          nullptr,
                                          0u,
                                          sv.data(),
                                          static_cast<std::int32_t>(sv.size()),
                                          &err);
            if (::U_FAILURE(err) && (err != ::U_BUFFER_OVERFLOW_ERROR))
                Traits::throwRuntimeError();
        }
        To r;
        r.resize(static_cast<typename To::size_type>(destSize));
        {
            ::UErrorCode err = ::U_ZERO_ERROR;
            destSize = conversionFunction(m_converter,
                                          r.data(),
                                          destSize,
                                          sv.data(),
                                          static_cast<std::int32_t>(sv.size()),
                                          &err);
            if (::U_FAILURE(err))
                Traits::throwRuntimeError();
        }
        return r;
    }

    std::basic_string<char16_t> toUnicode(std::string_view sv) {
        return convert<std::basic_string<char16_t>,
                       ::ucnv_resetToUnicode,
                       ::ucnv_toUChars>(sv);
    }

    std::string fromUnicode(std::basic_string_view<char16_t> sv) {
        return convert<std::string,
                       ::ucnv_resetFromUnicode,
                       ::ucnv_fromUChars>(sv);
    }

/* Fields: */

    std::mutex m_mutex;
    UConverter * const m_converter;

};

} // anonymous namespace

namespace swordxx {

UnicodeConverter::UnicodeConverter(char const * const type)
    : m_p(new UnicodeConverterPrivate(type))
{}

UnicodeConverter::UnicodeConverter(UnicodeConverter && move) noexcept
    : m_p(move.m_p)
{ move.m_p = nullptr; }

UnicodeConverter::~UnicodeConverter() noexcept
{ delete static_cast<UnicodeConverterPrivate *>(m_p); }

UnicodeConverter &
UnicodeConverter::operator=(UnicodeConverter && move) noexcept {
    delete static_cast<UnicodeConverterPrivate *>(m_p);
    m_p = move.m_p;
    move.m_p = nullptr;
    return *this;
}

std::basic_string<char16_t> UnicodeConverter::toUnicode(std::string_view sv) {
    assert(m_p);
    return static_cast<UnicodeConverterPrivate *>(m_p)->toUnicode(sv);
}

std::string UnicodeConverter::fromUnicode(std::basic_string_view<char16_t> sv) {
    assert(m_p);
    return static_cast<UnicodeConverterPrivate *>(m_p)->fromUnicode(sv);
}

} /* namespace swordxx */
