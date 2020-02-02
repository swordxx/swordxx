/******************************************************************************
 *
 *  utf8bidireorder.cpp -    SWFilter descendant to perform reordering of
 *                UTF-8 text to visual order according to the
 *                Unicode Bidirectional Algorithm (UBA)
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
#include <new>
#include <stdexcept>
#include <string>
#include <unicode/ubidi.h>
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

    // Convert UTF-8 string to UTF-16 (UChars):
    auto ustr(utf8ToUtf16(text));

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
        if (!bidi)
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
                throw std::bad_array_new_length();
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

    text = utf16ToUtf8(ustr2);
    return 0;
}

} /* namespace swordxx */
