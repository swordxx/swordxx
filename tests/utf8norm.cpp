/******************************************************************************
 *
 *  utf8norm.cpp -	
 *
 * $Id$
 *
 * Copyright 2009-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <iostream>
#include <utilstr.h>
#include <swbuf.h>

using namespace sword;
using namespace std;

int main(int argc, char **argv) {
	const char *buf = (argc > 1) ? argv[1] : "Description=German Unrevidierte Luther Übersetzung von 1545";

	SWBuf fixed = assureValidUTF8(buf);

	cout << "input / processed:\n" << buf << "\n" << fixed << endl;

	return 0;
}
