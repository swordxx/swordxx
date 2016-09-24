/******************************************************************************
 *
 *  swld.h -    code for base class 'SWLD'.  SWLD is the basis for all
 *        types of Lexicon and Dictionary modules (hence the 'LD').
 *
 * $Id$
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWLD_H
#define SWLD_H

#include "swmodule.h"

#include "defs.h"


namespace swordxx {

  /** the basis for all types of Lexicon and
  * Dictionary modules (hence the 'LD').
  */
class SWDLLEXPORT SWLD : public SWModule {
protected:
    mutable char *entkeytxt;
    static void strongsPad(char *buf);
    bool strongsPadding;
public:
    /** Initializes data for instance of SWLD
    */
    SWLD(char const * imodname = nullptr,
         char const * imoddesc = nullptr,
         SWTextEncoding encoding = ENC_UNKNOWN,
         SWTextDirection dir = DIRECTION_LTR,
         SWTextMarkup markup = FMT_UNKNOWN,
         char const * ilang = nullptr,
         bool strongsPadding = true);

    virtual ~SWLD();
    virtual SWKey *createKey() const;

    /** Sets/gets module KeyText, getting from saved text if key is persistent
    *
    * @param ikeytext value which to set keytext;
    *  [0] - only get
    * @return pointer to keytext
    */
    virtual const char *getKeyText() const;
    virtual void setPosition(SW_POSITION pos);


    virtual long getEntryCount() const = 0;
    virtual long getEntryForKey(const char *key) const = 0;
    virtual char *getKeyForEntry(long entry) const = 0;

    virtual bool hasEntry(const SWKey *k) const;

    // OPERATORS -----------------------------------------------------------------

    SWMODULE_OPERATORS

};

} /* namespace swordxx */
#endif
