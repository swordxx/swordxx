/*****************************************************************************
 *
 *  gbfidx.cpp -    This code reeks but works (at least for WEB).
 *            Good luck!
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#ifndef __GNUC__
#include <io.h>
#endif
#include <swordxx/filemgr.h>
#include <swordxx/keys/versekey.h>
#include <swordxx/utilstr.h>
#ifdef __GNUC__
#include <unistd.h>
#endif


using namespace swordxx;

void writeidx(VerseKey &g_key1, VerseKey &g_key2, VerseKey &g_key3, long offset, short size);
char findbreak(int g_fp, long *offset, int *num1, int *num2, int *rangemax, short *size);
void openfiles(char *fname);
void checkparams(int argc, char **argv);


VerseKey g_key1, g_key2, g_key3;
int g_fp, g_vfp, g_cfp, g_bfp;
long g_chapoffset;
short g_chapsize;
char g_testmnt;


int main(int argc, char **argv)
{
    long pos, offset;
    int num1, num2, rangemax;
    char startflag = 0;
    short size;

    checkparams(argc, argv);

    openfiles(argv[1]);

    g_testmnt = g_key1.getTestament();
    num1 = g_key1.getChapter();
    num2 = g_key1.getVerse();
    pos  = 0;
    write(g_bfp, &pos, 4);  /* Book    offset for testament intros */
    pos = 4;
    write(g_cfp, &pos, 4);  /* Chapter offset for testament intro */


/*    Right now just zero out intros until parsing correctly */
    pos = 0;
    size = 0;
    write(g_vfp, &pos, 4);  /* Module intro */
    write(g_vfp, &size, 2);
    write(g_vfp, &pos, 4);  /* Testament intro */
    write(g_vfp, &size, 2);

    while(!findbreak(g_fp, &offset, &num1, &num2, &rangemax, &size)) {
        if (!startflag) {
            startflag = 1;
        }
        else {
            if (num2 < g_key2.getVerse()) {            // new chapter
                if (num1 <= g_key2.getChapter()) { // new book
                    g_key2.setVerse(1);
                    g_key2.setChapter(1);
                    g_key2.setBook(g_key2.getBook()+1);
                }
                printf("Found Chapter Break: %d ('%s')\n", num1, g_key2.getText().c_str());
                g_chapoffset = offset;
                g_chapsize = size;
//                continue;
            }
        }
        g_key2.setVerse(1);
        g_key2.setChapter(num1);
        g_key2.setVerse(num2);

        g_key3 = g_key2;
//        key3 += (rangemax - key3.getVerse());

        writeidx(g_key1, g_key2, g_key3, offset, size);
    }
    close(g_vfp);
    close(g_cfp);
    close(g_bfp);
    close(g_fp);
    return 0;
}


/**************************************************************************
 * ENT:    key1    - current location of index
 *     key2    - minimum keyval for which this offset is valid
 *     key3    - maximum keyval for which this offset is valid
 */

void writeidx(VerseKey &key1, VerseKey &key2, VerseKey &key3, long offset, short size)
{
    long pos;
    short tmp;

    for (; ((key1 <= key3) && (key1.popError() != KEYERR_OUTOFBOUNDS) && (key1.getTestament() == g_testmnt)); key1.increment()) {
        if (key1.getVerse() == 1) {    // new chapter
            if (key1.getChapter() == 1) {    // new book
                pos = lseek(g_cfp, 0, SEEK_CUR);
                write(g_bfp, &pos, 4);
                pos = lseek(g_vfp, 0, SEEK_CUR); /* Book intro (cps) */
                write(g_cfp, &pos, 4);
                write(g_vfp, &g_chapoffset, 4);  /* Book intro (vss)  set to same as chap for now(it should be chap 1 which usually contains the book into anyway)*/
                write(g_vfp, &g_chapsize, 2);
            }
            pos = lseek(g_vfp, 0, SEEK_CUR);
            write(g_cfp, &pos, 4);
            write(g_vfp, &g_chapoffset, 4);  /* Chapter intro */
            write(g_vfp, &g_chapsize, 2);
        }
        if (key1 >= key2) {
            write(g_vfp, &offset, 4);
            write(g_vfp, &size, 2);
        }
        else    {
            pos = 0;
            tmp = 0;
            write(g_vfp, &pos, 4);
            write(g_vfp, &tmp, 2);
        }
    }
}


char startchap(char *buf)
{
    if (buf[0] != '<')
        return 0;
    if (buf[1] != 'S')
        return 0;
    if (buf[2] != 'C')
        return 0;
/*
    if (!charIsDigit(buf[2]))
        return 0;
    for (loop = 3; loop < 7; loop++) {
        if (buf[loop] == ' ')
            break;
        if ((!charIsDigit(buf[loop])) && (buf[loop] != ',') && (buf[loop] != '-'))
            return 0;
    }
*/
    return 1;
}


char startentry(char *buf)
{
    if (buf[0] != '<')
        return 0;
    if (buf[1] != 'S')
        return 0;
    if (buf[2] != 'V')
        return 0;
/*
    if (!charIsDigit(buf[2]))
        return 0;
    for (loop = 3; loop < 7; loop++) {
        if (buf[loop] == ' ')
            break;
        if ((!charIsDigit(buf[loop])) && (buf[loop] != ',') && (buf[loop] != '-'))
            return 0;
    }
*/
    return 1;
}


char findbreak(int fp, long *offset, int *num1, int *num2, int * /* rangemax */, short *size)
{
    char buf[7];
    int loop;
    long offset2;
    int ch2, vs2, rm2;
    bool flag;
    long chapstart = 0;

    std::memset(buf, ' ', 7);

    while (1) {
        if (startchap(buf)) {
            chapstart = lseek(fp, 0, SEEK_CUR) - 7;
            std::memset(buf, ' ', 3);
            flag = false;
            for (loop = 3; loop < 6; loop++) {
                if (charIsDigit(buf[loop]))
                    flag = true;
                else {
                    buf[loop] = 0;
                    break;
                }
            }
            if (flag)
                *num1 = std::atoi(buf);
            else    (*num1)++;
        }
        if (startentry(buf)) {
            std::memset(buf, ' ', 3);
            flag = false;
            for (loop = 3; loop < 6; loop++) {
                if (charIsDigit(buf[loop]))
                    flag = true;
                else {
                    buf[loop] = 0;
                    break;
                }
            if (flag)
                *num2 = std::atoi(buf);
            else    (*num2)++;
            }
            loop++;
            if (size)
                *offset = lseek(fp, 0, SEEK_CUR) - (7 - loop);
            else    *offset = (chapstart) ? chapstart : lseek(fp, 0, SEEK_CUR) - 7;
            if (size) {
                ch2 = *num1;
                vs2 = *num2;
                if (findbreak(fp, &offset2, &ch2, &vs2, &rm2, nullptr)) {
                    *size = (short) (lseek(fp, 0, SEEK_END) - (*offset));
                }
                else {
                    if (vs2) {
                        *size = (offset2 - (*offset));
                    }
                }
                lseek(fp, *offset, SEEK_SET);
            }
            return 0;
        }
        std::memmove(buf, &buf[1], 6);
        if (read(fp, &buf[6], 1) != 1)
            return 1;
    }
}


void openfiles(char *fname)
{

    if ((g_fp = FileMgr::openFileReadOnly(fname)) < 0) {
        fprintf(stderr, "Couldn't open file: %s\n", fname);
        std::exit(1);
    }

    std::string buf(formatted("%s.vss", fname));
    if ((g_vfp = FileMgr::createPathAndFile(buf.c_str())) < 0) {
        fprintf(stderr, "Couldn't open file: %s\n", buf.c_str());
        std::exit(1);
    }

    buf = formatted("%s.cps", fname);
    if ((g_cfp = FileMgr::createPathAndFile(buf.c_str())) < 0) {
        fprintf(stderr, "Couldn't open file: %s\n", buf.c_str());
        std::exit(1);
    }

    buf = formatted("%s.bks", fname);
    if ((g_bfp = FileMgr::createPathAndFile(buf.c_str())) < 0) {
        fprintf(stderr, "Couldn't open file: %s\n", buf.c_str());
        std::exit(1);
    }
}


void checkparams(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file to process> [nt - for new testmt file]\n", argv[0]);
        std::exit(1);
    }
    if (argc == 3) {
        g_key3.setText("Matthew 1:1");
    } else {
        g_key3.setText("Genesis 1:1");
    }
    g_key2.positionFrom(g_key3);
    g_key1.positionFrom(g_key2);
}
