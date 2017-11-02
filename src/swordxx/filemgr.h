/******************************************************************************
 *
 *  filemgr.h -    definition of class FileMgr used for pooling file handles
 *
 * $Id$
 *
 * Copyright 1998-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
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

#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <memory>
#if defined(__GNUC__) || defined(_WIN32_WCE)
#include <unistd.h>
#endif
#include <utility>
#include "defs.h"
#include "normalizedpath.h"


namespace swordxx {

class SWDLLEXPORT FileMgr;

struct SWDLLEXPORT DirEntry {
public:
    std::string name;
    unsigned long size;
    bool isDirectory;
};
/**
* This class represents one file. It works with the FileMgr object.
*/
class SWDLLEXPORT FileDesc {

    friend class FileMgr;

    long offset;
    int fd;            // -77 closed;
    FileMgr *parent;
    FileDesc *next;

    FileDesc(FileMgr * parent, const char *path, int mode, int perms, bool tryDowngrade);
    virtual ~FileDesc();

public:
    /** @return File handle.
    */
    int getFd();

    template <typename ... Args>
    auto seek(Args && ... args)
    { return ::lseek(getFd(), std::forward<Args>(args)...); }

    template <typename ... Args>
    auto read(Args && ... args)
    { return ::read(getFd(), std::forward<Args>(args)...); }

    template <typename ... Args>
    auto write(Args && ... args)
    { return ::write(getFd(), std::forward<Args>(args)...); }

    /** Path to file.
    */
    std::string path;
    /** File access mode.
    */
    int mode;
    /** File permissions.
    */
    int perms;
    /**
    */
    bool tryDowngrade;
};

/**
 * This class isolates all file io for SWORD, making OS
 * level quirks easier to fix.  This class is typically
 * copied and replaced if necessary to get SWORD to run on
 * a specific platform (e.g., Windows Mobile), but in
 * the future, statics should be removed to make possible to
 * instead simply subclass and override necessary methods.
 *
 * This class also provides many convenience methods which
 * make working with data storage easier.
 *
 * Conceptually, this factory exposes an interface which
 * allows SWORD to 'open' every file it wants, without
 * worrying about OS limits, and takes care of opening and
 * closing the actual file descriptors when necessary.
 */
class SWDLLEXPORT FileMgr {

    friend class FileDesc;

    FileDesc *files;
    int sysOpen(FileDesc * file);
protected:
    static std::unique_ptr<FileMgr> systemFileMgr;
public:
    static int const CREAT;
    static int const APPEND;
    static int const TRUNC;
    static int const RDONLY;
    static int const RDWR;
    static int const WRONLY;
    static int const IREAD;
    static int const IWRITE;

    /** Maximum number of open files set in the constructor.
    * determines the max number of real system files that
    * filemgr will open.  Adjust for tuning.
    */
    int maxFiles;

    static FileMgr *getSystemFileMgr();
    static void setSystemFileMgr(FileMgr *newFileMgr);

    /** Constructor.
    * @param maxFiles The number of files that this FileMgr may open in parallel, if necessary.
    */
    FileMgr(int maxFiles = 35);

    /**
    * Destructor. Clean things up. Will close all files opened by this FileMgr object.
    */
    virtual ~FileMgr() noexcept;

    /** Open a file and return a FileDesc for it.
    * The file itself will only be opened when FileDesc::getFd() is called.
    * @param path Filename.
    * @param mode File access mode.
    * @param tryDowngrade
    * @return FileDesc object for the requested file.
    */
    FileDesc *open(const char *path, int mode, bool tryDowngrade);

    /** Open a file and return a FileDesc for it.
    * The file itself will only be opened when FileDesc::getFd() is called.
    * @param path Filename.
    * @param mode File access mode.
    * @param perms Permissions.
    * @param tryDowngrade
    * @return FileDesc object for the requested file.
    */
    FileDesc *open(const char *path, int mode, int perms = IREAD | IWRITE, bool tryDowngrade = false);

    /** Close a given file and delete its FileDesc object.
    * Will only close the file if it was created by this FileMgr object.
    * @param file The file to close.
    */
    void close(FileDesc *file);

    void flush() noexcept;

    /** Checks for the existence and readability of a file.
    * @param ipath Path to file.
    * @param ifileName Name of file to check for.
    */
    [[deprecated("Use exists() or isReadable() instead.")]]
    static bool existsFile(NormalizedPath const & path,
                           char const * ifileName = nullptr);

    /** Checks for the existence and readability of a directory.
    * @param ipath Path to directory.
    * @param idirName Name of directory to check for.
    */
    [[deprecated("Use exists() or isReadable() instead.")]]
    static bool existsDir(NormalizedPath const & path,
                          char const * idirName = nullptr);

    /** \returns whether the given path exists and is visible to the process. */
    static bool exists(std::string const & fullPath) noexcept;

    /** \returns whether the given path is readable to the process.*/
    static bool isReadable(std::string const & fullPath) noexcept;

    /** Truncate a file at its current position
    * leaving byte at current possition intact deleting everything afterward.
    * @param file The file to operate on.
    */
    signed char trunc(FileDesc *file);

    static bool isDirectory(std::string const & path);
    static int createParent(NormalizedPath const & path);
    static int createPathAndFile(const char *fName);

    /** attempts to open a file readonly
     * @param fName filename to open
     * @return fd; < 0 = error
     */
    static int openFileReadOnly(const char *fName);
    static int copyFile(const char *srcFile, const char *destFile);
    static int copyDir(const char *srcDir, const char *destDir);
    static int removeDir(const char *targetDir);
    static int removeFile(const char *fName);
    static std::string getLine(FileDesc * fDesc);

};


} /* namespace swordxx */
#endif
