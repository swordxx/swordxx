/*
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
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

#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <filemgr.h>
#include <swcipher.h>
#include <versekey.h>
#include <rawverse.h>
#include <swbuf.h>

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv) {
	SWCipher *zobj;
	VerseKey key;
	RawVerse *rawdrv;
	int ofd[2], oxfd[2];
	long tmpoff = 0, offset, loffset = 0, lzoffset = 0;
	unsigned short size, lsize = 0, lzsize;
	char *tmpbuf;
	
	if (argc != 3) {
		fprintf(stderr, "usage: %s <datapath> \"<key>\"\n", argv[0]);
		exit(1);
	}

	rawdrv = new RawVerse(argv[1]);
	zobj = new SWCipher((unsigned char *)argv[2]);

	tmpbuf = new char [ strlen(argv[1]) + 11 ];
	sprintf(tmpbuf, "%sot.zzz", argv[1]);
	ofd[0] = FileMgr::createPathAndFile(tmpbuf);
	sprintf(tmpbuf, "%sot.zzz.vss", argv[1]);
	oxfd[0] = FileMgr::createPathAndFile(tmpbuf);
	sprintf(tmpbuf, "%snt.zzz", argv[1]);
	ofd[1] = FileMgr::createPathAndFile(tmpbuf);
	sprintf(tmpbuf, "%snt.zzz.vss", argv[1]);
	oxfd[1] = FileMgr::createPathAndFile(tmpbuf);

	delete [] tmpbuf;

	printf("\n");
	write(oxfd[0], &lzoffset, 4);
	write(oxfd[0], &lzsize, 2);
	write(oxfd[1], &lzoffset, 4);
	write(oxfd[1], &lzsize, 2);

	key.AutoNormalize(0);
	key.Headings(1);
	for (key.Index(0); (!key.Error()); key++) {
		rawdrv->findOffset(key.Testament(), key.Index(), &offset, &size);
		printf("%s: OLD offset: %ld; size: %d\n", (const char *)key, offset, size);

		if ((offset == loffset) && (size == lsize)) {
			printf("using previous offset,size %d\n", size);
			offset = lseek(oxfd[key.Testament() - 1], 0, SEEK_CUR);
			printf("%ld %ld %d \n", offset, lzoffset, lzsize);
			write(oxfd[key.Testament() - 1], &lzoffset, 4);
			write(oxfd[key.Testament() - 1], &lzsize, 2);
		}
		else {
			lsize   = size;
			loffset = offset;

			if (size) {
				SWBuf tmpbuf;
				rawdrv->readText(key.Testament(), offset, size, tmpbuf);
				zobj->Buf(tmpbuf.c_str(), size);
				unsigned long ulSize = size;
				zobj->cipherBuf(&ulSize);
				size = (unsigned int)ulSize;
			}
			offset = lseek(ofd[key.Testament() - 1], 0, SEEK_CUR);
			tmpoff = lseek(oxfd[key.Testament() - 1], 0, SEEK_CUR);
			printf("%s: (%ld) NEW offset: %ld; size: %d\n", (const char *)key, tmpoff, offset, size);
			write(oxfd[key.Testament() - 1], &offset, 4);
			unsigned long ulSize = size;
			if (size) 
				write(ofd[key.Testament() - 1], zobj->cipherBuf(&ulSize), size);
			size = (unsigned int)ulSize;
			lzoffset = offset;
			write(oxfd[key.Testament() - 1], &size, 2);
			lzsize = size;
		}
	}
	delete zobj;
	close(ofd[0]);
	close(oxfd[0]);
	close(ofd[1]);
	close(oxfd[1]);
	return 0;
}
