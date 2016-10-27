/******************************************************************************
 *
 *  utf8greekaccents.h -    Implementation of UTF8GreekAccents
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

#ifndef UTF8GREEKACCENTS_H
#define UTF8GREEKACCENTS_H

#include "../onoffoptionfilter.h"


namespace swordxx {

  /** This Filter shows/hides Greek Accents marks in UTF8 text
  */
class SWDLLEXPORT UTF8GreekAccents: public OnOffOptionFilter {
public:
    UTF8GreekAccents();
    ~UTF8GreekAccents() override;

    char processText(std::string & text,
                         SWKey const * key = nullptr,
                         SWModule const * module = nullptr) override;

};

} /* namespace swordxx */
#endif
