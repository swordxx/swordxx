/* Output from p2c, the Pascal-to-C translator */
/* From input file "Gbf.pas" */


#include <p2c/p2c.h>


typedef enum {
  tokNull, tokEOF, tokHeader, tokContent, tokTail, tokStyle, tokWord,
  tokSpace, tokSync, tokControl, tokChar, tokFont
} TToken;
typedef enum {
  caBold, caSmallCaps, caItalic, caOTQuote, caRed, caSuperscript, caUnderline,
  caSubscript
} TCharacterAttribute;
typedef long TCharAttribs;



typedef struct TBookNameRec {
  Char Name[256], Abbr[256];
  uchar Num;
} TBookNameRec;

typedef TBookNameRec TBookAbbr[116];


typedef struct TReadGBF {
  /*        private*/
  FILE *F;
  Char FName[256], TokenLine[256];
  long TokenPos;
  boolean fFileIsOpen, fParagraphEnd, fInTitle, fInPsalmBookTitle,
	  fInHebrewTitle, fInSectionTitle;

  /*        public*/
  Char sBook[256], sChapter[256], sVerse[256], sMode[256];
  Char sContext[256];   /*// Last text type (header, body, or tail)*/
  Char sTitle[256];   /*// Title of this book of the Bible*/
  Char sPsalmBookTitle[256];   /*// Title of this Psalm book*/
  Char sHebrewTitle[256];   /*// Psalm Hebrew title*/
  Char sSectionTitle[256];   /*// Section headings*/
  Char sDate[256], sFontName[256];
  long iTotalWords;
  Char chJustification, chDirection;
  boolean fIndent, fPoetry;
  TCharAttribs CharAttribs;
  uchar bBk, bChap, bVs, bWd;
  /*
          function Init({const*/
  /*sFileName: string): boolean;
          procedure Done;
          function GetToken(var TokenKind: TToken): string;
  */
} TReadGBF;

typedef struct TWriteGBF {
  /*        private*/
  FILE *F;
  Char FName[256], LineOut[256];
  boolean fFileIsOpen;
  uchar bBk, bChap, bVs, bWd;

  /*        public*/
  /*
          function Init({const*/
  /*sFileName: string): boolean;
          function Done: boolean;
          procedure Out({const*/
  /*s: string);
*/
} TWriteGBF;

/*implementation */


/*//0*/
/*//1*/
/*//2*/
/*//3*/
/*//4*/
/*//5*/
/*//6*/
/*//7*/
/*//8*/
/*//9*/
/*//10*/
/*//11*/
/*//12*/
/*//13*/
/*//14*/
/*//15*/
/*//16*/
/*//17*/
/*//18*/
/*//19*/
/*//20*/
/*//21*/
/*//22*/
/*//0*/
/*//1*/
/*//2*/
/*//3*/
/*//4*/
/*//5*/
/*//6*/
/*//7*/
/*//8*/
/*//9*/
/*//10*/
/*//11*/
/*//12*/
/*//13*/
/*//14*/
/*//15*/
/*//16*/
/*//17*/
/*//18*/
/*//19*/
/*//20*/
/*//21*/
/*//22*/
/*//23*/
/*//24*/
/*//26*/
/*//27*/
/*//28*/
/*//29*/
/*//30*/
/*//31*/
/*//32*/
/*//33*/
/*//34*/
/*//35*/
/*//36*/
/*//37*/
/*//38*/
/*//39*/
/*//40*/
/*//41*/
/*//42*/
/*//43*/
/*//44*/
/*//45*/
/*//46*/
/*//47*/
/*//48*/
/*//49*/
/*//50*/
/*//51*/
/*//52*/
/*//53*/
/*//54*/
/*//55*/
/*//56*/
/*//57*/
/*//58*/
/*//59*/
/*//60*/
/*//61*/
/*//62*/
/*//63*/
/*//64*/
/*//65*/
/*//66*/
/*//67*/
/*//68*/
/*//69*/
/*//70*/
/*//71*/
/*//72*/
/*//73*/
/*//74*/
/*//75*/
/*//76*/
/*//77*/
/*//78*/
/*//79*/
/*//80*/
/*//81*/
/*//82*/
/*//83*/
/*//84*/
/*//85*/
/*//86*/
/*//87*/
/*//88*/
/*//89*/
/*//90*/
/*//91*/
/*//92*/

Static TBookAbbr BookAbbr = {
  { "1 Chronicles", "1CH", 13 },
  { "1 Corinthians", "1CO", 70 },
  { "1 Esdras", "1E", 52 },
  { "1 John", "1J", 86 },
  { "1 Kings", "1K", 11 },
  { "1 Maccabees", "1M", 50 },
  { "1 Peter", "1P", 84 },
  { "1 Samuel", "1S", 9 },
  { "1 Thessalonians", "1TH", 76 },
  { "1 Timothy", "1TI", 78 },
  { "2 Chronicles", "2CH", 14 },
  { "2 Corinthians", "2CO", 71 },
  { "2 Esdras", "2E", 56 },
  { "2 John", "2J", 87 },
  { "2 Kings", "2K", 12 },
  { "2 Maccabees", "2M", 51 },
  { "2 Peter", "2P", 85 },
  { "2 Samuel", "2S", 10 },
  { "2 Thessalonians", "2TH", 77 },
  { "2 Timothy", "2TI", 79 },
  { "3 John", "3J", 88 },
  { "3 Maccabees", "3M", 55 },
  { "4 Maccabees", "4M", 57 },
  { "1 Chronicles", "1 CH", 13 },
  { "1 Corinthians", "1 CO", 70 },
  { "1 Esdras", "1 E", 52 },
  { "1 John", "1 J", 86 },
  { "1 Kings", "1 K", 11 },
  { "1 Maccabees", "1 M", 50 },
  { "1 Peter", "1 P", 84 },
  { "1 Samuel", "1 S", 9 },
  { "1 Thessalonians", "1 TH", 76 },
  { "1 Timothy", "1 TI", 78 },
  { "2 Chronicles", "2 CH", 14 },
  { "2 Corinthians", "2 CO", 71 },
  { "2 Esdras", "2 E", 56 },
  { "2 John", "2 J", 87 },
  { "2 Kings", "2 K", 12 },
  { "2 Maccabees", "2 M", 51 },
  { "2 Peter", "2 P", 85 },
  { "2 Samuel", "2 S", 10 },
  { "2 Thessalonians", "2 TH", 77 },
  { "2 Timothy", "2 TI", 79 },
  { "3 John", "3 J", 88 },
  { "3 Maccabees", "3 M", 55 },
  { "4 Maccabees", "4 M", 57 },
  { "Acts", "AC", 68 },
  { "Amos", "AM", 30 },
  { "Prayer of Asariah and the Song of the Three Jews", "AZ", 47 },
  { "Baruch", "BA", 45 },
  { "Bel and the Dragon", "BE", 49 },
  { "Colossians", "CO", 75 },
  { "Daniel", "DA", 27 },
  { "Deuteronomy", "DE", 5 },
  { "Deuteronomy", "DT", 5 },
  { "Ecclesiasties", "EC", 21 },
  { "Esther", "ES", 17 },
  { "Exodus", "EX", 2 },
  { "Ezekiel", "EZE", 26 },
  { "Ezra", "EZR", 15 },
  { "Galatians", "GA", 72 },
  { "Genesis", "GE", 1 },
  { "Genesis", "GN", 1 },
  { "Ephesians", "EP", 73 },
  { "Esther (Greek)", "GR", 42 },
  { "Habakkuk", "HAB", 35 },
  { "Haggai", "HAG", 37 },
  { "Hebrews", "HE", 82 },
  { "Hosea", "HO", 28 },
  { "Isaiah", "IS", 23 },
  { "James", "JA", 83 },
  { "Jeremiah", "JE", 24 },
  { "Job", "JOB", 18 },
  { "Joel", "JOE", 29 },
  { "John", "JOH", 67 },
  { "Jonah", "JON", 32 },
  { "Joshua", "JOS", 6 },
  { "Jude", "JUDE", 89 },
  { "Judges", "JUDG", 7 },
  { "Judith", "JUDI", 41 },
  { "Lamentations", "LA", 25 },
  { "Letter of Jeremiah", "LET", 46 },
  { "Leviticus", "LEV", 3 },
  { "Luke", "LK", 66 },
  { "Leviticus", "LV", 3 },
  { "Luke", "LU", 66 },
  { "Malachi", "MAL", 39 },
  { "Prayer of Manasseh", "MAN", 53 },
  { "Mark", "MAR", 65 },
  { "Matthew", "MAT", 64 },
  { "Micah", "MI", 33 },
  { "Nahum", "NA", 34 },
  { "Nehemiah", "NE", 16 },
  { "Numbers", "NU", 4 },
  { "Obadiah", "OB", 31 },
  { "Psalm 151", "P1", 54 },
  { "Philemon", "PHILE", 81 },
  { "Philippians", "PHILI", 74 },
  { "Philemon", "PHM", 81 },
  { "Philippians", "PHP", 74 },
  { "Proverbs", "PR", 20 },
  { "Psalms", "PS", 19 },
  { "Revelation", "RE", 90 },
  { "Romans", "RM", 69 },
  { "Romans", "RO", 69 },
  { "Ruth", "RU", 8 },
  { "Sirach", "SI", 44 },
  { "Song of Solomon", "SOL", 22 },
  { "Song of Solomon", "SON", 22 },
  { "Song of Solomon", "SS", 22 },
  { "Susanna", "SU", 48 },
  { "Titus", "TI", 80 },
  { "Tobit", "TO", 40 },
  { "Wisdom", "WI", 43 },
  { "Zechariah", "ZEC", 38 },
  { "Zephaniah", "ZEP", 36 }
};

/*// 0 - 7*/
/*// 8 - 14*/
/*// 15-20*/
/*// 21-26*/
/*// 27-33*/
/*// 34-39*/
/*// 40-45*/
/*// 46-52*/
/*// 53-63*/
/*// 64-70*/
/*// 71-78*/
/*// 79-84*/

Static Char BookFileName[91][256] = {
  "", "Genesis", "Exodus", "Lev", "Num", "Deut", "Joshua", "Judges", "Ruth",
  "1Sam", "2Sam", "1Kings", "2Kings", "1Chron", "2Chron", "Ezra", "Nehemiah",
  "Esther", "Job", "Psalms", "Proverbs", "Eccl", "Song", "Isaiah", "Jeremiah",
  "Lament", "Ezekiel", "Daniel", "Hosea", "Joel", "Amos", "Obadiah", "Jonah",
  "Micah", "Nahum", "Habakkuk", "Zeph", "Haggai", "Zech", "Malachi", "Tobit",
  "Judith", "Esther", "Wisdom", "Sirach", "Baruch", "Let", "Azar", "Susanna",
  "Bel", "1Mac", "2Mac", "1Esdras", "Man", "P1", "3Mac", "2Esdras", "4Mac",
  "", "", "", "", "", "", "Matthew", "Mark", "Luke", "John", "Acts", "Romans",
  "1Cor", "2Cor", "Gal", "Eph", "Philip", "Col", "1Thes", "2Thes", "1Tim",
  "2Tim", "Titus", "Philemon", "Hebrews", "James", "1Peter", "2Peter",
  "1John", "2John", "3John", "Jude", "Rev"
/* p2c: Gbf.pas, line 200: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 336 [251] */
};   /*// 85-90*/


Static boolean isletter(ch)
Char ch;
{
  /*const*/
  boolean Result;

  if (isupper(ch)) {
    Result = true;
    return Result;
  }
  if (islower(ch))
    Result = true;
  else
    Result = false;
  return Result;
}


Static boolean isinword(ch)
Char ch;
{
  /*const*/
  boolean Result;

  switch (ch) {

  case '-':
    Result = true;
    break;

  default:
    if (isupper(ch))
      Result = true;
    else if (islower(ch))
      Result = true;
    else
      Result = false;
    break;
  }
  return Result;
}


Static boolean IsUpper(ch)
Char ch;
{
  /*const*/
  boolean Result;

  if (isupper(ch))
    Result = true;
  else
    Result = false;
  return Result;
}


Static boolean IsDigit(ch)
Char ch;
{
  /*const*/
  boolean Result;

  if (isdigit(ch))
    Result = true;
  else
    Result = false;
  return Result;
}


Static boolean MatchAbbrev(sName, sAbbrev)
Char *sName, *sAbbrev;
{
  /*const*/
  long i;
  boolean Result;

  if (strlen(sName) < strlen(sAbbrev)) {
    Result = false;
/* p2c: Gbf.pas, line 245: Warning: Symbol 'RESULT' is not defined [221] */
  } else
    Result = true;
  i = 1;
  while (i <= strlen(sAbbrev) && Result) {
    if (toupper(sName[i - 1]) != sAbbrev[i - 1])
      Result = false;
    i++;
  }
}


Static uchar BookNameToNumber(sBookName)
Char *sBookName;
{
  /*const*/
  long Result;

  Result = 0;
/* p2c: Gbf.pas, line 259: Warning: Symbol 'RESULT' is not defined [221] */
  TRY(try1);
    if (IsDigit(sBookName[strlen(sBookName) - 1]) & IsDigit(sBookName[0])) {
      Result = StrToInt(sBookName);
/* p2c: Gbf.pas, line 262:
 * Warning: Symbol 'STRTOINT' is not defined [221] */
    }
    except();
/* p2c: Gbf.pas, line 264: Warning: Symbol 'EXCEPT' is not defined [221] */
/* p2c: Gbf.pas, line 264:
 * Warning: Expected RECOVER, found 'Result' [227] */
  RECOVER(try1);
    ;
  ENDTRY(try1);
}


main(argc, argv)
int argc;
Char *argv[];
{   /*// Yuk! Linear search.*/
  Char STR1[256];
  uchar Result;

/* p2c: Gbf.pas, line 266: Warning: Expected BEGIN, found 'i' [227] */
  PASCAL_MAIN(argc, argv);
  if (MatchAbbrev(sBookName, BookAbbr[i].Abbr)) {
/* p2c: Gbf.pas, line 269:
 * Warning: Symbol 'SBOOKNAME' is not defined [221] */
/* p2c: Gbf.pas, line 269: Warning: Mixing non-strings with strings [170] */
/* p2c: Gbf.pas, line 269: Warning: Symbol 'I' is not defined [221] */
    Result = BookAbbr[i].Num;
/* p2c: Gbf.pas, line 271: Warning: Symbol 'I' is not defined [221] */
/* p2c: Gbf.pas, line 271: Warning: Symbol 'RESULT' is not defined [221] */
  }
/* p2c: Gbf.pas, line 273: Warning: Symbol 'I' is not defined [221] */
  i++;
  exit(EXIT_SUCCESS);
}
/* p2c: Gbf.pas, line 275: 
 * Warning: Junk at end of input file ignored [277] */



/* End. */
