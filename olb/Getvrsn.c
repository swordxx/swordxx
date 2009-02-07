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

/*
  Get version id for display

  If triple verse A_B_C then

  Inputs:
      1 - returns "A"
      2 - returns "B"
      3 - returns "A_B"
      4 - returns "C"
      5 - returns "A_C"
      6 - returns "B_C"
      7 - returns "A_B_C"
*/

CHAR *GETVRSN(int version)
{
 extern struct zonline online;
 static CHAR versionid[16];

 X("Getvrsn");
 if (version < 1 || version > 7) then
    call ERROR("Version Id Invalid");
 versionid[0] = 0;
 if (version & 1) then
    {
     strcat(versionid, "_");
     STRCAT(versionid, &online.versions[0][0], 16, "Getrsn-1");
    }
 if (version & 2) then
    {
     if (online.versions[1][0] != 0) then
        {
         strcat(versionid, "_");
         STRCAT(versionid, &online.versions[1][0], 16, "Getrsn-2");
        }
    }
 if (version & 4) then
    {
     if (online.versions[2][0] != 0) then
        {
         strcat(versionid, "_");
         STRCAT(versionid, &online.versions[2][0], 16, "Getrsn-3");
        }
    }
 Y();
 return(&versionid[1]);
}
