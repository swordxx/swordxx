/******************************************************************************
 *
 *  filemgr.cpp -    implementation of class FileMgr used for pooling file
 *            handles
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

#include "filemgr.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#if !defined(__GNUC__) && !defined(_WIN32_WCE)
#include <direct.h>
#include <io.h>
#endif
#include <fcntl.h>
#include <mutex>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include "DirectoryEnumerator.h"
#include "utilstr.h"


#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef S_IRGRP
#define S_IRGRP 0
#endif

#ifndef S_IROTH
#define S_IROTH 0
#endif

// Fix for VC6
#ifndef S_IREAD
#ifdef _S_IREAD
#define S_IREAD _S_IREAD
#define S_IWRITE _S_IWRITE
#endif
#endif
// -----------

// ------- if we still don't have something
#ifndef S_IREAD
#define S_IREAD 0400
#endif
#ifndef S_IWRITE
#define S_IWRITE 0200
#endif
// -------


namespace swordxx {


int const FileMgr::CREAT = O_CREAT;
int const FileMgr::APPEND = O_APPEND;
int const FileMgr::TRUNC = O_TRUNC;
int const FileMgr::RDONLY = O_RDONLY;
int const FileMgr::RDWR = O_RDWR;
int const FileMgr::WRONLY = O_WRONLY;
int const FileMgr::IREAD = S_IREAD;
int const FileMgr::IWRITE = S_IWRITE;


// ---------------- statics -----------------
namespace {

#if __cplusplus <= 201703L
std::mutex g_systemFileMgrMutex;
std::shared_ptr<FileMgr> g_systemFileMgr;
#else
std::atomic<std::shared_ptr<FileMgr>> g_systemFileMgr;
#endif

} // anonymous namespace

std::shared_ptr<FileMgr> FileMgr::getSystemFileMgr() {
    std::lock_guard<std::mutex> const guard(g_systemFileMgrMutex);
    if (g_systemFileMgr)
        return g_systemFileMgr;
    auto r(std::make_shared<FileMgr>());
    g_systemFileMgr = r;
    return r;
}


void FileMgr::setSystemFileMgr(std::shared_ptr<FileMgr> newFileMgr) {
    std::lock_guard<std::mutex> const guard(g_systemFileMgrMutex);
    g_systemFileMgr = std::move(newFileMgr);
}

// --------------- end statics --------------


FileDesc::FileDesc(FileMgr * parent,
                   const char * path_,
                   int mode_,
                   int perms_,
                   bool tryDowngrade_)
    : m_parent(parent)
    , path(path_)
    , mode(mode_)
    , perms(perms_)
    , tryDowngrade(tryDowngrade_)
{}


FileDesc::~FileDesc() {
    if (m_fd > 0)
        close(m_fd);
}


int FileDesc::getFd() {
    if (m_fd == -77)
        m_fd = m_parent->sysOpen(this);
//    if ((fd < -1) && (fd != -77))  // kludge to hand ce
//        return 777;
    return m_fd;
}

FileMgr::FileMgr(std::size_t maxFiles)
    : m_maxFiles(maxFiles)
{}


FileMgr::~FileMgr() noexcept {
    FileDesc *tmp;

    while (m_files) {
        tmp = m_files->m_next;
        delete m_files;
        m_files = tmp;
    }
}


FileDesc *FileMgr::open(const char *path, int mode, bool tryDowngrade) {
    return open(path, mode, S_IREAD|S_IWRITE|S_IRGRP|S_IROTH, tryDowngrade);
}


FileDesc *FileMgr::open(const char *path, int mode, int perms, bool tryDowngrade) {
    FileDesc **tmp, *tmp2;

    for (tmp = &m_files; *tmp; tmp = &((*tmp)->m_next)) {
        if ((*tmp)->m_fd < 0)        // insert as first non-system_open file
            break;
    }

    tmp2 = new FileDesc(this, path, mode, perms, tryDowngrade);
    tmp2->m_next = *tmp;
    *tmp = tmp2;

    return tmp2;
}


void FileMgr::close(FileDesc *file) {
    FileDesc **loop;

    for (loop = &m_files; *loop; loop = &((*loop)->m_next)) {
        if (*loop == file) {
            *loop = (*loop)->m_next;
            delete file;
            break;
        }
    }
}


int FileMgr::sysOpen(FileDesc *file) {
    FileDesc **loop;
    std::size_t openCount = 1u;        // because we are presently opening 1 file, and we need to be sure to close files to accomodate, if necessary

    for (loop = &m_files; *loop; loop = &((*loop)->m_next)) {

        if ((*loop)->m_fd > 0) {
            if (++openCount > m_maxFiles) {
                (*loop)->m_offset = lseek((*loop)->m_fd, 0, SEEK_CUR);
                ::close((*loop)->m_fd);
                (*loop)->m_fd = -77;
            }
        }

        if (*loop == file) {
            if (*loop != m_files) {
                *loop = (*loop)->m_next;
                file->m_next = m_files;
                m_files = file;
            }
            if ((!::access(file->path.c_str(), R_OK)) || ((file->mode & O_CREAT) == O_CREAT)) {    // check for at least file exists / read access before we try to open
                char tries = (((file->mode & O_RDWR) == O_RDWR) && (file->tryDowngrade)) ? 2 : 1;  // try read/write if possible
                for (int i = 0; i < tries; i++) {
                    if (i > 0) {
                        file->mode = (file->mode & ~O_RDWR);    // remove write access
                        file->mode = (file->mode | O_RDONLY);// add read access
                    }
                    file->m_fd = ::open(file->path.c_str(), file->mode|O_BINARY, file->perms);

                    if (file->m_fd >= 0)
                        break;
                }

                if (file->m_fd >= 0)
                    lseek(file->m_fd, file->m_offset, SEEK_SET);
            }
            else file->m_fd = -1;
            if (!*loop)
                break;
        }
    }
    return file->m_fd;
}


// to truncate a file at its current position
// leaving byte at current possition intact
// deleting everything afterward.
signed char FileMgr::trunc(FileDesc *file) {

    static const char *writeTest = "x";
    long size = file->seek(1, SEEK_CUR);
    if (size == 1) // was empty
        size = 0;
    char nibble [ 32767 ];
    bool writable = file->write(writeTest, 1);
    int bytes = 0;

    if (writable) {
        // get tmpfilename
        std::string tmpFileName;
        int i = 0;
        do {
            tmpFileName = formatted("%stmp%.4d", file->path.c_str(), i);
            if (!exists(tmpFileName.c_str()))
                goto tmpFileNameReady;
        } while (++i < 10000);
        return -2;
        tmpFileNameReady:

        int fd = ::open(tmpFileName.c_str(), O_CREAT|O_RDWR, S_IREAD|S_IWRITE|S_IRGRP|S_IROTH);
        if (fd < 0)
            return -3;

        file->seek(0, SEEK_SET);
        while (size > 0) {
            bytes = file->read(nibble, 32767);
            bytes = (bytes < size)?bytes:size;
            if (write(fd, nibble, bytes) != bytes) { break; }
            size -= bytes;
        }
        if (size < 1) {
            // zero out the file
            ::close(file->m_fd);
            file->m_fd = ::open(file->path.c_str(), O_TRUNC, S_IREAD|S_IWRITE|S_IRGRP|S_IROTH);
            ::close(file->m_fd);
            file->m_fd = -77;    // force file open by filemgr
            // copy tmp file back (dumb, but must preserve file permissions)
            lseek(fd, 0, SEEK_SET);
            do {
                bytes = read(fd, nibble, 32767);
                file->write(nibble, bytes);
            } while (bytes == 32767);
        }

        ::close(fd);
        ::close(file->m_fd);
        removeFile(tmpFileName.c_str()); // remove our tmp file
        file->m_fd = -77;    // causes file to be swapped out forcing open on next call to getFd()
    }
    else { // put offset back and return failure
        file->seek(-1, SEEK_CUR);
        return -1;
    }
    return 0;
}


bool FileMgr::existsFile(NormalizedPath const & path, char const * ifileName) {
    std::string buf(std::move(path));
    if (ifileName)
        buf.append("/").append(ifileName);

    return isReadable(buf);
}

bool FileMgr::existsDir(NormalizedPath const & path, const char *idirName)
{ return existsFile(path, idirName); }

bool FileMgr::exists(std::string const & fullPath) noexcept
{ return !::access(fullPath.c_str(), F_OK); }

bool FileMgr::isReadable(std::string const & fullPath) noexcept
{ return !::access(fullPath.c_str(), R_OK); }

int FileMgr::createParent(NormalizedPath const & path) {
    NormalizedPath const buf(path.getParentDirectory());

    int retCode = 0;
    if (!buf.empty()) {
        if (::access(buf.c_str(), W_OK)) {  // not exists with write access?
            if ((retCode = ::mkdir(buf.c_str()
#ifndef WIN32
                    , 0755
#endif
                    ))) {
                createParent(buf.c_str());
                retCode = ::mkdir(buf.c_str()
#ifndef WIN32
                    , 0755
#endif
                    );
            }
        }
    }
    else retCode = -1;
    return retCode;
}

int FileMgr::openFileReadOnly(char const * fName)
{ return ::open(fName, O_RDONLY|O_BINARY, S_IREAD|S_IWRITE|S_IRGRP|S_IROTH); }

int FileMgr::createPathAndFile(const char *fName) {
    int fd;

    fd = ::open(fName, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE|S_IRGRP|S_IROTH);
    if (fd < 1) {
        createParent(fName);
        fd = ::open(fName, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE|S_IRGRP|S_IROTH);
    }
    return fd;
}


int FileMgr::copyFile(const char *sourceFile, const char *targetFile) {
    int sfd, dfd, len;
    char buf[4096];

    if ((sfd = ::open(sourceFile, O_RDONLY|O_BINARY, S_IREAD|S_IWRITE|S_IRGRP|S_IROTH)) < 1)
        return -1;
    if ((dfd = createPathAndFile(targetFile)) < 1)
        return -1;

    do {
        len = read(sfd, buf, 4096);
        if (write(dfd, buf, len) != len) break;
    }
    while(len == 4096);
    ::close(dfd);
    ::close(sfd);

    return 0;
}


int FileMgr::removeFile(const char *fName) {
    return ::remove(fName);
}

/// \bug error is same as empty line
std::string FileMgr::getLine(FileDesc *fDesc) {
    int len;
    bool more = true;
    char chunk[255];

    std::string line;

    // assert we have a valid file handle
    if (fDesc->getFd() < 1)
        return line;

    while (more) {
        more = false;
        long index = fDesc->seek(0, SEEK_CUR);
        len = fDesc->read(chunk, 254);

        // assert we have a readable file (not a directory)
        if (len < 1)
            break;

        int start = 0;
        // clean up any preceding white space if we're at the beginning of line
        if (!line.length()) {
            for (;start < len; start++) {
                if ((chunk[start] != 13) && (chunk[start] != ' ') && (chunk[start] != '\t'))
                    break;
            }
        }

        // find the end
        int end;
        for (end = start; ((end < (len-1)) && (chunk[end] != 10)); end++);

        if ((chunk[end] != 10) && (len == 254)) {
            more = true;
        }
        index += (end + 1);

        // reposition to next valid place to read
        fDesc->seek(index, SEEK_SET);

        // clean up any trailing junk on line if we're at the end
        if (!more) {
            for (; end > start; end--) {
                if ((chunk[end] != 10) && (chunk[end] != 13) && (chunk[end] != ' ') && (chunk[end] != '\t')) {
                    if (chunk[end] == '\\') {
                        more = true;
                        end--;
                    }
                    break;
                }
            }
        }

        int size = (end - start) + 1;

        if (size > 0) {
            // line.appendFormatted("%.*s", size, chunk+start);
            line.append(chunk+start, size);
        }
    }
    return line;
}


bool FileMgr::isDirectory(std::string const & path) {
    struct ::stat stats;
    if (::stat(path.c_str(), &stats))
        return false;
    return (stats.st_mode & S_IFDIR) == S_IFDIR;
}


int FileMgr::copyDir(const char *srcDir, const char *destDir) {
    int retVal = 0;
    if (auto dir = DirectoryEnumerator(srcDir)) {
        std::string sDir(std::string(srcDir) + '/');
        std::string dDir(std::string(destDir) + '/');
        while (auto const ent = dir.readEntry()) {
            if (retVal)
                break;
            std::string const srcPath(sDir + ent);
            std::string const destPath(dDir + ent);
            if (!isDirectory(srcPath)) {
                retVal = copyFile(srcPath.c_str(), destPath.c_str());
            }
            else {
                retVal = copyDir(srcPath.c_str(), destPath.c_str());
            }
        }
    }
    return retVal;
}


int FileMgr::removeDir(const char *targetDir) {
    if (auto dir = DirectoryEnumerator(targetDir)) {
        while (auto const ent = dir.readEntry()) {
            std::string targetPath(std::string(targetDir) + '/' + ent);
            if (!isDirectory(targetPath)) {
                FileMgr::removeFile(targetPath.c_str());
            }
            else {
                FileMgr::removeDir(targetPath.c_str());
            }
        }
        dir.close();
        FileMgr::removeFile(targetDir);
/*
        int status = FileMgr::removeFile(targetDir);
          int stuff = errno;
          char *err = std::strerror(errno);
          int x = stuff;
*/
    }
    return 0;
}


void FileMgr::flush() noexcept {
    FileDesc **loop;

    for (loop = &m_files; *loop; loop = &((*loop)->m_next)) {
        if ((*loop)->m_fd > 0) {
            (*loop)->m_offset = lseek((*loop)->m_fd, 0, SEEK_CUR);
            ::close((*loop)->m_fd);
            (*loop)->m_fd = -77;
        }
    }
}

} /* namespace swordxx */
