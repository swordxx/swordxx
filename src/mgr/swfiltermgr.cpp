/******************************************************************************
 *  swfiltermgr.cpp   - definition of class SWFilterMgr used as an interface to
 *				manage filters on a module
 *
 * $Id: swfiltermgr.cpp,v 1.1 2001/11/30 11:16:15 scribe Exp $
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

#include <swfiltermgr.h>


void SWFilterMgr::setParentMgr(SWMgr *parentMgr) {
  this->parentMgr = parentMgr;
}

SWMgr *SWFilterMgr::getParentMgr() {
  return parentMgr;
}

