/*
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
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

#include <online.dcl>
struct zonline online;
struct zversion version;
struct zref ref;
struct zstdindex stdindex;
struct ztrace trace;
/*
  Initialises the System

  Inputs:
      pointer to path for files
         e.g. c:\bible\scholar
*/
void DOINIT(CHAR *olbpath)
{
 static CHAR *titles[10] =
    {"\\id.dat",      "\\text.dat", "\\textndx.dat", "\\tokens.dat",
     "\\version.dat", "\\xref.dat", "\\xrefndx.dat", "\\xrefndxs.dat",
     "\\cxref\\cxref.dat", "\\cxref\\cxrefndx.dat"};

 CHAR  path[64];
 static long int zero ={0};

 struct
    {
     CHAR key;
     CHAR size[2];
     INT indexsize;
     int token_blksize;
    } record;

 int   i;
 int   j;

 /*
   Open Files
 */

 trace.level = -1;
 X("Doinit");
 for (i = 0; i < 8; i++)
    {
     STRCPY(path, olbpath, 64, "Doinit-1");
     STRCAT(path, titles[i], 64,"Doinit-2");
     call OPEN(i, path);
     online.lrecl[i] = 0;
    }

 /*
   Initialise Version Structure
 */
 call READ(fp_version, sizeof(version), &version.version_id);
 call CLOSE(fp_version);
 call DUMPLINE("VERSION.DAT", 16, &version.version_id);
                 /* convert PLI strings to C-Weed format */
/*
 for (i = 0; i < 128; i++)
    call PLITOC(&(version.hot_words[i][0]), 9);
*/
 for (i = 0; i < 66; i++)
    call PLITOC(&(version.books[i][0]), 15);

 /*
   setup access to TEXT.DAT file
 */
 call DREAD(fp_textndx, 3, &record.key, zero);
 call DUMPLINE("TEXTNDX.DAT", 3, &record.key);

 online.textndxsz = record.size[0];
 if (record.key == 0) then
    online.lrecl[fp_textndx] = 3;
 else
    online.lrecl[fp_textndx] = record.size[0];

 /*
   setup access to XREF.DAT file
 */
 call DREAD(fp_xrefndx, 7, &record.key, zero);
 call DUMPLINE("XREFNDX.DAT", 7, &record.key);
 online.lrecl[fp_xrefndx] = record.size[0] + record.size[1];
 online.xrefndxsz[0] = record.size[0];
 online.xrefndxsz[1] = record.size[1];
 if (record.key == 2) then
    {
     online.indexsize = record.indexsize;
     online.tokenblksize = record.token_blksize * 2;
     online.tokenarea = &online.tokenwork[0];
    }
 else
     online.tokenblksize = 0;
 /*
   setup access to XREFNDXS.DAT file
 */
 call DREAD(fp_xrefndxs, 3, &record.key, zero);
 call DUMPLINE("XREFNDXS.DAT", 3, &record.key);
 if (record.key == 0) then
    online.lrecl[fp_xrefndxs] = 3;
 else
    online.lrecl[fp_xrefndxs] = record.size[0];

 online.version_mark = 3;
 /*
   Setup Version ID for multiple versions
 */
 call VERINIT();

 /*
   Setup Cross Reference Files
 */
 for (i = 8; i < 10; i++)
    {
     STRCPY(path, olbpath, 64, "Doinit-3");
     j = strlen(path);
     while(path[j] != '\\')
        j--;
     path[j] = 0;
     STRCAT(path, titles[i], 64, "Doinit-4");
     call OPEN(i, path);
     online.lrecl[i] = 0;
    }
 /*
   setup access to CXREF.DAT file
 */
 call DREAD(fp_cxrefndx, 3, &record.key, zero);
 call DUMPLINE("CXREFNDX.DAT", 3, &record.key);
 if (record.key == 0) then
    online.lrecl[fp_cxrefndx] = 3;
 else
    online.lrecl[fp_cxrefndx] = record.size[0];
 trace.traceflag = 1;
 Y();
}
