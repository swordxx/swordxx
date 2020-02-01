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

#ifndef SWORDXX_UNICODECONVERSIONFILTER_H
#define SWORDXX_UNICODECONVERSIONFILTER_H

#include "swfilter.h"

#include <string_view>
#include "UnicodeConverter.h"


namespace swordxx {

class SWDLLEXPORT UnicodeConversionFilter : public SWFilter {

public: /* Methods: */

    UnicodeConversionFilter(char const * type);

protected: /* Methods: */

    std::string decode(std::string_view sv);
    std::string encode(std::string_view sv);

private: /* Fields: */

    UnicodeConverter m_converter;

};

} /* namespace swordxx */

#define SWORDXX_DECLARE_UNICODE_CONVERSION_FILTER(Name) \
    namespace swordxx { \
    class SWDLLEXPORT Name: public UnicodeConversionFilter { \
    public: /* Methods: */ \
        Name(); \
        ~Name() noexcept; \
        char processText(std::string & text, \
                         SWKey const * key = nullptr, \
                         SWModule const * module = nullptr) override; \
    }; \
    } /* namespace swordxx */

#define SWORDXX_DEFINE_UNICODE_CONVERSION_FILTER(Name,conv,method) \
    namespace swordxx { \
    Name::Name() : UnicodeConversionFilter((conv)) {} \
    Name::~Name() noexcept = default; \
    char Name::processText(std::string & text, \
                           SWKey const * key, \
                           SWModule const *) \
    { \
        if ((unsigned long)key < 2) /* hack, we're en(1)/de(0)ciphering */ \
            return -1; \
        text = method(text); \
        return 0; \
    } \
    } /* namespace swordxx */

#endif /* SWORDXX_UNICODECONVERSIONFILTER_H */
