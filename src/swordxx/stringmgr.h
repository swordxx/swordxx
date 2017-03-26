/******************************************************************************
 *
 *  stringmgr.h -    A class which provides string handling functions which
 *            can be reimplemented by frontends
 *
 * $Id$
 *
 * Copyright 2004-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ    85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the GNU
 * General Public License for more details.
 *
 */


#ifndef STRINGMGR_H
#define STRINGMGR_H

#include <cstring>
#include <memory>
#include "defs.h"
#include "utilstr.h"


namespace swordxx {

/** StringMgr is a way to provide UTf8 handling by the Sword++ frontend
 * Each platform, if it's up-to-date, should provide functions to handle unicode and utf8. This class makes it possible to implement Unicode support on the user-side and not in Sword++ itself.
 */
class SWDLLEXPORT StringMgr {
public:

    /** Sets the global StringMgr handle
    * @param newStringMgr The new global StringMgr. This pointer will be deleted by this StringMgr
    */
    static void setSystemStringMgr(StringMgr *newStringMgr);

    /** Returns the global StringMgr handle
    * @return The global string handle
    */
    static StringMgr *getSystemStringMgr();

    /** Checks whether Utf8 support is available.
    * Override the function supportsUnicode() to tell whether your implementation has utf8 support.
    * @return True if this implementation provides support for Utf8 handling or false if just latin1 handling is available
    */
    static inline bool hasUTF8Support() {
        return getSystemStringMgr()->supportsUnicode();
    }

    /** Converts the param to an upper case Utf8 string
    * @param text The text encoded in utf8 which should be turned into an upper case string
    * @param max Max buffer size
    * @return text buffer (only for convenience)
    */
    virtual char * upperUTF8(char * text, std::size_t const max = 0u) const;

    /** Converts the param to an uppercase latin1 string
    * @param text The text encoded in latin1 which should be turned into an upper case string
    * @param max Max buffer size
    * @return text buffer (only for convenience)
    */
    virtual char * upperLatin1(char * text, std::size_t const max = 0u) const;


protected:

    /** Default constructor. Protected to make instances on user side impossible, because this is a Singleton
    */
    StringMgr();

    /** Copy constructor
    */
    StringMgr(const StringMgr &);

    virtual bool supportsUnicode() const;

private:
    static std::unique_ptr<StringMgr> systemStringMgr;
};

inline char * toupperstr(char * t, std::size_t const max = 0u)
{ return StringMgr::getSystemStringMgr()->upperUTF8(t, max); }

inline char * toupperstr_utf8(char * t, std::size_t const max = 0u)
{ return StringMgr::getSystemStringMgr()->upperUTF8(t, max); }

/**
 * Converts an std::string filled with UTF-8 to upper case
 *
 * @param b std::string to change to upper case
 *
 * @return b for convenience
 */
inline std::string &toupperstr(std::string & b) {
    auto const utf8(std::make_unique<char[]>((b.size() + 1u) * 2u));
    std::strcpy(utf8.get(), b.c_str());
    toupperstr(utf8.get(), b.size() * 2u);
    return b = utf8.get();
}

} /* namespace swordxx */


#endif //STRINGMGR_H
