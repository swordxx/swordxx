/******************************************************************************
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

#include "utf8transliterator.h"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <unicode/resbund.h>
#include <unicode/translit.h>
#include <unicode/rep.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/unistr.h>
#include <vector>
#include <utility>
#include <type_traits>
#include "../DebugOnly.h"
#include "../max_v.h"
#include "../SignConversion.h"
#include "../swlog.h"
#include "../swmodule.h"
#include "../utilstr.h"


namespace swordxx {
namespace {

enum : unsigned { SE_OFF, SE_LATIN,
/*one-way (to) transliterators*/
SE_IPA, SE_BASICLATIN, SE_SBL, SE_TC, SE_BETA, SE_BGREEK, SE_SERA, SE_HUGOYE, SE_UNGEGN, SE_ISO, SE_ALALC, SE_BGN,
/*two-way transliterators*/
SE_GREEK, SE_HEBREW, SE_CYRILLIC, SE_ARABIC, SE_SYRIAC, SE_KATAKANA, SE_HIRAGANA, SE_HANGUL, SE_DEVANAGARI, SE_TAMIL, SE_BENGALI, SE_GURMUKHI, SE_GUJARATI, SE_ORIYA, SE_TELUGU, SE_KANNADA, SE_MALAYALAM, SE_THAI, SE_GEORGIAN, SE_ARMENIAN, SE_ETHIOPIC, SE_GOTHIC, SE_UGARITIC, SE_COPTIC, SE_MEROITIC, SE_LINEARB, SE_CYPRIOT, SE_RUNIC, SE_OGHAM, SE_THAANA, SE_GLAGOLITIC, SE_CHEROKEE,
/*SE_TENGWAR, SE_CIRTH,*/
/*one-way (from) transliterators*/
SE_JAMO, SE_HAN, SE_KANJI
};

constexpr unsigned const NUMSCRIPTS = SE_KANJI + 1u;

/// \copyright 2020 Jaak Ristioja
class ReplaceableUtf16String
        : public std::basic_string<char16_t>
        , public icu::Replaceable
{

public: /* Methods: */

    template <typename ... Args,
              typename =
                    std::enable_if_t<
                        std::is_constructible_v<
                            std::basic_string<char16_t>,
                            Args...
                        >
                    >>
    ReplaceableUtf16String(Args && ... args)
        noexcept(std::is_nothrow_default_constructible_v<icu::Replaceable>
                 && std::is_nothrow_constructible_v<
                            std::basic_string<char16_t>,
                            Args...
                        >)
        : std::basic_string<char16_t>(std::forward<Args>(args)...)
    {}

    using std::basic_string<char16_t>::operator=;

    void extractBetween(std::int32_t start,
                        std::int32_t limit,
                        icu::UnicodeString & target) const final override
    {
        static_assert(std::numeric_limits<std::int32_t>::max()
                      <= std::numeric_limits<size_type>::max(), "");
        assert(0 <= start);
        assert(start <= limit);
        assert(static_cast<size_type>(limit) < size());
        if (start >= limit) {
            target.remove();
        } else {
            target.setTo(data() + start, limit - start);
        }
    }

    void handleReplaceBetween(std::int32_t start,
                              std::int32_t limit,
                              icu::UnicodeString const & text) final override
    {
        static_assert(std::numeric_limits<std::int32_t>::max()
                      <= std::numeric_limits<size_type>::max(), "");
        assert(0 <= start);
        assert(start <= limit);
        assert(static_cast<size_type>(limit) < size());
        auto const pos = static_cast<size_type>(start);
        auto const n = static_cast<size_type>(limit - start);
        if (auto const * const buffer = text.getBuffer()) {
            replace(pos,
                    n,
                    text.getBuffer(),
                    static_cast<size_type>(text.length()));
        } else {
            erase(pos, n);
        }
    }

    void copy(std::int32_t start, std::int32_t limit, std::int32_t dest)
            final override
    {
        static_assert(std::numeric_limits<std::int32_t>::max()
                      <= std::numeric_limits<size_type>::max(), "");
        assert(0 <= start);
        assert(start <= limit);
        assert(static_cast<size_type>(limit) < size());
        assert(dest <= start || dest >= limit);

        auto const pos = static_cast<size_type>(start);
        auto const n = static_cast<size_type>(limit - start);
        insert(static_cast<size_type>(dest), *this, pos, n);
    }

    std::int32_t getLength() const final override {
        if (size() > std::numeric_limits<std::int32_t>::max())
            throw std::runtime_error("Implementation limits reached");
        return static_cast<std::int32_t>(size());
    }

    char16_t getCharAt(std::int32_t offset) const final override {
        static_assert(std::numeric_limits<std::int32_t>::max()
                      <= std::numeric_limits<size_type>::max(), "");
        assert(offset >= 0);
        assert(static_cast<size_type>(offset) < size());
        return (*this)[static_cast<size_type>(offset)];
    }

    ::UChar32 getChar32At(std::int32_t offset) const final override {
        auto const len = getLength();
        return icu::UnicodeString(data(), len, len).char32At(offset);
    }


    ::UBool hasMetaData() const final override { return false; }

    icu::Replaceable * clone() const final override
    { return new ReplaceableUtf16String(*this); }

};

auto createTrans(std::string const & id) {
    UErrorCode status = U_ZERO_ERROR;
    std::unique_ptr<icu::Transliterator> r(
                icu::Transliterator::createInstance(
                    icu::UnicodeString(id.c_str()),
                    UTRANS_FORWARD,
                    status));
    if (U_FAILURE(status))
        r.reset();
    return r;
}

} // anonymous namespace


UTF8Transliterator::UTF8Transliterator()
    : SWOptionFilter(
        "Transliteration",
        "Transliterates between scripts",
        {"Off", "Latin"})
{}


UTF8Transliterator::~UTF8Transliterator() = default;

char UTF8Transliterator::processText(std::string & text,
                                     SWKey const *,
                                     SWModule const * module)
{
    auto const selectedOptionIndex = getSelectedOptionValueIndex();

    // Return immediately if we don't want transliteration:
    if (!selectedOptionIndex)
        return 0;

    std::string id;
    auto const addTrans =
            [&id](auto && newTrans) {
                id += std::forward<decltype(newTrans)>(newTrans);
                id += ';';
            };

    bool compat = false;

    // Convert UTF-8 string to UTF-16 (UChars)
    ReplaceableUtf16String source(utf8ToUtf16(text));

    // Figure out which scripts are used in the string
    std::array<unsigned char, NUMSCRIPTS> scripts;
    for (auto & s : scripts)
        s = false;
    for (auto const uChar : source) {
        auto const ublockCode(::ublock_getCode(uChar));
        scripts[SE_LATIN] = true;
        switch (ublockCode) {
        #define C(ub,se) case ub: scripts[se] = true; break
        // C(BASIC_LATIN, LATIN);
        C(UBLOCK_GREEK, SE_GREEK);
        C(UBLOCK_HEBREW, SE_HEBREW);
        C(UBLOCK_CYRILLIC, SE_CYRILLIC);
        C(UBLOCK_ARABIC, SE_ARABIC);
        C(UBLOCK_SYRIAC, SE_SYRIAC);
        C(UBLOCK_KATAKANA, SE_KATAKANA);
        C(UBLOCK_HIRAGANA, SE_HIRAGANA);
        C(UBLOCK_HANGUL_SYLLABLES, SE_HANGUL);
        C(UBLOCK_HANGUL_JAMO, SE_JAMO);
        C(UBLOCK_DEVANAGARI, SE_DEVANAGARI);
        C(UBLOCK_TAMIL, SE_TAMIL);
        C(UBLOCK_BENGALI, SE_BENGALI);
        C(UBLOCK_GURMUKHI, SE_GURMUKHI);
        C(UBLOCK_GUJARATI, SE_GUJARATI);
        C(UBLOCK_ORIYA, SE_ORIYA);
        C(UBLOCK_TELUGU, SE_TELUGU);
        C(UBLOCK_KANNADA, SE_KANNADA);
        C(UBLOCK_MALAYALAM, SE_MALAYALAM);
        C(UBLOCK_THAI, SE_THAI);
        C(UBLOCK_GEORGIAN, SE_GEORGIAN);
        C(UBLOCK_ARMENIAN, SE_ARMENIAN);
        C(UBLOCK_ETHIOPIC, SE_ETHIOPIC);
        C(UBLOCK_GOTHIC, SE_GOTHIC);
        C(UBLOCK_UGARITIC, SE_UGARITIC);
        // C(UBLOCK_MEROITIC, SE_MEROITIC);
        C(UBLOCK_LINEAR_B_SYLLABARY, SE_LINEARB);
        C(UBLOCK_CYPRIOT_SYLLABARY, SE_CYPRIOT);
        C(UBLOCK_RUNIC, SE_RUNIC);
        C(UBLOCK_OGHAM, SE_OGHAM);
        C(UBLOCK_THAANA, SE_THAANA);
        C(UBLOCK_GLAGOLITIC, SE_GLAGOLITIC);
        C(UBLOCK_CHEROKEE, SE_CHEROKEE);
        // C(UBLOCK_TENGWAR, SE_TENGWAR);
        // C(UBLOCK_CIRTH, SE_CIRTH);
        #undef C
        case UBLOCK_CJK_COMPATIBILITY:
        case UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS:
        case UBLOCK_CJK_COMPATIBILITY_FORMS:
            compat = true;
            [[fallthrough]];
        case UBLOCK_CJK_RADICALS_SUPPLEMENT:
        case UBLOCK_KANGXI_RADICALS:
        case UBLOCK_IDEOGRAPHIC_DESCRIPTION_CHARACTERS:
        case UBLOCK_CJK_SYMBOLS_AND_PUNCTUATION:
        case UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A:
        case UBLOCK_CJK_UNIFIED_IDEOGRAPHS:
            scripts[SE_HAN] = true;
            break;
        case UBLOCK_HANGUL_COMPATIBILITY_JAMO:
            scripts[SE_HANGUL] = true;
            compat = true;
            break;
        default: break;
        }
    }

    // Turn off the reflexive transliteration:
    scripts[selectedOptionIndex] = false;

    { // Return if we have no transliteration to do for this text:
        bool needTransliteration = false;
        for (auto script : scripts) {
            if (script) {
                needTransliteration = true;
                break;
            }
        }
        if (!needTransliteration)
            return 0;
    }

    addTrans(compat ? "NFKD": "NFD");

    // Simple X to Latin transliterators:
    if (scripts[SE_GREEK]) {
        if (strnicmp(module->getLanguage().c_str(), "cop", 3)) {
            if (selectedOptionIndex == SE_SBL) {
                addTrans("Greek-Latin/SBL");
            } else if (selectedOptionIndex == SE_TC) {
                addTrans("Greek-Latin/TC");
            } else if (selectedOptionIndex == SE_BETA) {
                addTrans("Greek-Latin/Beta");
            } else if (selectedOptionIndex == SE_BGREEK) {
                addTrans("Greek-Latin/BGreek");
            } else if (selectedOptionIndex == SE_UNGEGN) {
                addTrans("Greek-Latin/UNGEGN");
            } else if (selectedOptionIndex == SE_ISO) {
                addTrans("Greek-Latin/ISO");
            } else if (selectedOptionIndex == SE_ALALC) {
                addTrans("Greek-Latin/ALALC");
            } else if (selectedOptionIndex == SE_BGN) {
                addTrans("Greek-Latin/BGN");
            } else if (selectedOptionIndex == SE_IPA) {
                addTrans("Greek-IPA/Ancient");
            } else {
                addTrans("Greek-Latin");
                scripts[SE_LATIN] = true;
            }
        } else {
            if (selectedOptionIndex == SE_SBL) {
                addTrans("Coptic-Latin/SBL");
            } else if (selectedOptionIndex == SE_TC) {
                addTrans("Coptic-Latin/TC");
            } else if (selectedOptionIndex == SE_BETA) {
                addTrans("Coptic-Latin/Beta");
            } else if (selectedOptionIndex == SE_IPA) {
                addTrans("Coptic-IPA");
            } else {
                addTrans("Coptic-Latin");
                scripts[SE_LATIN] = true;
            }
        }
    }
    if (scripts[SE_HEBREW]) {
        if (selectedOptionIndex == SE_SBL) {
            addTrans("Hebrew-Latin/SBL");
        } else if (selectedOptionIndex == SE_TC) {
            addTrans("Hebrew-Latin/TC");
        } else if (selectedOptionIndex == SE_BETA) {
            addTrans("Hebrew-Latin/Beta");
        } else if (selectedOptionIndex == SE_UNGEGN) {
            addTrans("Hebrew-Latin/UNGEGN");
        } else if (selectedOptionIndex == SE_ALALC) {
            addTrans("Hebrew-Latin/ALALC");
        } else if (selectedOptionIndex == SE_SYRIAC) {
            addTrans("Hebrew-Syriac");
        } else {
            addTrans("Hebrew-Latin");
            scripts[SE_LATIN] = true;
        }
    }
    if (scripts[SE_CYRILLIC]) {
        if (selectedOptionIndex == SE_GLAGOLITIC) {
            addTrans("Cyrillic-Glagolitic");
        } else {
            addTrans("Cyrillic-Latin");
            scripts[SE_LATIN] = true;
        }
    }
    if (scripts[SE_ARABIC]) {
        addTrans("Arabic-Latin");
        scripts[SE_LATIN] = true;
    }
    if (scripts[SE_SYRIAC]) {
        if (selectedOptionIndex == SE_TC) {
            addTrans("Syriac-Latin/TC");
        } else if (selectedOptionIndex == SE_BETA) {
            addTrans("Syriac-Latin/Beta");
        } else if (selectedOptionIndex == SE_HUGOYE) {
            addTrans("Syriac-Latin/Hugoye");
        } else if (selectedOptionIndex == SE_HEBREW) {
            addTrans("Syriac-Hebrew");
        } else {
            addTrans("Syriac-Latin");
            scripts[SE_LATIN] = true;
        }
    }
    if (scripts[SE_THAI]) {
        addTrans("Thai-Latin");
        scripts[SE_LATIN] = true;
    }
    if (scripts[SE_GEORGIAN]) {
        if (selectedOptionIndex == SE_ISO) {
            addTrans("Georgian-Latin/ISO");
        } else if (selectedOptionIndex == SE_ALALC) {
            addTrans("Georgian-Latin/ALALC");
        } else if (selectedOptionIndex == SE_BGN) {
            addTrans("Georgian-Latin/BGN");
        } else if (selectedOptionIndex == SE_IPA) {
            addTrans("Georgian-IPA");
        } else {
            addTrans("Georgian-Latin");
            scripts[SE_LATIN] = true;
        }
    }
    if (scripts[SE_ARMENIAN]) {
        if (selectedOptionIndex == SE_ISO) {
            addTrans("Armenian-Latin/ISO");
        } else if (selectedOptionIndex == SE_ALALC) {
            addTrans("Armenian-Latin/ALALC");
        } else if (selectedOptionIndex == SE_BGN) {
            addTrans("Armenian-Latin/BGN");
        } else if (selectedOptionIndex == SE_IPA) {
            addTrans("Armenian-IPA");
        } else {
            addTrans("Armenian-Latin");
            scripts[SE_LATIN] = true;
        }
    }
    if (scripts[SE_ETHIOPIC]) {
        if (selectedOptionIndex == SE_UNGEGN) {
            addTrans("Ethiopic-Latin/UNGEGN");
        } else if (selectedOptionIndex == SE_ISO) {
            addTrans("Ethiopic-Latin/ISO");
        } else if (selectedOptionIndex == SE_ALALC) {
            addTrans("Ethiopic-Latin/ALALC");
        } else if (selectedOptionIndex == SE_SERA) {
            addTrans("Ethiopic-Latin/SERA");
        } else {
            addTrans("Ethiopic-Latin");
            scripts[SE_LATIN] = true;
        }
    }
    if (scripts[SE_GOTHIC]) {
        if (selectedOptionIndex == SE_BASICLATIN) {
            addTrans("Gothic-Latin/Basic");
        } else if (selectedOptionIndex == SE_IPA) {
            addTrans("Gothic-IPA");
        } else {
            addTrans("Gothic-Latin");
            scripts[SE_LATIN] = true;
        }
    }
    if (scripts[SE_UGARITIC]) {
        if (selectedOptionIndex == SE_SBL) {
            addTrans("Ugaritic-Latin/SBL");
        } else {
            addTrans("Ugaritic-Latin");
            scripts[SE_LATIN] = true;
        }
    }
    if (scripts[SE_MEROITIC]) {
        addTrans("Meroitic-Latin");
        scripts[SE_LATIN] = true;
    }
    if (scripts[SE_LINEARB]) {
        addTrans("LinearB-Latin");
        scripts[SE_LATIN] = true;
    }
    if (scripts[SE_CYPRIOT]) {
        addTrans("Cypriot-Latin");
        scripts[SE_LATIN] = true;
    }
    if (scripts[SE_RUNIC]) {
        addTrans("Runic-Latin");
        scripts[SE_LATIN] = true;
    }
    if (scripts[SE_OGHAM]) {
        addTrans("Ogham-Latin");
        scripts[SE_LATIN] = true;
    }
    if (scripts[SE_THAANA]) {
        if (selectedOptionIndex == SE_ALALC) {
            addTrans("Thaana-Latin/ALALC");
        } else if (selectedOptionIndex == SE_BGN) {
            addTrans("Thaana-Latin/BGN");
        } else {
            addTrans("Thaana-Latin");
            scripts[SE_LATIN] = true;
        }
    }
    if (scripts[SE_GLAGOLITIC]) {
        if (selectedOptionIndex == SE_ISO) {
            addTrans("Glagolitic-Latin/ISO");
        } else if (selectedOptionIndex == SE_ALALC) {
            addTrans("Glagolitic-Latin/ALALC");
        } else if (selectedOptionIndex == SE_ALALC) {
            addTrans("Glagolitic-Cyrillic");
        } else {
            addTrans("Glagolitic-Latin");
            scripts[SE_LATIN] = true;
        }
    }
    if (scripts[SE_CHEROKEE]) {
        addTrans("Cherokee-Latin");
        scripts[SE_LATIN] = true;
    }
    if (scripts[SE_THAI]) {
        addTrans("Thai-Latin");
        scripts[SE_LATIN] = true;
    }
    if (scripts[SE_THAI]) {
        addTrans("Thai-Latin");
        scripts[SE_LATIN] = true;
    }

    if (scripts[SE_HAN]) {
        if (!strnicmp(module->getLanguage().c_str(), "ja", 2)) {
            addTrans("Kanji-Romaji");
        } else {
            addTrans("Han-Latin");
        }
        scripts[SE_LATIN] = true;
    }

    // Inter-Kana and Kana to Latin transliterators:
    if (selectedOptionIndex == SE_HIRAGANA && scripts[SE_KATAKANA]) {
        addTrans("Katakana-Hiragana");
        scripts[SE_HIRAGANA] = true;
    } else if (selectedOptionIndex == SE_KATAKANA && scripts[SE_HIRAGANA]) {
        addTrans("Hiragana-Katakana");
        scripts[SE_KATAKANA] = true;
    } else {
        if (scripts[SE_KATAKANA]) {
            addTrans("Katakana-Latin");
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_HIRAGANA]) {
            addTrans("Hiragana-Latin");
            scripts[SE_LATIN] = true;
        }
    }

    // Korean to Latin transliterators:
    if (scripts[SE_HANGUL]) {
        addTrans("Hangul-Latin");
        scripts[SE_LATIN] = true;
    }
    if (scripts[SE_JAMO]) {
        addTrans("Jamo-Latin");
        scripts[SE_LATIN] = true;
    }

    // Indic-Latin:
    if (selectedOptionIndex < SE_DEVANAGARI
        || selectedOptionIndex > SE_MALAYALAM)
    {
        // Indic to Latin:
        if (scripts[SE_TAMIL]) {
            addTrans("Tamil-Latin");
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_BENGALI]) {
            addTrans("Bengali-Latin");
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_GURMUKHI]) {
            addTrans("Gurmukhi-Latin");
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_GUJARATI]) {
            addTrans("Gujarati-Latin");
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_ORIYA]) {
            addTrans("Oriya-Latin");
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_TELUGU]) {
            addTrans("Telugu-Latin");
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_KANNADA]) {
            addTrans("Kannada-Latin");
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_MALAYALAM]) {
            addTrans("Malayalam-Latin");
            scripts[SE_LATIN] = true;
        }
    } else {
        if (scripts[SE_LATIN])
            addTrans("Latin-InterIndic");
        if (scripts[SE_DEVANAGARI])
            addTrans("Devanagari-InterIndic");
        if (scripts[SE_TAMIL])
            addTrans("Tamil-InterIndic");
        if (scripts[SE_BENGALI])
            addTrans("Bengali-InterIndic");
        if (scripts[SE_GURMUKHI])
            addTrans("Gurmurkhi-InterIndic");
        if (scripts[SE_GUJARATI])
            addTrans("Gujarati-InterIndic");
        if (scripts[SE_ORIYA])
            addTrans("Oriya-InterIndic");
        if (scripts[SE_TELUGU])
            addTrans("Telugu-InterIndic");
        if (scripts[SE_KANNADA])
            addTrans("Kannada-InterIndic");
        if (scripts[SE_MALAYALAM])
            addTrans("Malayalam-InterIndic");

        switch(selectedOptionIndex) {
        #define C(se,ses) case se: addTrans("InterIndic-" ses); break
        C(SE_DEVANAGARI, "Devanagari");
        C(SE_TAMIL, "Tamil");
        C(SE_BENGALI, "Bengali");
        C(SE_GURMUKHI, "Gurmukhi");
        C(SE_GUJARATI, "Gujarati");
        C(SE_ORIYA, "Oriya");
        C(SE_TELUGU, "Telugu");
        C(SE_KANNADA, "Kannada");
        C(SE_MALAYALAM, "Malayalam");
        #undef C
        default:
            addTrans("InterIndic-Latin");
            scripts[SE_LATIN] = true;
            break;
        }
    }

    #if 0
    if (scripts[SE_TENGWAR]) {
        addTrans("Tengwar-Latin");
        scripts[SE_LATIN] = true;
    }
    if (scripts[SE_CIRTH]) {
        addTrans("Cirth-Latin");
        scripts[SE_LATIN] = true;
    }
    #endif

    if (scripts[SE_LATIN]) {
        switch (selectedOptionIndex) {
        #define C(se,ses) case se: addTrans("Latin-" ses); break
        C(SE_GREEK, "Greek");
        C(SE_HEBREW, "Hebrew");
        C(SE_CYRILLIC, "Cyrillic");
        C(SE_ARABIC, "Arabic");
        C(SE_SYRIAC, "Syriac");
        C(SE_THAI, "Thai");
        C(SE_GEORGIAN, "Georgian");
        C(SE_ARMENIAN, "Armenian");
        C(SE_ETHIOPIC, "Ethiopic");
        C(SE_GOTHIC, "Gothic");
        C(SE_UGARITIC, "Ugaritic");
        C(SE_COPTIC, "Coptic");
        C(SE_KATAKANA, "Katakana");
        C(SE_HIRAGANA, "Hiragana");
        C(SE_JAMO, "Jamo");
        C(SE_HANGUL, "Hangul");
        C(SE_MEROITIC, "Meroitic");
        C(SE_LINEARB, "LinearB");
        C(SE_CYPRIOT, "Cypriot");
        C(SE_RUNIC, "Runic");
        C(SE_OGHAM, "Ogham");
        C(SE_THAANA, "Thaana");
        C(SE_GLAGOLITIC, "Glagolitic");
        C(SE_CHEROKEE, "Cherokee");
        // C(SE_TENGWAR, "Tengwar");
        // C(SE_CIRTH:, "Cirth");
        #undef C
        }
    }

    if (selectedOptionIndex == SE_BASICLATIN)
        addTrans("Any-Latin1");

    addTrans("NFC");

    if (auto const trans = createTrans(id)) {
        trans->transliterate(source);
        text = utf16ToUtf8(source);
    }
    return 0;
}

} /* namespace swordxx */
