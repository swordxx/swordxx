/******************************************************************************
 *	refsys.h - code for class 'refsys'- Biblical reference system
 *
 * $Id: refsys.h,v 1.1 2004/05/07 17:02:32 dglassey Exp $
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
 
#ifndef REFSYS_H
#define REFSYS_H

#include <defs.h>

SWORD_NAMESPACE_START


#define OTBOOKS 39
#define NTBOOKS 27
#define MAXOSISBOOKS 93
#define TESTAMENT_HEADING 255
#define NOTINREFSYS -1


 
struct bkref
{
	long offset;
	unsigned char maxnext;
};


class SWDLLEXPORT RefSys
{
	const bkref *m_offsets[2];
	int m_offsize[2];
	char *m_name;

	//static bkref kjvbks[];
	//static bkref kjvcps[];
	/** Binary search to find the index closest, but less
	* than the given value.
	*
	* @param array long * to array to search
	* @param size number of elements in the array
	* @param value value to find
	* @return the index into the array that is less than but closest to value
	*/
	int findindex(const bkref *array, int size, long value) const;


public:
	RefSys(const char *name,
		const bkref *offsets[2] = 0,
		const int offsize[2] = 0
	);
	~RefSys();

	virtual const int getMaxChaptersInBook(char book) const;
	virtual const int getMaxVerseInChapter(char book, int chapter) const;
	void getBCV(long iindex, char *book, int *chapter, int *verse) const;
	const long getIndex(char book, int chapter, int verse) const;
	const bool isBookInRefSys(char book) const;
	const char *getName() const {return m_name;}

};

SWORD_NAMESPACE_END
#endif
