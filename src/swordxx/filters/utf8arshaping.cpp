/******************************************************************************
 *
 *  utf8arshaping.cpp -    SWFilter descendant to perform Arabic shaping on
 *            UTF-8 text
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
 * Copyright 2020 Jaak Ristioja
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

#include "utf8arshaping.h"

#include "../utilstr.h"


namespace swordxx {

char UTF8arShaping::processText(std::string & text,
                                SWKey const * key,
                                SWModule const *)
{
    if ((unsigned long)key < 2)    // hack, we're en(1)/de(0)ciphering
        return -1;

    text = utf8ShapeArabic(text);
    return 0;
}

} /* namespace swordxx */
