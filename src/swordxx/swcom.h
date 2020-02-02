/******************************************************************************
 *
 *  swcom.h -    code for base class 'SWCom'.  SWCom is the basis for all
 *        types of commentary modules.
 *        It is traditionally close to the same as the SWText
 *        module type, but has defaults set which more
 *        closely represent how a commentary will be used and how
 *        the bulk of our commentaries expect to be used.  For example,
 *        most commentaries consist of entries for a range of text
 *        (Matt.1.1-6: "Matthew begins with by telling us...").  This
 *        same entry will be returned when any verse between Matt.1.1-6
 *        is requested from the engine.  This is done with verse linking.
 *        Since linking is used heavily in commentaries, the flag
 *        skipConsecutiveLinks is defaulted to true so when the
 *        commentary is incremented, it will go to the next ENTRY,
 *        not the next verse (likely Matt.1.7, from our example above).
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

#ifndef SWORDXX_SWCOM_H
#define SWORDXX_SWCOM_H

#include "swmodule.h"

#include "defs.h"
#include "keys/versekey.h"


namespace swordxx {

class VerseKey;
class SWKey;

  /** The basis for all commentary modules
  */
class SWDLLEXPORT SWCom : public SWModule {

protected:
    std::shared_ptr<VerseKey const> getVerseKey(
            std::shared_ptr<SWKey const> key = nullptr) const;


public:

    SWCom(char const * imodname = nullptr,
          char const * imoddesc = nullptr,
          TextEncoding enc = ENC_UNKNOWN,
          SWTextDirection dir = DIRECTION_LTR,
          SWTextMarkup mark = FMT_UNKNOWN,
          char const * ilang = nullptr,
          std::shared_ptr<VersificationMgr::System const> v11n = nullptr);

    std::shared_ptr<SWKey> createKey() const override;

private: /* Methods: */

    SWCom(char const * imodname,
          char const * imoddesc,
          TextEncoding enc,
          SWTextDirection dir,
          SWTextMarkup mark,
          char const * ilang,
          std::shared_ptr<VerseKey> key);

private: /* Fields: */

    std::shared_ptr<VersificationMgr::System const> const m_v11n;

};

} /* namespace swordxx */

#endif /* SWORDXX_SWCOM_H */
