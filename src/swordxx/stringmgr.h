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

    virtual ~StringMgr() noexcept;

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
    virtual void upperUTF8(std::string & str) const;

    /** Converts the param to an uppercase latin1 string
    * @param text The text encoded in latin1 which should be turned into an upper case string
    * @param max Max buffer size
    * @return text buffer (only for convenience)
    */
    virtual void upperLatin1(std::string & str) const;


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

inline void toupperstr(std::string & str)
{ return StringMgr::getSystemStringMgr()->upperUTF8(str); }

inline void toupperstr_utf8(std::string & str)
{ return StringMgr::getSystemStringMgr()->upperUTF8(str); }

} /* namespace swordxx */


#endif //STRINGMGR_H
