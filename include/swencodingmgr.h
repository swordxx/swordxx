/******************************************************************************
 *  swencodingmgr.h   - definition of class SWEncodingMgr, subclass of
 *                        used to transcode all module text to a requested
 *                        markup.
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
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

#ifndef SWENCODINGMGR_H
#define SWENCODINGMGR_H

#include <swmgr.h>

class SWDLLEXPORT SWEncodingMgr : public SWMgr
{
protected:
        SWFilter *latin1utf8;
        SWFilter *scsuutf8;
        SWFilter *targetenc;
        
        char encoding;

public:
        SWEncodingMgr (SWConfig * iconfig = 0, SWConfig * isysconfig = 0, bool autoload = true, char encoding = ENC_UTF8);
        ~SWEncodingMgr();

        char Encoding(char enc);

        virtual void AddRawFilters(SWModule *module, ConfigEntMap &section);
        virtual void AddEncodingFilters(SWModule *module, ConfigEntMap &section);
};
#endif
