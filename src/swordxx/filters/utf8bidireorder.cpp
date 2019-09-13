/******************************************************************************
 *
 *  utf8bidireorder.cpp -    SWFilter descendant to perform reordering of
 *                UTF-8 text to visual order according to the
 *                Unicode Bidirectional Algorithm (UBA)
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
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

#include "utf8bidireorder.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <unicode/ubidi.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/utypes.h>
#include <vector>
#include "../DebugOnly.h"
#include "../max_v.h"
#include "../SignConversion.h"
#include "../utilstr.h"


namespace swordxx {

UTF8BiDiReorder::UTF8BiDiReorder() = default;

UTF8BiDiReorder::~UTF8BiDiReorder() = default;

char UTF8BiDiReorder::processText(std::string & text,
                                  SWKey const * key,
                                  SWModule const *)
{
    if ((unsigned long)key < 2)    // hack, we're en(1)/de(0)ciphering
        return -1;

    std::unique_ptr<::UConverter, void (*)(::UConverter *)> conv(
                []() {
                    ::UErrorCode err = U_ZERO_ERROR;
                    return ::ucnv_open("UTF-8", &err);
                }(),
                +[](::UConverter * const p) noexcept { ::ucnv_close(p); });

    if (text.size() > toUnsigned(max_v<std::int32_t>))
        throw std::runtime_error("Implementation limits reached!");

    // Convert UTF-8 string to UTF-16 (UChars):
    std::vector<::UChar> ustr;
    {
        ::UErrorCode err = U_ZERO_ERROR;
        auto const sourceLen = std::strlen(text.c_str());
        if (sourceLen >= toUnsigned(max_v<std::int32_t>))
            throw std::runtime_error("Implementation limits reached!");
        auto const len = ::ucnv_toUChars(conv.get(),
                                         nullptr,
                                         0,
                                         text.c_str(),
                                         static_cast<std::int32_t>(sourceLen),
                                         &err);
        if (U_FAILURE(err))
            throw std::runtime_error("::ucnv_toUChars() failed!");
        assert(len >= 0);
        if (len >= max_v<decltype(len)>) // need to increment later
            throw std::runtime_error("Implementation limits reached!");
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wtype-limits"
        if constexpr (toUnsigned(max_v<decltype(len)>)
                      > max_v<decltype(ustr)::size_type>)
            if (toUnsigned(len) > max_v<decltype(ustr)::size_type>)
                throw std::runtime_error("Implementation limits reached!");
        #pragma GCC diagnostic pop
        ustr.resize(static_cast<decltype(ustr)::size_type>(toUnsigned(len)));
        err = U_ZERO_ERROR;
        SWORDXX_DEBUG_ONLY(auto const r =)
                ::ucnv_toUChars(conv.get(),
                                ustr.data(),
                                len,
                                text.c_str(),
                                static_cast<std::int32_t>(sourceLen),
                                &err);
        assert(r == len);
        if (U_FAILURE(err))
            throw std::runtime_error("::ucnv_toUChars() failed!");
    }

    decltype(ustr) ustr2;
    assert(ustr.size() < toUnsigned(max_v<std::int32_t>));
    {
        auto const len = static_cast<std::int32_t>(ustr.size());
        std::unique_ptr<::UBiDi, void (*)(::UBiDi *)> bidi(
                    [len]() {
                        ::UErrorCode err = U_ZERO_ERROR;
                        return ::ubidi_openSized(len + 1, 0, &err);
                    }(),
                    +[](::UBiDi * const p) noexcept { ::ubidi_close(p); });
        if (!conv)
            throw std::runtime_error("::ubidi_openSized() failed!");
        {
            ::UErrorCode err = U_ZERO_ERROR;
            ::ubidi_setPara(bidi.get(),
                            ustr.data(),
                            len,
                            UBIDI_DEFAULT_RTL,
                            nullptr,
                            &err);
            if (U_FAILURE(err))
                throw std::runtime_error("::ubidi_setPara() failed!");
        }
        auto const maxLen(std::max(::ubidi_getLength(bidi.get()),
                                   ::ubidi_getProcessedLength(bidi.get())));
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wtype-limits"
        if constexpr (toUnsigned(max_v<decltype(maxLen)>)
                      > max_v<decltype(ustr2)::size_type>)
            if (toUnsigned(maxLen) > max_v<decltype(ustr2)::size_type>)
                throw std::runtime_error("Implementation limits reached!");
        #pragma GCC diagnostic pop
        ustr2.resize(static_cast<decltype(ustr2)::size_type>(maxLen));
        {
            ::UErrorCode err = U_ZERO_ERROR;
            auto const outLen =
                    ::ubidi_writeReordered(
                        bidi.get(),
                        ustr2.data(),
                        maxLen,
                        UBIDI_DO_MIRRORING | UBIDI_REMOVE_BIDI_CONTROLS,
                        &err);
            if (U_FAILURE(err))
                throw std::runtime_error("::ubidi_writeReordered() failed!");
            assert(outLen >= 0);
            assert(outLen <= maxLen);
            ustr2.resize(static_cast<decltype(ustr2)::size_type>(outLen));
        }
    }

#if 0
//        len = ubidi_writeReverse(ustr, len, ustr2, len,
//                UBIDI_DO_MIRRORING | UBIDI_REMOVE_BIDI_CONTROLS, &err);
#endif

    UErrorCode err = U_ZERO_ERROR;
    auto const len =
            ::ucnv_fromUChars(conv.get(),
                              nullptr,
                              0,
                              ustr2.data(),
                              static_cast<std::int32_t>(ustr2.size()),
                              &err);
    if (U_FAILURE(err))
        throw std::runtime_error("::ucnv_fromUChars() failed!");
    assert(len >= 0);
    std::string out;
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wtype-limits"
    if constexpr (toUnsigned(max_v<decltype(len)>)
                  > max_v<decltype(out)::size_type>)
        if (toUnsigned(len) > max_v<decltype(out)::size_type>)
            throw std::runtime_error("Implementation limits reached!");
    #pragma GCC diagnostic pop
    out.resize(static_cast<decltype(out)::size_type>(toUnsigned(len)));
    err = U_ZERO_ERROR;
    SWORDXX_DEBUG_ONLY(auto const r =)
            ::ucnv_fromUChars(conv.get(),
                              out.data(),
                              len,
                              ustr2.data(),
                              static_cast<std::int32_t>(ustr2.size()),
                              &err);
    assert(r == len);
    if (U_FAILURE(err))
        throw std::runtime_error("::ucnv_fromUChars() failed!");
    text.swap(out);
    return 0;
}

} /* namespace swordxx */
