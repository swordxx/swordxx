/******************************************************************************
 *
 *  diafiltmgr.h -	DiathekeFilterMgr
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef DIAFILTMGR_H
#define DIAFILTMGR_H

#define FMT_CGI 127

#include <encfiltmgr.h>

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

class DiathekeFilterMgr : public EncodingFilterMgr {
protected:
        SWFilter* fromthml;
        SWFilter* fromgbf;
        SWFilter* fromplain;
        SWFilter* fromosis;

        char markup;

        void CreateFilters(char markup);
public:
        DiathekeFilterMgr(char markup = FMT_THML, char encoding = ENC_UTF8);
        ~DiathekeFilterMgr();
        char Markup(char m = FMT_UNKNOWN);
        virtual void AddRenderFilters(SWModule *module, ConfigEntMap &section);
};

#endif
