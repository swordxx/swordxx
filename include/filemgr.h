/******************************************************************************
 *  filemgr.h   - definition of class FileMgr used for pooling file handles
 *
 * $Id: filemgr.h,v 1.2 1999/05/04 22:51:22 scribe Exp $
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

#ifndef FILEMGR_H
#define FILEMGR_H

#include <sys/stat.h>
class FileMgr;

class FileDesc {

friend class FileMgr;

	char *path;
	int mode;
	int perms;
	long offset;
	int fd;		// -77 closed;
	FileMgr *parent;
	FileDesc *next;
	
public:
	FileDesc(FileMgr *parent, char *path, int mode, int perms);
	~FileDesc();
	int getFd();
};

	
class FileMgr {

friend class FileDesc;

	FileDesc *files;
	int sysOpen(FileDesc *file);
public:
	
	FileMgr(int maxFiles = 35);
	~FileMgr();
	FileDesc *open(char *path, int mode, int perms = S_IREAD|S_IWRITE);
	void close(FileDesc *);
	int maxFiles;

	static FileMgr systemFileMgr;
};
	
	


#endif
