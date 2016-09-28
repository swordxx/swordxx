/******************************************************************************
 *
 *  stepdump.cpp -    Utility to dump a STEP module
 *
 * $Id$
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

#ifdef _MSC_VER
    #pragma warning( disable: 4251 )
    #pragma warning( disable: 4996 )
#endif

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#ifndef __GNUC__
#include <io.h>
#endif
#include <iostream>
#include <string>
#include <swordxx/filemgr.h>
#include <swordxx/swio.h>
#include <swordxx/modules/common/lzsscomprs.h>
#include <sys/stat.h>
#ifdef __GNUC__
#include <unistd.h>
#endif


using namespace std;
using namespace swordxx;

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
    short viewableHeaderRecordSize;
    long viewableBlocksCount; // this is listed as nonGlossBlocksCount in spec!
    long glossBlocksCount;
    char compressionType; // 0 - none; 1 - LZSS
    char reserved1;
    short blockEntriesSize;
    short reserved2;
} ViewableHeader;

typedef struct {
    long offset;
    long uncompressedSize;
    long size;
} ViewableBlock;

void readVersion(int fd, Version *versionRecord);
void readHeaderControlWordAreaText(int fd, char **buf);
void readViewableHeader(int fd, ViewableHeader *viewableHeaderRecord);
void readViewableBlock(int fd, ViewableBlock *vb);
void readViewableBlockText(int fd, ViewableBlock *vb, char **buf);

SWCompress * compress = nullptr;

int main(int argc, char **argv) {

    compress = new LZSSCompress();
    char *buf;
    Version versionRecord;
    ViewableHeader viewableHeaderRecord;

    if (argc < 2) {
        cerr << "usage: "<< *argv << " <database to step module>\n";
        exit (-1);
    }

    string bookpath = argv[1];
    string fileName;

    if ((argv[1][strlen(argv[1])-1] != '/') &&
        (argv[1][strlen(argv[1])-1] != '\\'))
        bookpath += "/";

    fileName = bookpath + "Book.dat";
    int fd = FileMgr::openFileReadOnly(fileName.c_str());

    if (fd < 1) {
        cerr << "error, couldn't open file: " << fileName << "\n";
        exit (-2);
    }

    readVersion(fd, &versionRecord);
    readHeaderControlWordAreaText(fd, &buf);
    delete [] buf;


    fileName = bookpath + "Viewable.idx";
    int fdv = FileMgr::openFileReadOnly(fileName.c_str());

    if (fdv < 1) {
        cerr << "error, couldn't open file: " << fileName << "\n";
        exit (-3);
    }

    readVersion(fdv, &versionRecord);
    readViewableHeader(fdv, &viewableHeaderRecord);

    ViewableBlock vb;

    cout << "\n\nReading special preface viewable BLOCK 0";

    readViewableBlock(fdv, &vb);
    readViewableBlockText(fd, &vb, &buf);
    delete [] buf;

    int nonGlossBlocksCount = viewableHeaderRecord.viewableBlocksCount
                        - viewableHeaderRecord.glossBlocksCount;

    cout << "\n\nReading " << nonGlossBlocksCount << " non-glossary viewable blocks";
    // 1 because we already read the first block above
    for (int i = 1; i < nonGlossBlocksCount; i++) {
        cout << "\nNon-Glossary viewable block: " << i;
        readViewableBlock(fdv, &vb);
        readViewableBlockText(fd, &vb, &buf);
        delete [] buf;
    }

    cout << "\n\nReading " << viewableHeaderRecord.glossBlocksCount << " glossary viewable blocks";
    for (int i = 0; i < viewableHeaderRecord.glossBlocksCount; i++) {
        cout << "\nGlossary viewable block: " << i;
        readViewableBlock(fdv, &vb);
        readViewableBlockText(fd, &vb, &buf);
        delete [] buf;
    }

    close(fdv);
    close(fd);

}



void readVersion(int fd, Version *versionRecord) {

    cout << "\n\nReading Version Record (" << 16/*sizeof(struct Version)*/ << " bytes)\n\n";
// DO NOT USE BECAUSE OF RECORD BYTE ALIGNMENT PROBLEMS
//    read(fd, &versionRecord, sizeof(struct Version));

    cout << "Version Record Information\n";
    swread(fd, &(versionRecord->versionRecordSize), 2);
    cout << "\tversionRecordSize: " << versionRecord->versionRecordSize << "\n";
    swread(fd, &(versionRecord->publisherID), 2);
    cout << "\tpublisherID: " << versionRecord->publisherID << "\n";
    swread(fd, &(versionRecord->bookID), 2);
    cout << "\tbookID: " << versionRecord->bookID << "\n";
    swread(fd, &(versionRecord->setID), 2);
    cout << "\tsetID: " << versionRecord->setID << "\n";
    swread(fd, &(versionRecord->conversionProgramVerMajor), 1);
    cout << "\tconversionProgramVerMajor: " << (int)versionRecord->conversionProgramVerMajor << "\n";
    swread(fd, &(versionRecord->conversionProgramVerMinor), 1);
    cout << "\tconversionProgramVerMinor: " << (int)versionRecord->conversionProgramVerMinor << "\n";
    swread(fd, &(versionRecord->leastCompatSTEPVerMajor), 1);
    cout << "\tleastCompatSTEPVerMajor: " << (int)versionRecord->leastCompatSTEPVerMajor << "\n";
    swread(fd, &(versionRecord->leastCompatSTEPVerMinor), 1);
    cout << "\tleastCompatSTEPVerMinor: " << (int)versionRecord->leastCompatSTEPVerMinor << "\n";
    swread(fd, &(versionRecord->encryptionType), 1);
    cout << "\tencryptionType: " << (int)versionRecord->encryptionType << "\n";
    swread(fd, &(versionRecord->editionID), 1);
    cout << "\teditionID: " << (int)versionRecord->editionID << "\n";
    swread(fd, &(versionRecord->modifiedBy), 2);
    cout << "\tmodifiedBy: " << versionRecord->modifiedBy << "\n";

    int skip = versionRecord->versionRecordSize - 16/*sizeof(struct Version*/;

    if (skip) {
        cout << "\nSkipping " << skip << " unknown bytes.\n";
        char *skipbuf = new char[skip];
        swread(fd, skipbuf, skip);
        delete [] skipbuf;
    }
}


void readViewableHeader(int fd, ViewableHeader *viewableHeaderRecord) {

    cout << "\n\nReading Viewable Header Record (" << 16/*sizeof(struct ViewableHeader)*/ << " bytes)\n\n";

// DO NOT USE BECAUSE OF RECORD BYTE ALIGNMENT PROBLEMS
//    read(fd, &viewableHeaderRecord, sizeof(struct ViewableHeader));

    cout << "Viewable Header Record Information\n";
    swread(fd, &(viewableHeaderRecord->viewableHeaderRecordSize), 2);
    cout << "\tviewableHeaderRecordSize: " << viewableHeaderRecord->viewableHeaderRecordSize << "\n";
    swread(fd, &(viewableHeaderRecord->viewableBlocksCount), 4);
    cout << "\tviewableBlocksCount: " << viewableHeaderRecord->viewableBlocksCount << "\n";
    swread(fd, &(viewableHeaderRecord->glossBlocksCount), 4);
    cout << "\tglossBlocksCount: " << viewableHeaderRecord->glossBlocksCount << "\n";
    swread(fd, &(viewableHeaderRecord->compressionType), 1);
    cout << "\tcompressionType: " << (int)viewableHeaderRecord->compressionType << "(0 - none; 1 - LZSS)\n";
    swread(fd, &(viewableHeaderRecord->reserved1), 1);
    cout << "\treserved1: " << (int)viewableHeaderRecord->reserved1 << "\n";
    swread(fd, &(viewableHeaderRecord->blockEntriesSize), 2);
    cout << "\tblockEntriesSize: " << viewableHeaderRecord->blockEntriesSize << "\n";
    swread(fd, &(viewableHeaderRecord->reserved2), 2);
    cout << "\treserved2: " << viewableHeaderRecord->reserved2 << "\n";

    int skip = viewableHeaderRecord->viewableHeaderRecordSize - 16/*sizeof(struct ViewableHeader)*/;

    if (skip) {
        cout << "\nSkipping " << skip << " unknown bytes.\n";
        char *skipbuf = new char[skip];
        swread(fd, skipbuf, skip);
        delete [] skipbuf;
    }
}


void readViewableBlockText(int fd, ViewableBlock *vb, char **buf) {
    unsigned long size = vb->size;

    *buf = new char [ ((vb->size > vb->uncompressedSize) ? vb->size : vb->uncompressedSize) + 1 ];
    lseek(fd, vb->offset, SEEK_SET);
    swread(fd, *buf, vb->size);

    compress->zBuf(&size, *buf);
    strcpy(*buf, compress->Buf());
    cout << "Viewable Block Text:\n";
    cout << *buf << "\n\n";
}


void readViewableBlock(int fd, ViewableBlock *vb) {

    cout << "\n\nReading Viewable Block (" << 12/*sizeof(struct ViewableHeader)*/ << " bytes)\n\n";

// DO NOT USE BECAUSE OF RECORD BYTE ALIGNMENT PROBLEMS
//    read(fd, &vb, sizeof(struct ViewableBlock));

    cout << "Viewable Block Information\n";
    swread(fd, &(vb->offset), 4);
    cout << "\toffset: " << vb->offset << "\n";
    swread(fd, &(vb->uncompressedSize), 4);
    cout << "\tuncompressedSize: " << vb->uncompressedSize << "\n";
    swread(fd, &(vb->size), 4);
    cout << "\tsize: " << vb->size << "\n";
}


void readHeaderControlWordAreaText(int fd, char **buf) {
    long headerControlWordAreaSize;
    swread(fd, &headerControlWordAreaSize, 4);
    cout << "Reading Header Control Word Area (" << headerControlWordAreaSize << " bytes)\n\n";

    *buf = new char [headerControlWordAreaSize + 1];

    swread(fd, *buf, headerControlWordAreaSize);
    (*buf)[headerControlWordAreaSize] = 0;

    cout << "headerControlWordArea:\n" << *buf << "\n";
}
