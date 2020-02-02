/******************************************************************************
 *
 *  step2vpl.cpp -    Utility to export a STEP module as VPL
 *
 * Copyright 2000-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#ifndef __GNUC__
#include <io.h>
#endif
#include <iostream>
#include <string>
#include <swordxx/filemgr.h>
#include <swordxx/modules/common/lzsscomprs.h>
#include <sys/stat.h>
#ifdef __GNUC__
#include <unistd.h>
#endif


using namespace std;
using namespace swordxx;

long SECTIONSLEVELSTART = 38;
long SECTIONSLEVELSIZE = 29;

long VIEWABLEBLOCKSTART = 0;
long VIEWABLEBLOCKSIZE = 0;

typedef struct {
    short versionRecordSize;
    short publisherID;
    short bookID;
    short setID;
    char conversionProgramVerMajor;
    char conversionProgramVerMinor;
    char leastCompatSTEPVerMajor;
    char leastCompatSTEPVerMinor;
    char encryptionType;
    char editionID;
    short modifiedBy;
} Version;

typedef struct {
    short sectionsHeaderRecordSize;
    long levelEntriesCount; // this is listed as nonGlossBlocksCount in spec!
    long glossEntriesCount;
    short levelEntriesSize;
    long reserved;
} SectionsHeader;

typedef struct {
    short viewableHeaderRecordSize;
    long viewableBlocksCount; // this is listed as nonGlossBlocksCount in spec!
    long glossBlocksCount;
    char compressionType; // 0 - none; 1 - LZSS
    char reserved1;
    short blockEntriesSize;
    short reserved2;
} ViewableHeader;

typedef struct {
    short vSyncHeaderRecordSize;
    short startBookNumber;
    short endBookNumber;
    short bookPointerEntriesSize;
    short syncPointEntriesSize;
    long reserved1_1;
    short reserved1_2;
} VSyncHeader;

typedef struct {
    long offset;
    long uncompressedSize;
    long size;
} ViewableBlock;

typedef struct {
    long offset;    // offset into VSYNC.IDX to first VSyncPoint
    short count;    // number of VSyncPoints for this book
} VSyncBooksInfo;

typedef struct {
    short chapter;
    short verse;
    long offset;    // offset into SECTIONS.IDX
} VSyncPoint;

typedef struct {
    long parentOffset; // many of these are 0 if glossary
    long previousOffset;
    long nextOffset;
    long viewableOffset;
    short startLevel;
    char level;
    long nameOffset;
    long outSync_1;
    short outSync_2;
} SectionLevelInfo;

void readVersion(int fd, Version *versionRecord);
void readHeaderControlWordAreaText(int fd);
void readViewableHeader(int fd, ViewableHeader *viewableHeaderRecord);
void readVSyncHeader(int fd, VSyncHeader *vSyncHeaderRecord);
void readVSyncBooksInfo(int fd, VSyncHeader *, VSyncBooksInfo **vSyncBooksInfo);
void readViewableBlock(int fd, ViewableBlock *vb);
std::string readViewableBlockText(int fd, ViewableBlock *vb);
void readSectionsHeader(int fd, SectionsHeader *sectionsHeaderRecord);
void readSectionLevelInfo(int fd, SectionLevelInfo *sli);
std::string readSectionName(int fd, SectionLevelInfo *sli);
void displayBook(int fdbook, int fdviewable, int fdvsync, int fdsections, VSyncBooksInfo *vSyncBooksInfo);
std::string extractVerseText(int fdviewable, int fdbook, SectionLevelInfo *sectionLevelInfo);
void cleanBuf(char *buf);

SWCompress * compress = nullptr;

int main(int argc, char **argv) {

    compress = new LZSSCompress();
    Version versionRecord;
    VSyncHeader vSyncHeaderRecord;
    VSyncBooksInfo *vSyncBooksInfo;
    SectionsHeader sectionsHeaderRecord;
    ViewableHeader viewableHeaderRecord;


    if (argc < 2) {
        cerr << "usage: "<< *argv << " <database to step module>\n";
        std::exit(-1);
    }

    string bookpath = argv[1];
    string fileName;

    if ((argv[1][std::strlen(argv[1])-1] != '/') &&
        (argv[1][std::strlen(argv[1])-1] != '\\'))
        bookpath += "/";

    fileName = bookpath + "Book.dat";
    int fdbook = FileMgr::openFileReadOnly(fileName.c_str());

    if (fdbook < 1) {
        cerr << "error, couldn't open file: " << fileName << "\n";
        std::exit(-2);
    }

    try {
        readVersion(fdbook, &versionRecord);
        readHeaderControlWordAreaText(fdbook);
    }
    catch (std::runtime_error &e) {
        std::cout << e.what () << '\n';
        std::exit(-3);
    }

    fileName = bookpath + "Viewable.idx";
    int fdviewable = FileMgr::openFileReadOnly(fileName.c_str());

    if (fdviewable < 1) {
        cerr << "error, couldn't open file: " << fileName << "\n";
        std::exit(-4);
    }

    try {
        readVersion(fdviewable, &versionRecord);
        readViewableHeader(fdviewable, &viewableHeaderRecord);
    }
    catch (std::runtime_error &e) {
        std::cout << e.what () << '\n';
        std::exit(-5);
    }

    VIEWABLEBLOCKSTART = lseek(fdviewable, 0, SEEK_CUR);
    VIEWABLEBLOCKSIZE = viewableHeaderRecord.blockEntriesSize;


    fileName = bookpath + "Vsync.idx";
    int fdvsync = FileMgr::openFileReadOnly(fileName.c_str());

    if (fdvsync < 1) {
        cerr << "error, couldn't open file: " << fileName << "\n";
        std::exit(-6);
    }

    fileName = bookpath + "Sections.idx";
    int fdsections = FileMgr::openFileReadOnly(fileName.c_str());

    if (fdsections < 1) {
        cerr << "error, couldn't open file: " << fileName << "\n";
        std::exit(-7);
    }

    try {
        readVersion(fdsections, &versionRecord);
        readSectionsHeader(fdsections, &sectionsHeaderRecord);
        SECTIONSLEVELSTART = lseek(fdsections, 0, SEEK_CUR);
        SECTIONSLEVELSIZE = sectionsHeaderRecord.levelEntriesSize;

        readVersion(fdvsync, &versionRecord);
        readVSyncHeader(fdvsync, &vSyncHeaderRecord);
        readVSyncBooksInfo(fdvsync, &vSyncHeaderRecord, &vSyncBooksInfo);

        int bookCount = vSyncHeaderRecord.endBookNumber - vSyncHeaderRecord.startBookNumber;
        for (int i = 0; i <= bookCount; i++) {
            displayBook(fdbook, fdviewable, fdvsync, fdsections, &vSyncBooksInfo[i]);
        }
    }
    catch (std::runtime_error &e) {
        std::cout << e.what () << '\n';
        std::exit(-8);
    }

    close(fdviewable);
    close(fdvsync);
    close(fdsections);
    close(fdbook);

}



void readVersion(int fd, Version *versionRecord) {
    string readError = "Error reading Version Record.";
    string seekError = "Error seeking Version Record.";
    int rtn = read(fd, &(versionRecord->versionRecordSize), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(versionRecord->publisherID), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(versionRecord->bookID), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(versionRecord->setID), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(versionRecord->conversionProgramVerMajor), 1);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(versionRecord->conversionProgramVerMinor), 1);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(versionRecord->leastCompatSTEPVerMajor), 1);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(versionRecord->leastCompatSTEPVerMinor), 1);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(versionRecord->encryptionType), 1);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(versionRecord->editionID), 1);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(versionRecord->modifiedBy), 2);
    if (rtn <= 0) throw runtime_error(readError);

    int skip = versionRecord->versionRecordSize - 16/*sizeof(struct Version*/;

    if (skip && (lseek(fd, skip, SEEK_CUR) < 0))
        throw runtime_error(seekError);
}


void readSectionsHeader(int fd, SectionsHeader *sectionsHeaderRecord) {

    string readError = "Error reading Section Header.";
    string seekError = "Error seeking Section Header.";
    int rtn = read(fd, &(sectionsHeaderRecord->sectionsHeaderRecordSize), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(sectionsHeaderRecord->levelEntriesCount), 4);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(sectionsHeaderRecord->glossEntriesCount), 4);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(sectionsHeaderRecord->levelEntriesSize), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(sectionsHeaderRecord->reserved), 4);
    if (rtn <= 0) throw runtime_error(readError);

    int skip = sectionsHeaderRecord->sectionsHeaderRecordSize - 16/*sizeof(struct ViewableHeader)*/;

    if (skip && (lseek(fd, skip, SEEK_CUR) < 0))
        throw runtime_error(seekError);
}


void readViewableHeader(int fd, ViewableHeader *viewableHeaderRecord) {

    string readError = "Error reading Viewable Header.";
    string seekError = "Error seeking Viewable Header.";
    int rtn = read(fd, &(viewableHeaderRecord->viewableHeaderRecordSize), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(viewableHeaderRecord->viewableBlocksCount), 4);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(viewableHeaderRecord->glossBlocksCount), 4);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(viewableHeaderRecord->compressionType), 1);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(viewableHeaderRecord->reserved1), 1);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(viewableHeaderRecord->blockEntriesSize), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(viewableHeaderRecord->reserved2), 2);
    if (rtn <= 0) throw runtime_error(readError);

    int skip = viewableHeaderRecord->viewableHeaderRecordSize - 16/*sizeof(struct ViewableHeader)*/;

    if (skip && (lseek(fd, skip, SEEK_CUR) < 0))
        throw runtime_error(seekError);
}


void readVSyncHeader(int fd, VSyncHeader *vSyncHeaderRecord) {

    string readError = "Error reading VSync Header.";
    string seekError = "Error seeking VSync Header.";
    int rtn = read(fd, &(vSyncHeaderRecord->vSyncHeaderRecordSize), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(vSyncHeaderRecord->startBookNumber), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(vSyncHeaderRecord->endBookNumber), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(vSyncHeaderRecord->bookPointerEntriesSize), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(vSyncHeaderRecord->syncPointEntriesSize), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(vSyncHeaderRecord->reserved1_1), 4);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(vSyncHeaderRecord->reserved1_2), 2);
    if (rtn <= 0) throw runtime_error(readError);

    int skip = vSyncHeaderRecord->vSyncHeaderRecordSize - 16/*sizeof(VSyncHeader)*/;

    if (skip && (lseek(fd, skip, SEEK_CUR) < 0))
        throw runtime_error(seekError);
}


std::string readViewableBlockText(int fd, ViewableBlock *vb) {
    string readError = "Error reading Viewable Block Text.";
    unsigned long size = vb->size;

    auto buf(std::make_unique<char[]>(((vb->size > vb->uncompressedSize)
                                       ? vb->size
                                       : vb->uncompressedSize) + 1u));
    lseek(fd, vb->offset, SEEK_SET);
    int rtn = read(fd, buf.get(), vb->size);
    if (rtn <= 0) throw runtime_error(readError);

    compress->zBuf(&size, buf.get());
    return compress->Buf();
}


void readViewableBlock(int fd, ViewableBlock *vb) {

    string readError = "Error reading Viewable Block.";
    int rtn = read(fd, &(vb->offset), 4);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(vb->uncompressedSize), 4);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(vb->size), 4);
    if (rtn <= 0) throw runtime_error(readError);
}


void readHeaderControlWordAreaText(int fd) {
    string readError = "Error reading Control Word Area Text.";
    long headerControlWordAreaSize;
    int rtn = read(fd, &headerControlWordAreaSize, 4);
    if (rtn <= 0) throw runtime_error(readError);

    auto const buf(std::make_unique<char[]>(headerControlWordAreaSize + 1));

    rtn = read(fd, buf.get(), headerControlWordAreaSize);
    if (rtn <= 0) throw runtime_error(readError);
}

void readVSyncBooksInfo(int fd, VSyncHeader *vSyncHeaderRecord, VSyncBooksInfo **vSyncBooksInfo) {

    string readError = "Error reading VSync Books Info.";
    int bookCount = vSyncHeaderRecord->endBookNumber - vSyncHeaderRecord->startBookNumber;
    *vSyncBooksInfo = new VSyncBooksInfo[bookCount];
    for (int i = 0; i <= bookCount; i++) {
        int rtn = read(fd, &(*vSyncBooksInfo)[i].offset, 4);
        if (rtn <= 0) throw runtime_error(readError);
        rtn = read(fd, &(*vSyncBooksInfo)[i].count, 2);
        if (rtn <= 0) throw runtime_error(readError);
    }
}

void displayBook(int fdbook, int fdviewable, int fdvsync, int fdsections, VSyncBooksInfo *vSyncBooksInfo) {
    string readError = "Error reading Display Book.";
    VSyncPoint vSyncPoint;

    lseek(fdvsync, vSyncBooksInfo->offset, SEEK_SET);

    for (int i = 0; i < vSyncBooksInfo->count; i++) {

        SectionLevelInfo sectionLevelInfo;

        int rtn = read(fdvsync, &(vSyncPoint.chapter), 2);
        if (rtn <= 0) throw runtime_error(readError);
        rtn = read(fdvsync, &(vSyncPoint.verse), 2);
        if (rtn <= 0) throw runtime_error(readError);
        rtn = read(fdvsync, &(vSyncPoint.offset), 4);
        if (rtn <= 0) throw runtime_error(readError);
        vSyncPoint.offset = SECTIONSLEVELSTART + (vSyncPoint.offset * SECTIONSLEVELSIZE);
        lseek(fdsections, vSyncPoint.offset, SEEK_SET);
        readSectionLevelInfo(fdsections, &sectionLevelInfo);
        cout << readSectionName(fdsections, &sectionLevelInfo) << " ";
        std::string verseText(extractVerseText(fdviewable, fdbook, &sectionLevelInfo));
        cleanBuf(std::addressof(verseText[0u]));
        cout << verseText << "\n";
    }
}


std::string extractVerseText(int fdviewable, int fdbook, SectionLevelInfo *sectionLevelInfo) {
    char numberBuf[16];
    string startToken;
    ViewableBlock vb;
    int len = 0;
    static long lastEntryOffset = -1;
    std::string cachedEntryText;

    if (sectionLevelInfo->viewableOffset != lastEntryOffset) {
        lseek(fdviewable, sectionLevelInfo->viewableOffset, SEEK_SET);
        readViewableBlock(fdviewable, &vb);
        cachedEntryText = readViewableBlockText(fdbook, &vb);
        lastEntryOffset = sectionLevelInfo->viewableOffset;
    }
    sprintf(numberBuf, "%d", sectionLevelInfo->startLevel);
    startToken = "\\stepstartlevel";
    startToken += numberBuf;
    char const * start = std::strstr(cachedEntryText.c_str(), startToken.c_str());
    if (start) {
        start += std::strlen(startToken.c_str());
        char const * end = std::strstr(start, "\\stepstartlevel");
        if (end)
            len = end - start;
        else len = std::strlen(start);
    }
    return std::string(start, len);
}


std::string readSectionName(int fd, SectionLevelInfo *sli) {
    string readError = "Error reading Section Name.";
    short size;
    lseek(fd, sli->nameOffset, SEEK_SET);
    int rtn = read(fd, &size, 2);
    if (rtn <= 0) throw runtime_error(readError);
    std::string name;
    name.resize(size);
    rtn = read(fd, std::addressof(name[0u]), size);
    if (rtn <= 0) throw runtime_error(readError);
    return name;
}

void readSectionLevelInfo(int fd, SectionLevelInfo *sli) {

    string readError = "Error reading Section Level Info.";
    int rtn = read(fd, &(sli->parentOffset), 4);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(sli->previousOffset), 4);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(sli->nextOffset), 4);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(sli->viewableOffset), 4);
    if (rtn <= 0) throw runtime_error(readError);
    sli->viewableOffset = VIEWABLEBLOCKSTART + (VIEWABLEBLOCKSIZE * sli->viewableOffset);
    rtn = read(fd, &(sli->startLevel), 2);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(sli->level), 1);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(sli->nameOffset), 4);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(sli->outSync_1), 4);
    if (rtn <= 0) throw runtime_error(readError);
    rtn = read(fd, &(sli->outSync_2), 2);
    if (rtn <= 0) throw runtime_error(readError);
}

void cleanBuf(char *buf) {
    char *from = buf;
    char *to = buf;

    while (*from) {
        if ((*from != 10) && (*from != 13)) {
            *to++ = *from++;
        }
        else {
            from++;
        }
    }
    *to = 0;
}
