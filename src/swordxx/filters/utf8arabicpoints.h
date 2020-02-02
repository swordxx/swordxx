/******************************************************************************
 *
 *  utf8arabicpoints.h -    SWFilter descendant to remove UTF-8 Arabic
 *                vowel points
 *
 * Copyright 2009-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_UTF8ARABICPOINTS_H
#define SWORDXX_UTF8ARABICPOINTS_H

#include "../onoffoptionfilter.h"


namespace swordxx {

/** This Filter shows/hides Arabic vowel points in UTF8 text
 */
class SWDLLEXPORT UTF8ArabicPoints: public OnOffOptionFilter {
public:
    UTF8ArabicPoints();
    ~UTF8ArabicPoints() override;

    char processText(std::string & text,
                         SWKey const * key = nullptr,
                         SWModule const * module = nullptr) override;

};

} /* namespace swordxx */

#endif /* SWORDXX_UTF8ARABICPOINTS_H */
