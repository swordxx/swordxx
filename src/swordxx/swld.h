/******************************************************************************
 *
 *  swld.h -    code for base class 'SWLD'.  SWLD is the basis for all
 *        types of Lexicon and Dictionary modules (hence the 'LD').
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

#ifndef SWORDXX_SWLD_H
#define SWORDXX_SWLD_H

#include "swmodule.h"

#include "defs.h"


namespace swordxx {

  /** the basis for all types of Lexicon and
  * Dictionary modules (hence the 'LD').
  */
class SWDLLEXPORT SWLD : public SWModule {
protected:
    mutable std::string m_entkeytxt;
    static std::unique_ptr<char[]> strongsPadBuf(char const * buf);
    bool m_strongsPadding;
public:
    /** Initializes data for instance of SWLD
    */
    SWLD(char const * imodname = nullptr,
         char const * imoddesc = nullptr,
         TextEncoding encoding = ENC_UNKNOWN,
         SWTextDirection dir = DIRECTION_LTR,
         SWTextMarkup markup = FMT_UNKNOWN,
         char const * ilang = nullptr,
         bool strongsPadding = true);

    std::shared_ptr<SWKey> createKey() const override;

    std::string getKeyText() const override;
    void positionToTop() override;
    void positionToBottom() override;

    virtual long getEntryCount() const = 0;
    virtual long getEntryForKey(const char *key) const = 0;
    virtual std::string getKeyForEntry(long entry) const = 0;

    bool hasEntry(SWKey const & k) const override;

};

} /* namespace swordxx */

#endif /* SWORDXX_SWLD_H */
