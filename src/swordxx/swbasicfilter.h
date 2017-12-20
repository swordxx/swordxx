/******************************************************************************
 *
 *  swbasicfilter.h -    definition of class SWBasicFilter.  An SWFilter
 *                     impl that provides some basic methods that
 *                     many filter will need and can use as a starting
 *                 point
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

#ifndef SWBASICFILTER_H
#define SWBASICFILTER_H

#include <map>
#include <memory>
#include <set>
#include <string>
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

    virtual ~BasicFilterUserData() {}

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

    enum StageFlags: char {
        INITIALIZE = 1, // indicates processing before char loop
        PRECHAR    = 2, // indicates processing at top in char loop
        POSTCHAR   = 4, // indicates processing at bottom in char loop
        FINALIZE   = 8  // indicates processing after char loop
    };

    /** Sets the beginning of token start sequence (by default "<").*/
    void setTokenStart(char const * tokenStart)
    { m_tokenStart = tokenStart; }

    /** Sets the end of token start sequence (by default ">").*/
    void setTokenEnd(char const * tokenEnd)
    { m_tokenEnd = tokenEnd; }

    /** Sets the beginning of escape sequence (by default "&").*/
    void setEscapeStart(char const * escStart)
    { m_escStart = escStart; }

    /** Sets the end of escape sequence (by default ";").*/
    void setEscapeEnd(char const * escEnd)
    { m_escEnd = escEnd; }

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
    { m_passThruUnknownEsc = val; }

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

    void setStageProcessing(char stages) { m_processStages = stages; }

    /** Registers an esc control sequence that can pass unchanged
     */
    void addAllowedEscapeString(char const * findString);

    /** Unregisters an esc control sequence that can pass unchanged
     */
    void removeAllowedEscapeString(char const * findString)
    { m_escPassSet.erase(findString); }

    /** Registers an esc control sequence
     */
    void addEscapeStringSubstitute(char const * findString,
                                   char const * replaceString);

    /** Unregisters an esc control sequence
     */
    void removeEscapeStringSubstitute(char const * findString)
    { m_escSubMap.erase(findString); }

    /** This function performs the substitution of escapeStrings */
    bool substituteEscapeString(std::string & buf, char const * escString);

    /** This passes allowed escapeStrings */
    bool passAllowedEscapeString(std::string & buf, char const * escString);

    /** This appends escString to buf as an entity */
    std::string & appendEscapeString(std::string & buf, char const * escString)
    { return buf.append(m_escStart).append(escString).append(m_escEnd); }

    /** Registers a simple token substitutions.  Usually called from the
     *    c-tor of a subclass
     */
    void addTokenSubstitute(char const * findString,
                            char const * replaceString);

    /** Unregisters a simple token substitute
     */
    void removeTokenSubstitute(char const * findString)
    { m_tokenSubMap.erase(findString); }

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

    virtual bool processStage(char stage,
                              std::string & text,
                              char *& from,
                              BasicFilterUserData * userData)
    {
        (void) stage; (void) text; (void) from; (void) userData;
        return false;
    }

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

    std::string m_tokenStart{'<'};
    std::string m_tokenEnd{'>'};
    std::string m_escStart{'&'};
    std::string m_escEnd{';'};
    bool m_passThruUnknownToken = false;
    bool m_passThruUnknownEsc = false;
    bool m_passThruNumericEsc = false;
    bool m_tokenCaseSensitive = false;
    bool m_escStringCaseSensitive = false;
    char m_processStages = 0;

    std::map<std::string, std::string> m_tokenSubMap;
    std::map<std::string, std::string> m_escSubMap;
    std::set<std::string> m_escPassSet;

};

} /* namespace swordxx */

#endif
