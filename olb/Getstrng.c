#include <online.dcl>

void GETSTRNG(strongs_no, verse_list, otflg)

                           /* get verses for a given strongs no */

int strongs_no;            /* strongs number to fetch verse list for */
INT verse_list[MAX_LIST];  /* resultant verse list */
CHAR otflg;                /* Old Testament Number flag */
{
 extern struct zversion version;
 int entry_size;
 long int seek_posn;

 X("Getstrng");
 if (strongs_no > 8849 || strongs_no < 0) then
    ERROR("Getstrng - Invalid Strong's Number");

 entry_size = READNDX(strongs_no, fp_xrefndxs, &seek_posn);
 call LISTLOAD(fp_xref, entry_size, verse_list, seek_posn);
 if (otflg == 0) then
    call LISTRANG(verse_list, version.nt_ver, version.total_ver);
 else
    call LISTRANG(verse_list, 1, version.nt_ver - 1);
 Y();
}
