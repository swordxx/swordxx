/******************************************************************************
 *  filemgr.cpp	- implementation of class FileMgr used for pooling file
 *  					handles
 *
 * $Id: filemgr.cpp,v 1.17 2001/10/30 00:01:50 chrislit Exp $
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

#include <filemgr.h>
#include <utilstr.h>

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

// ---------------- statics -----------------
FileMgr FileMgr::systemFileMgr;

// --------------- end statics --------------


FileDesc::FileDesc(FileMgr *parent, char *path, int mode, int perms) {
	this->parent = parent;
	this->path = 0;
	stdstr(&this->path, path);
	this->mode = mode;
	this->perms = perms;
	offset = 0;
	fd = -77;
}


FileDesc::~FileDesc() {
	if (fd > 0)
		close(fd);
		
	if (path)
		delete [] path;
}


int FileDesc::getFd() {
	if (fd == -77)
		fd = parent->sysOpen(this);
	return fd;
}


FileMgr::FileMgr(int maxFiles) {
	this->maxFiles = maxFiles;		// must be at least 2
	files = 0;
}


FileMgr::~FileMgr() {
	FileDesc *tmp;
	
	while(files) {
		tmp = files->next;
		delete files;
		files = tmp;
	}
}


FileDesc *FileMgr::open(char *path, int mode, int perms) {
	FileDesc **tmp, *tmp2;
	
	for (tmp = &files; *tmp; tmp = &((*tmp)->next)) {
		if ((*tmp)->fd < 0)		// insert as first non-system_open file
			break;
	}

	tmp2 = new FileDesc(this, path, mode, perms);
	tmp2->next = *tmp;
	*tmp = tmp2;
	
	return tmp2;
}


void FileMgr::close(FileDesc *file) {
	FileDesc **loop;
	
	for (loop = &files; *loop; loop = &((*loop)->next)) {
		if (*loop == file) {
			*loop = (*loop)->next;
			delete file;
			break;
		}
	}
}


// to truncate a file at its current position
// leaving byte at current possition intact
// deleting everything afterward.
signed char FileMgr::trunc(FileDesc *file) {

	static const char *writeTest = "x";
	long size = lseek(file->getFd(), 1, SEEK_CUR);
	char nibble [ 32767 ];
	bool writable = write(file->getFd(), writeTest, 1);
	int bytes = 0;

	if (writable) {
		// get tmpfilename
		char *buf = new char [ strlen(file->path) + 10 ];
		int i;
		for (i = 0; i < 9999; i++) {
			sprintf(buf, "%stmp%.4d", file->path, i);
			if (!existsFile(buf))
				break;
		}
		if (i == 9999)
			return -2;

		int fd = ::open(buf, O_CREAT|O_RDWR, S_IREAD|S_IWRITE);
		if (fd < 0)
			return -3;
	
		lseek(file->getFd(), 0, SEEK_SET);
		while (size > 0) {
			bytes = read(file->getFd(), nibble, 32767);
			write(fd, nibble, (bytes < size)?bytes:size);
			size -= bytes;
		}
		// zero out the file
		::close(file->fd);
		file->fd = ::open(file->path, O_TRUNC, S_IREAD|S_IWRITE);
		::close(file->fd);
		file->fd = -77;	// force file open by filemgr
		// copy tmp file back (dumb, but must preserve file permissions)
		lseek(fd, 0, SEEK_SET);
		do {
			bytes = read(fd, nibble, 32767);
			write(file->getFd(), nibble, bytes);
		} while (bytes == 32767);
		
		::close(fd);
		::close(file->fd);
		unlink(buf);		// remove our tmp file
		file->fd = -77;	// causes file to be swapped out forcing open on next call to getFd()
	}
	else { // put offset back and return failure
		lseek(file->getFd(), -1, SEEK_CUR);
		return -1;
	}
	return 0;
}


int FileMgr::sysOpen(FileDesc *file) {
	FileDesc **loop;
	int openCount = 1;		// because we are presently opening 1 file, and we need to be sure to close files to accomodate, if necessary
	
	for (loop = &files; *loop; loop = &((*loop)->next)) {

		if ((*loop)->fd > 0) {
			if (++openCount > maxFiles) {
				(*loop)->offset = lseek((*loop)->fd, 0, SEEK_CUR);
				::close((*loop)->fd);
				(*loop)->fd = -77;
			}
		}

		if (*loop == file) {
			if (*loop != files) {
				*loop = (*loop)->next;
				file->next = files;
				files = file;
			}
               if ((!access(file->path, 04)) || (file->mode & O_CREAT == O_CREAT)) {	// check for at least file exists / read access before we try to open
				file->fd = ::open(file->path, file->mode, file->perms);
			        if (file->fd >= 0)
					lseek(file->fd, file->offset, SEEK_SET);
               }
               else file->fd = -1;
               if (!*loop)
                  break;
		}
	}
	return file->fd;
}


signed char FileMgr::existsFile(const char *ipath, const char *ifileName)
{
	int len = strlen(ipath) + ((ifileName)?strlen(ifileName):0) + 1;
	char *ch;

	if (ifileName)
		len +=  strlen(ifileName);
	char *path = new char [ len ];
	strcpy(path, ipath);
	
	if ((path[strlen(path)-1] == '\\') || (path[strlen(path)-1] == '/'))
		path[strlen(path)-1] = 0;
	
	if (ifileName) {
		ch = path + strlen(path);
		sprintf(ch, "/%s", ifileName);
	}
	signed char retVal = !access(path, 04);
    delete [] path;
    return retVal;
}


signed char FileMgr::existsDir(const char *ipath, const char *idirName)
{
	char *ch;
	int len = strlen(ipath) + ((idirName)?strlen(idirName):0) + 1;
	if (idirName)
		len +=  strlen(idirName);
	char *path = new char [ len ];
	strcpy(path, ipath);
	
	if ((path[strlen(path)-1] == '\\') || (path[strlen(path)-1] == '/'))
		path[strlen(path)-1] = 0;
	
	if (idirName) {
		ch = path + strlen(path);
		sprintf(ch, "/%s", idirName);
	}
	signed char retVal = !access(path, 04);
     delete [] path;
     return retVal;
}
