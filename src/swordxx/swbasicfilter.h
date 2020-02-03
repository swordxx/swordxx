/******************************************************************************
 *
 *  swbasicfilter.h -    definition of class SWBasicFilter.  An SWFilter
 *                     impl that provides some basic methods that
 *                     many filter will need and can use as a starting
 *                 point
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

#ifndef SWORDXX_SWBASICFILTER_H
#define SWORDXX_SWBASICFILTER_H

#include <map>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include "keys/versekey.h"
#include "swfilter.h"


namespace swordxx {

// not a protected inner class because MSVC++ sucks and can't handle it
struct SWDLLEXPORT BasicFilterUserData {

/* Methods: */

    BasicFilterUserData(SWModule const * const module_,
                        SWKey const * const key_)
        : module(module_)
        , key(key_)
    {}

    virtual ~BasicFilterUserData() noexcept;

/* Fields: */

    SWModule const * const module;
    SWKey const * const key;
    VerseKey const * const verseKey = dynamic_cast<VerseKey const *>(key);
    std::string lastTextNode;
    std::string lastSuspendSegment;
    bool suspendTextPassThru = false;
    bool supressAdjacentWhitespace = false;

};

/** A filter providing commonly used functionality.
 * This filter has facilities for handling SGML/HTML/XML like tokens and
 * escape strings (like SGML entities). It has the facility for just
 * substituting the given tokens and escape strings to other strings and for
 * "manual" custom token handling.
 *
 * In this class the functions with arguments looking as <code>char
 * **buf</code> write a character sequnce at address specified by
 * <code>*buf</code> address and change <code>*buf</code> to point past
 * the last char of the written sequence.
 */
class SWDLLEXPORT SWBasicFilter: public virtual SWFilter {

public: /* Methods: */

    char processText(std::string & text,
                     SWKey const * key = nullptr,
                     SWModule const * module = nullptr) override;

protected: /* Methods: */

    virtual std::unique_ptr<BasicFilterUserData> createUserData(
            SWModule const * const module,
            SWKey const * const key);

    /** Sets whether to pass thru an unknown token unchanged
     *    or just remove it.
     * Default is false.*/
    void setPassThruUnknownToken(bool val)
    { m_passThruUnknownToken = val; }

    /** Sets whether to pass thru an unknown escape sequence unchanged
     *    or just remove it.
     *    Default is false.
     */
    void setPassThruUnknownEscapeString(bool val)
    { m_passThruUnknownEsc = val; }

    /** Sets whether to pass thru a numeric escape sequence unchanged
     *    or allow it to be handled otherwise.
     * Default is false.*/
    void setPassThruNumericEscapeString(bool val)
    { m_passThruNumericEsc = val; }

    /** Are tokens case sensitive (like in GBF) or not? Call this
     *    function before addTokenSubstitute()
     */
    void setTokenCaseSensitive(bool val)
    { m_tokenCaseSensitive = val; }

    /** Are escapeStrings case sensitive or not? Call this
     *    function before addEscapeStingSubstitute()
     */
    void setEscapeStringCaseSensitive(bool val)
    { m_escStringCaseSensitive = val; }

    /** Registers an esc control sequence that can pass unchanged
     */
    void addAllowedEscapeString(char const * findString);

    /** Registers an esc control sequence
     */
    void addEscapeStringSubstitute(char const * findString,
                                   char const * replaceString);

    /** This function performs the substitution of escapeStrings */
    bool substituteEscapeString(std::string & buf, char const * escString);

    /** This passes allowed escapeStrings */
    bool passAllowedEscapeString(std::string & buf, char const * escString);

    /** This appends escString to buf as an entity */
    static std::string &
    appendEscapeString(std::string & buf, std::string_view escString)
    { return buf.append(1u, '&').append(escString).append(1u, ';'); }

    /** Registers a simple token substitutions.  Usually called from the
     *    c-tor of a subclass
     */
    void addTokenSubstitute(char const * findString,
                            char const * replaceString);

    /** This function performs the substitution of tokens */
    bool substituteToken(std::string & buf, char const * token);

    /** This function is called for every token encountered in the input text.
     * @param buf the output buffer
     * @param token the token (e.g. <code>"p align='left'"</code>
     * @param userData user storage space for data transient to 1 full buffer parse
     * @return subclasses should return true if they handled the token, or false if they did not.
     */
    virtual bool handleToken(std::string & buf,
                             char const * token,
                             BasicFilterUserData * userData);

    virtual bool processPrechar(std::string & text,
                                std::string_view & view,
                                BasicFilterUserData * userData);

    /** This function is called for every escape sequence encountered in the input text.
     * @param buf the output buffer
     * @param escString the escape sequence (e.g. <code>"amp"</code> for &amp;amp;)
     * @param userData user storage space for data transient to 1 full buffer parse
     * @return <code>false</code> if was not handled and should be handled in
     * @return subclasses should return true if they handled the esc seq, or false if they did not.
     */
    virtual bool handleEscapeString(std::string & buf,
                                    char const * escString,
                                    BasicFilterUserData * userData);

    /** This function is called for all numeric escape sequences. If passThrough
     * @param buf the output buffer
     * @param escString the escape sequence (e.g. <code>"#235"</code> for &amp;235;)
     * @return subclasses should return true if they handled the esc seq, or false if they did not.
         */
    virtual bool handleNumericEscapeString(std::string & buf,
                                           char const * escString);

private: /* Fields: */

    bool m_passThruUnknownToken = false;
    bool m_passThruUnknownEsc = false;
    bool m_passThruNumericEsc = false;
    bool m_tokenCaseSensitive = false;
    bool m_escStringCaseSensitive = false;

    std::map<std::string, std::string> m_tokenSubMap;
    std::map<std::string, std::string> m_escSubMap;
    std::set<std::string> m_escPassSet;

};

} /* namespace swordxx */

#endif /* SWORDXX_SWBASICFILTER_H */
