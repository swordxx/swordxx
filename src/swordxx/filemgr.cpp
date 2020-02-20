/******************************************************************************
 *
 *  filemgr.cpp -    implementation of class FileMgr used for pooling file
 *            handles
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

struct FileMgrInner {

/* Methods: */

    constexpr FileMgrInner(std::size_t maxFiles) noexcept
        : m_maxFiles(maxFiles)
    {}

    void sysOpen(FileDesc & file) noexcept;
    void sysClose(FileDesc & file) noexcept;

/* Fields: */

    FileDesc * m_files = nullptr;
    std::size_t const m_maxFiles;

};

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


FileDesc::FileDesc(std::shared_ptr<FileMgrInner> fileMgrInner,
                   std::string path,
                   int mode,
                   int perms,
                   bool tryDowngrade)
    : m_fileMgrInner(std::move(fileMgrInner))
    , m_path(std::move(path))
    , m_mode(mode)
    , m_perms(perms)
    , m_tryDowngrade(tryDowngrade)
{}


FileDesc::~FileDesc() {
    m_fileMgrInner->sysClose(*this);
    if (m_fd)
        ::close(m_fd.value());
}


int FileDesc::getFd() noexcept {
    if (!m_fd)
        m_fileMgrInner->sysOpen(*this);
    return m_fd.value();
}

FileMgr::FileMgr(std::size_t maxFiles)
    : m_inner(std::make_shared<FileMgrInner>(maxFiles))
{}


FileMgr::~FileMgr() noexcept = default;


std::shared_ptr<FileDesc> FileMgr::open(const char *path, int mode, bool tryDowngrade) {
    return open(path, mode, S_IREAD|S_IWRITE|S_IRGRP|S_IROTH, tryDowngrade);
}


std::shared_ptr<FileDesc> FileMgr::open(const char *path, int mode, int perms, bool tryDowngrade) {
    FileDesc **tmp, *tmp2;

    for (tmp = &m_inner->m_files; *tmp; tmp = &((*tmp)->m_next)) {
        if (!(*tmp)->m_fd)        // insert as first non-system_open file
            break;
    }

    auto r(std::make_shared<FileDesc>(m_inner, path, mode, perms, tryDowngrade));
    tmp2 = r.get();
    tmp2->m_next = *tmp;
    *tmp = tmp2;

    return r;
}


bool FileMgr::touch(char const * path, int perms)
{ return open(path, CREAT | WRONLY, perms)->getFd() >= 0; }


void FileMgrInner::sysClose(FileDesc & file) noexcept {
    FileDesc **loop;

    for (loop = &m_files; *loop; loop = &((*loop)->m_next)) {
        if (*loop == &file) {
            *loop = (*loop)->m_next;
            return;
        }
    }
}


void FileMgrInner::sysOpen(FileDesc & file) noexcept {
    FileDesc **loop;
    std::size_t openCount = 1u;        // because we are presently opening 1 file, and we need to be sure to close files to accomodate, if necessary

    for (loop = &m_files; *loop; loop = &((*loop)->m_next)) {

        if ((*loop)->m_fd) {
            if (++openCount > m_maxFiles) {
                (*loop)->m_offset = ::lseek((*loop)->m_fd.value(), 0, SEEK_CUR);
                ::close((*loop)->m_fd.value());
                (*loop)->m_fd.reset();
            }
        }

        if (*loop == &file) {
            if (*loop != m_files) {
                *loop = (*loop)->m_next;
                file.m_next = m_files;
                m_files = &file;
            }

            file.m_fd = ::open(file.m_path.c_str(), file.m_mode|O_BINARY, file.m_perms);
            if (!file.m_fd) {
                if (file.m_tryDowngrade
                    && ((file.m_mode & O_RDWR) == O_RDWR))
                {
                    file.m_mode = (file.m_mode & ~O_RDWR) | O_RDONLY;
                    file.m_fd = ::open(file.m_path.c_str(), file.m_mode|O_BINARY, file.m_perms);
                    if (file.m_fd) {
                        ::lseek(file.m_fd.value(), file.m_offset, SEEK_SET);
                        file.m_mode = file.m_mode & ~(O_CREAT | O_TRUNC);
                    }
                }
            } else {
                ::lseek(file.m_fd.value(), file.m_offset, SEEK_SET);
                file.m_mode = file.m_mode & ~(O_CREAT | O_TRUNC);
            }
            if (!*loop)
                break;
        }
    }
}


int FileDesc::truncate() noexcept { return ::ftruncate(getFd(), m_offset); }


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
std::string FileDesc::getLine() {
    int len;
    bool more = true;
    char chunk[255];

    std::string line;

    // assert we have a valid file handle
    if (getFd() < 1)
        return line;

    while (more) {
        more = false;
        long index = seek(0, SEEK_CUR);
        len = read(chunk, 254);

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
        seek(index, SEEK_SET);

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
    if (auto dir = DirectoryEnumerator(srcDir)) {
        auto const sDir(std::string(srcDir) + '/');
        auto const dDir(std::string(destDir) + '/');
        while (auto const ent = dir.readEntry()) {
            auto const srcPath(sDir + ent);
            auto const destPath(dDir + ent);
            if (auto retVal = isDirectory(srcPath)
                              ? copyDir(srcPath.c_str(), destPath.c_str())
                              : copyFile(srcPath.c_str(), destPath.c_str()))
                return retVal;
        }
    }
    return 0;
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

} /* namespace swordxx */
