/******************************************************************************
 *	refsys.cpp - code for class 'refsys'- Biblical reference system
 *
 * $Id$
 *
 * Copyright 2004 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ	85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU
 * General Public License for more details.
 *
 */

#include <refsys.h>
#include <utilstr.h>

SWORD_NAMESPACE_START

//#include <kjvref.h>

//bkref         *RefSys::offsets[2]  = {RefSys::kjvbks, RefSys::kjvcps};

RefSys::RefSys(const char *name, const bkref *offsets[2], const int offsize[2])
{
	m_name = 0;
	stdstr(&m_name, name);
	if (offsets)
	{
		m_offsets[0] = offsets[0];
		m_offsets[1] = offsets[1];
		m_offsize[0] = offsize[0]; 
		m_offsize[1] = offsize[1]; 
	}
	#if 0
	else
	{
		m_offsets[0] = RefSys::kjvbks;
		m_offsets[1] = RefSys::kjvcps;
		m_offsize[0] = NTBOOKS+OTBOOKS+3; 
		m_offsize[1] = sizeof (RefSys::kjvcps) / sizeof(bkref); 
	}
	#endif
}

RefSys::~RefSys()
{
	delete[] m_name;
	//if (m_offsets[0] != RefSys::kjvbks)
	if (m_offsets[0])
		delete[] m_offsets[0]; 
	//if (m_offsets[1] != RefSys::kjvcps)
	if (m_offsets[1])
		delete[] m_offsets[1]; 
}

/******************************************************************************
 * RefSys::findindex - binary search to find the index closest, but less
 *						than the given value.
 *
 * ENT:	array	- long * to array to search
 *		size		- number of elements in the array
 *		value	- value to find
 *
 * RET:	the index into the array that is less than but closest to value
 */

int RefSys::findindex(const bkref *array, int size, long value) const
{
	int lbound, ubound, tval;

	lbound = 0;
	ubound = size - 1;
	while ((ubound - lbound) > 1) {
		tval = lbound + (ubound-lbound)/2;
		if (array[tval].offset <= value)
			lbound = tval;
		else ubound = tval;
	}
	return (array[ubound].offset <= value) ? ubound : lbound;
}

void RefSys::getBCV(long iindex, char *book, int *chapter, int *verse) const
{
	long offset;
	offset  = findindex(m_offsets[1], m_offsize[1], iindex);
	*verse   = iindex - m_offsets[1][offset].offset;
	*book    = findindex(m_offsets[0], m_offsize[0], offset);
	*chapter = offset - m_offsets[0][*book].offset;
	if (m_offsets[0][*book].maxnext == TESTAMENT_HEADING) // if we want testament heading
	{
		*chapter = 0;
		*verse = 0;
	}
	#ifdef WDGDEBUG
	printf("offset %d:%d:%d bcv %d:%d:%d\n", offset, 
		m_offsets[1][offset].offset, m_offsets[0][*book].offset, 
		*book, *chapter, *verse);fflush(NULL);
	#endif

}

const int RefSys::getMaxChaptersInBook(char book) const {
	return (m_offsets[0][book].maxnext == TESTAMENT_HEADING ? 0 : m_offsets[0][book].maxnext);
}

const int RefSys::getMaxVerseInChapter(char book, int chapter) const {
	return m_offsets[1][(m_offsets[0][book].offset)+chapter].maxnext;
}

const long RefSys::getIndex(char book, int chapter, int verse) const {
	long  loffset;
	loffset = m_offsets[0][book].offset;
	loffset = m_offsets[1][(int)loffset + chapter].offset;
	return loffset + verse;
}

const bool RefSys::isBookInRefSys(char book) const {
	return m_offsets[0][book].offset != NOTINREFSYS;
}

SWORD_NAMESPACE_END

