/******************************************************************************
 *  swmarkupmgr.h   - definition of class SWMarkupMgr, subclass of
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

#ifndef SWMARKUPMGR_H
#define SWMARKUPMGR_H

#include <swencodingmgr.h>

class SWDLLEXPORT SWMarkupMgr : public SWEncodingMgr
{
protected:
        SWFilter* fromthml;
        SWFilter* fromgbf;
        SWFilter* fromplain;
        SWFilter* fromosis;

        char markup;

        void CreateFilters(char markup);
public:
        SWMarkupMgr (SWConfig * iconfig = 0, SWConfig * isysconfig = 0, bool autoload = true, char encoding = ENC_UTF8, char markup = FMT_THML);
        ~SWMarkupMgr();

        char Markup(char m = FMT_UNKNOWN);

        virtual void AddRenderFilters(SWModule *module, ConfigEntMap &section);
};

#endif
