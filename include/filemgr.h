/******************************************************************************
 *  filemgr.h   - definition of class FileMgr used for pooling file handles
 *
 * $Id: filemgr.h,v 1.16 2003/02/25 04:12:47 scribe Exp $
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
#include <fcntl.h>

#include <defs.h>

SWORD_NAMESPACE_START

class SWDLLEXPORT FileMgr;

class SWDLLEXPORT FileDesc
{

  friend class FileMgr;

  long offset;
  int fd;			// -77 closed;
  FileMgr *parent;
  FileDesc *next;

public:
   FileDesc (FileMgr * parent, const char *path, int mode, int perms, bool tryDowngrade);
   virtual ~FileDesc ();
  int getFd ();
  char *path;
  int mode;
  int perms;
  bool tryDowngrade;
};


class FileMgr
{

  friend class FileDesc;

  FileDesc *files;
  int sysOpen (FileDesc * file);
public:

    FileMgr (int maxFiles = 35);
   ~FileMgr ();
  FileDesc *open (const char *path, int mode, bool tryDowngrade);
  FileDesc *open (const char *path, int mode, int perms = S_IREAD | S_IWRITE, bool tryDowngrade = false);
  void close (FileDesc *);

  static signed char existsFile (const char *ipath, const char *ifileName = 0);
  static signed char existsDir (const char *ipath, const char *idirName = 0);
  // to truncate a file at its current position
  // leaving byte at current possition intact
  // deleting everything afterward.
  signed char trunc (FileDesc *);

  int maxFiles;
  static FileMgr systemFileMgr;
};


SWORD_NAMESPACE_END
#endif
