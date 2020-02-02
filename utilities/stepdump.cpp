/******************************************************************************
 *
 *  stepdump.cpp -    Utility to dump a STEP module
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
#include <memory>
#include <string>
#include <swordxx/filemgr.h>
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
void readHeaderControlWordAreaText(int fd);
void readViewableHeader(int fd, ViewableHeader *viewableHeaderRecord);
void readViewableBlock(int fd, ViewableBlock *vb);
void readViewableBlockText(int fd, ViewableBlock *vb);

SWCompress * compress = nullptr;

int main(int argc, char **argv) {

    compress = new LZSSCompress();
    Version versionRecord;
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
    int fd = FileMgr::openFileReadOnly(fileName.c_str());

    if (fd < 1) {
        cerr << "error, couldn't open file: " << fileName << "\n";
        std::exit(-2);
    }

    try {
        readVersion(fd, &versionRecord);
        readHeaderControlWordAreaText(fd);
    }
    catch (std::runtime_error &e) {
        std::cout << e.what () << '\n';
        std::exit(-3);
    }


    fileName = bookpath + "Viewable.idx";
    int fdv = FileMgr::openFileReadOnly(fileName.c_str());

    if (fdv < 1) {
        cerr << "error, couldn't open file: " << fileName << "\n";
        std::exit(-4);
    }

    try {
        readVersion(fdv, &versionRecord);
        readViewableHeader(fdv, &viewableHeaderRecord);
    }
    catch (std::runtime_error &e) {
        std::cout << e.what () << '\n';
        std::exit(-5);
    }

    ViewableBlock vb;

    cout << "\n\nReading special preface viewable BLOCK 0";

    try {
        readViewableBlock(fdv, &vb);
        readViewableBlockText(fd, &vb);
    }
    catch (std::runtime_error &e) {
        std::cout << e.what () << '\n';
        std::exit(-6);
    }

    int nonGlossBlocksCount = viewableHeaderRecord.viewableBlocksCount
                        - viewableHeaderRecord.glossBlocksCount;

    cout << "\n\nReading " << nonGlossBlocksCount << " non-glossary viewable blocks";
    // 1 because we already read the first block above
    for (int i = 1; i < nonGlossBlocksCount; i++) {
        cout << "\nNon-Glossary viewable block: " << i;
        readViewableBlock(fdv, &vb);
        readViewableBlockText(fd, &vb);
    }

    cout << "\n\nReading " << viewableHeaderRecord.glossBlocksCount << " glossary viewable blocks";
    for (int i = 0; i < viewableHeaderRecord.glossBlocksCount; i++) {
        cout << "\nGlossary viewable block: " << i;
        readViewableBlock(fdv, &vb);
        readViewableBlockText(fd, &vb);
    }

    close(fdv);
    close(fd);

}



void readVersion(int fd, Version *versionRecord) {

    string readError = "Error reading Version Record.";
    string seekError = "Error seeking Version Record.";

    cout << "\n\nReading Version Record (" << 16/*sizeof(struct Version)*/ << " bytes)\n\n";
// DO NOT USE BECAUSE OF RECORD BYTE ALIGNMENT PROBLEMS
//    read(fd, &versionRecord, sizeof(struct Version));

    cout << "Version Record Information\n";
    int rtn = read(fd, &(versionRecord->versionRecordSize), 2);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tversionRecordSize: " << versionRecord->versionRecordSize << "\n";
    rtn = read(fd, &(versionRecord->publisherID), 2);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tpublisherID: " << versionRecord->publisherID << "\n";
    rtn = read(fd, &(versionRecord->bookID), 2);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tbookID: " << versionRecord->bookID << "\n";
    rtn = read(fd, &(versionRecord->setID), 2);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tsetID: " << versionRecord->setID << "\n";
    rtn = read(fd, &(versionRecord->conversionProgramVerMajor), 1);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tconversionProgramVerMajor: " << (int)versionRecord->conversionProgramVerMajor << "\n";
    rtn = read(fd, &(versionRecord->conversionProgramVerMinor), 1);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tconversionProgramVerMinor: " << (int)versionRecord->conversionProgramVerMinor << "\n";
    rtn = read(fd, &(versionRecord->leastCompatSTEPVerMajor), 1);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tleastCompatSTEPVerMajor: " << (int)versionRecord->leastCompatSTEPVerMajor << "\n";
    rtn = read(fd, &(versionRecord->leastCompatSTEPVerMinor), 1);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tleastCompatSTEPVerMinor: " << (int)versionRecord->leastCompatSTEPVerMinor << "\n";
    rtn = read(fd, &(versionRecord->encryptionType), 1);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tencryptionType: " << (int)versionRecord->encryptionType << "\n";
    rtn = read(fd, &(versionRecord->editionID), 1);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\teditionID: " << (int)versionRecord->editionID << "\n";
    rtn = read(fd, &(versionRecord->modifiedBy), 2);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tmodifiedBy: " << versionRecord->modifiedBy << "\n";

    int skip = versionRecord->versionRecordSize - 16/*sizeof(struct Version*/;

    if (skip) {
        cout << "\nSkipping " << skip << " unknown bytes.\n";
        if (lseek(fd, skip, SEEK_CUR) < 0)
            throw runtime_error(seekError);
    }
}


void readViewableHeader(int fd, ViewableHeader *viewableHeaderRecord) {

    string readError = "Error reading Viewable Header.";
    string seekError = "Error seeking Viewable Header.";

    cout << "\n\nReading Viewable Header Record (" << 16/*sizeof(struct ViewableHeader)*/ << " bytes)\n\n";

// DO NOT USE BECAUSE OF RECORD BYTE ALIGNMENT PROBLEMS
//    read(fd, &viewableHeaderRecord, sizeof(struct ViewableHeader));

    cout << "Viewable Header Record Information\n";
    int rtn = read(fd, &(viewableHeaderRecord->viewableHeaderRecordSize), 2);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tviewableHeaderRecordSize: " << viewableHeaderRecord->viewableHeaderRecordSize << "\n";
    rtn = read(fd, &(viewableHeaderRecord->viewableBlocksCount), 4);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tviewableBlocksCount: " << viewableHeaderRecord->viewableBlocksCount << "\n";
    rtn = read(fd, &(viewableHeaderRecord->glossBlocksCount), 4);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tglossBlocksCount: " << viewableHeaderRecord->glossBlocksCount << "\n";
    rtn = read(fd, &(viewableHeaderRecord->compressionType), 1);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tcompressionType: " << (int)viewableHeaderRecord->compressionType << "(0 - none; 1 - LZSS)\n";
    rtn = read(fd, &(viewableHeaderRecord->reserved1), 1);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\treserved1: " << (int)viewableHeaderRecord->reserved1 << "\n";
    rtn = read(fd, &(viewableHeaderRecord->blockEntriesSize), 2);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tblockEntriesSize: " << viewableHeaderRecord->blockEntriesSize << "\n";
    rtn = read(fd, &(viewableHeaderRecord->reserved2), 2);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\treserved2: " << viewableHeaderRecord->reserved2 << "\n";

    int skip = viewableHeaderRecord->viewableHeaderRecordSize - 16/*sizeof(struct ViewableHeader)*/;

    if (skip) {
        cout << "\nSkipping " << skip << " unknown bytes.\n";
        if (lseek(fd, skip, SEEK_CUR) < 0)
            throw runtime_error(seekError);
    }
}


void readViewableBlockText(int fd, ViewableBlock * vb) {
    string readError = "Error reading Viewable Block Text.";

    unsigned long size = vb->size;

    auto buf(std::make_unique<char[]>(((vb->size > vb->uncompressedSize)
                                       ? vb->size
                                       : vb->uncompressedSize) + 1));
    lseek(fd, vb->offset, SEEK_SET);
    int rtn = read(fd, buf.get(), vb->size);
    if (rtn <= 0) throw runtime_error(readError);

    compress->zBuf(&size, buf.get());
    std::strcpy(buf.get(), compress->Buf());
    cout << "Viewable Block Text:\n" << buf.get() << "\n\n";
}


void readViewableBlock(int fd, ViewableBlock *vb) {
    string readError = "Error reading Viewable Block.";

    cout << "\n\nReading Viewable Block (" << 12/*sizeof(struct ViewableHeader)*/ << " bytes)\n\n";

// DO NOT USE BECAUSE OF RECORD BYTE ALIGNMENT PROBLEMS
//    read(fd, &vb, sizeof(struct ViewableBlock));

    cout << "Viewable Block Information\n";
    int rtn = read(fd, &(vb->offset), 4);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\toffset: " << vb->offset << "\n";
    rtn = read(fd, &(vb->uncompressedSize), 4);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tuncompressedSize: " << vb->uncompressedSize << "\n";
    rtn = read(fd, &(vb->size), 4);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "\tsize: " << vb->size << "\n";
}


void readHeaderControlWordAreaText(int fd) {
    string readError = "Error reading Control Word Area.";

    long headerControlWordAreaSize;
    int rtn = read(fd, &headerControlWordAreaSize, 4);
    if (rtn <= 0) throw runtime_error(readError);
    cout << "Reading Header Control Word Area (" << headerControlWordAreaSize << " bytes)\n\n";

    auto const buf(std::make_unique<char[]>(headerControlWordAreaSize + 1));

    rtn = read(fd, buf.get(), headerControlWordAreaSize);
    if (rtn <= 0) throw runtime_error(readError);
    buf[headerControlWordAreaSize] = 0;

    cout << "headerControlWordArea:\n" << buf.get() << "\n";
}
