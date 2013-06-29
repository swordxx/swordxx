/******************************************************************************
 *
 *  ciphertest.cpp -	
 *
 * $Id$
 *
 * Copyright 2005-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <cipherfil.h>
#include <filemgr.h>
#include <swbuf.h>
#include <iostream>

using namespace sword;

int main(int argc, char **argv) {
	
	if (argc != 3) {
		std::cerr << "usage: " << *argv << " <key> <0-encipher|1-decipher>\n";
		return -1;
	}

	
	long encipher = atoi(argv[2]);

	SWFilter *filter = new CipherFilter(argv[1]);

	SWBuf text;
	char buf[4096];
	std::cin >> buf;
	text = buf;

	filter->processText(text, (SWKey *)encipher);

	std::cout << text;
	

	
	return 0;
}
