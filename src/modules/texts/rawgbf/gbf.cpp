enum TToken {
tokNull, tokEOF, tokHeader, tokContent, tokTail, tokStyle,
	      tokWord, tokSpace, tokSync, tokControl, tokChar, tokFont};

enum TCharacterAttribute { caBold, caSmallCaps, caItalic, caOTQuote, caRed,
			    caSuperscript, caUnderline, caSubscript};

//    TCharAttribs = set of TCharacterAttribute;


struct TBookNameRec {
	string Name, Abbr;
	char Num;
}

const struct TBookNameRec TBookAbbr[116] = {
	{"1 Chronicles",    "1CH", 13},   //0
	{"1 Corinthians",   "1CO", 70},   //1
	{"1 Esdras",        "1E", 52},    //2
	{"1 John",          "1J", 86},    //3
	{"1 Kings",         "1K", 11},    //4
	{"1 Maccabees",     "1M", 50},    //5
	{"1 Peter",         "1P", 84},    //6
	{"1 Samuel",        "1S", 9},     //7
	{"1 Thessalonians", "1TH", 76},   //8
	{"1 Timothy",       "1TI", 78},   //9
	{"2 Chronicles",    "2CH", 14},   //10
	{"2 Corinthians",   "2CO", 71},   //11
	{"2 Esdras",        "2E", 56},    //12
	{"2 John",          "2J", 87},    //13
	{"2 Kings",         "2K", 12},    //14
	{"2 Maccabees",     "2M", 51},    //15
	{"2 Peter",         "2P", 85},    //16
	{"2 Samuel",        "2S", 10},    //17
	{"2 Thessalonians", "2TH", 77},   //18
	{"2 Timothy",       "2TI", 79},   //19
	{"3 John",          "3J", 88},    //20
	{"3 Maccabees",     "3M", 55},    //21
	{"4 Maccabees",     "4M", 57},    //22
	{"1 Chronicles",    "1 CH", 13},   //0
	{"1 Corinthians",   "1 CO", 70},   //1
	{"1 Esdras",        "1 E", 52},    //2
	{"1 John",          "1 J", 86},    //3
	{"1 Kings",         "1 K", 11},    //4
	{"1 Maccabees",     "1 M", 50},    //5
	{"1 Peter",         "1 P", 84},    //6
	{"1 Samuel",        "1 S", 9},     //7
	{"1 Thessalonians", "1 TH", 76},   //8
	{"1 Timothy",       "1 TI", 78},   //9
	{"2 Chronicles",    "2 CH", 14},   //10
	{"2 Corinthians",   "2 CO", 71},   //11
	{"2 Esdras",        "2 E", 56},    //12
	{"2 John",          "2 J", 87},    //13
	{"2 Kings",         "2 K", 12},    //14
	{"2 Maccabees",     "2 M", 51},    //15
	{"2 Peter",         "2 P", 85},    //16
	{"2 Samuel",        "2 S", 10},    //17
	{"2 Thessalonians", "2 TH", 77},   //18
	{"2 Timothy",       "2 TI", 79},   //19
	{"3 John",          "3 J", 88},    //20
	{"3 Maccabees",     "3 M", 55},    //21
	{"4 Maccabees",     "4 M", 57},    //22
	{"Acts",            "AC", 68},    //23
	{"Amos",            "AM", 30},    //24
	{"Prayer of Asariah and the Song of the Three Jews", "AZ", 47},
	{"Baruch",          "BA", 45},    //26
	{"Bel and the Dragon","BE", 49},  //27
	{"Colossians",      "CO", 75},    //28
	{"Daniel",          "DA", 27},    //29
	{"Deuteronomy",     "DE", 5},     //30
	{"Deuteronomy",     "DT", 5},     //31
	{"Ecclesiasties",   "EC", 21},    //32
	{"Esther",          "ES", 17},    //33
	{"Exodus",          "EX", 2},     //34
	{"Ezekiel",         "EZE", 26},   //35
	{"Ezra",            "EZR", 15},   //36
	{"Galatians",       "GA", 72},    //37
	{"Genesis",         "GE", 1},     //38
	{"Genesis",         "GN", 1},     //39
	{"Ephesians",       "EP", 73},    //40
	{"Esther (Greek}",  "GR", 42),    //41
	{"Habakkuk",        "HAB", 35},   //42
	{"Haggai",          "HAG", 37},   //43
	{"Hebrews",         "HE", 82},    //44
	{"Hosea",           "HO", 28},    //45
	{"Isaiah",          "IS", 23},    //46
	{"James",           "JA", 83},    //47
	{"Jeremiah",        "JE", 24},    //48
	{"Job",             "JOB", 18},   //49
	{"Joel",            "JOE", 29},   //50
	{"John",            "JOH", 67},   //51
	{"Jonah",           "JON", 32},   //52
	{"Joshua",          "JOS", 6},    //53
	{"Jude",            "JUDE", 89},  //54
	{"Judges",          "JUDG", 7},   //55
	{"Judith",          "JUDI", 41},  //56
	{"Lamentations",    "LA", 25},    //57
	{"Letter of Jeremiah",Abbr:"LET", 46},  //58
	{"Leviticus",       "LEV", 3},    //59
	{"Luke",            "LK", 66},    //60
	{"Leviticus",       "LV", 3},     //61
	{"Luke",            "LU", 66},    //62
	{"Malachi",         "MAL", 39},   //63
	{"Prayer of Manasseh",Abbr:"MAN", 53},  //64
	{"Mark",            "MAR", 65},   //65
	{"Matthew",         "MAT", 64},   //66
	{"Micah",           "MI", 33},    //67
	{"Nahum",           "NA", 34},    //68
	{"Nehemiah",        "NE", 16},    //69
	{"Numbers",         "NU", 4},     //70
	{"Obadiah",         "OB", 31},    //71
	{"Psalm 151",       "P1", 54},    //72
	{"Philemon",        "PHILE", 81}, //73
	{"Philippians",     "PHILI", 74}, //74
	{"Philemon",        "PHM", 81},   //75
	{"Philippians",     "PHP", 74},   //76
	{"Proverbs",        "PR", 20},    //77
	{"Psalms",          "PS", 19},    //78
	{"Revelation",      "RE", 90},    //79
	{"Romans",          "RM", 69},    //80
	{"Romans",          "RO", 69},    //81
	{"Ruth",            "RU", 8},     //82
	{"Sirach",          "SI", 44},    //83
	{"Song of Solomon", "SOL", 22},   //84
	{"Song of Solomon", "SON", 22},   //85
	{"Song of Solomon", "SS", 22},    //86
	{"Susanna",         "SU", 48},    //87
	{"Titus",           "TI", 80},    //88
	{"Tobit",           "TO", 40},    //89
	{"Wisdom",          "WI", 43},    //90
	{"Zechariah",       "ZEC", 38},   //91
	{"Zephaniah",       "ZEP", 36}    //92
	},

string BookFileName[91] = {
	"","Genesis","Exodus","Lev","Num","Deut","Joshua","Judges", // 0 - 7
	"Ruth","1Sam","2Sam","1Kings","2Kings","1Chron","2Chron",   // 8 - 14
	"Ezra","Nehemiah","Esther","Job","Psalms","Proverbs",       // 15-20
	"Eccl","Song","Isaiah","Jeremiah","Lament","Ezekiel",       // 21-26
	"Daniel","Hosea","Joel","Amos","Obadiah","Jonah","Micah",   // 27-33
	"Nahum","Habakkuk","Zeph","Haggai","Zech","Malachi",        // 34-39
	"Tobit","Judith","Esther","Wisdom","Sirach","Baruch",       // 40-45
	"Let","Azar","Susanna","Bel","1Mac","2Mac","1Esdras",       // 46-52
	"Man","P1","3Mac","2Esdras","4Mac","","","","","","",       // 53-63
	"Matthew","Mark","Luke","John","Acts","Romans","1Cor",      // 64-70
	"2Cor","Gal","Eph","Philip","Col","1Thes","2Thes","1Tim",   // 71-78
	"2Tim","Titus","Philemon","Hebrews","James","1Peter",       // 79-84
	"2Peter","1John","2John","3John","Jude","Rev"};             // 85-90

class TReadGBF {
private:
	FILE *fp;
	string FName, TokenLine;
	int TokenPos;
	bool fFileIsOpen, fParagraphEnd, fInTitle, fInPsalmBookTitle, fInHebrewTitle, fInSectionTitle;

public:
	string sBook, sChapter, sVerse, sMode;
	string sContext; // Last text type (header, body, or tail)
	string sTitle;   // Title of this book of the Bible
	string sPsalmBookTitle; // Title of this Psalm book
	string sHebrewTitle;    // Psalm Hebrew title
	string sSectionTitle;   // Section headings
	string sDate;
	string sFontName;
	int iTotalWords;
	char chJustification, chDirection;
	bool fIndent, fPoetry;
	int CharAttribs;
	char bBk, bChap, bVs, bWd;

	bool Init(const string sFileName);
	void Done();
	string GetToken(TToken &TokenKind);
    end;

class TWriteGBF {
	private:
	F: TextFile;
	FName, LineOut: string;
	fFileIsOpen: boolean;
	bBk, bChap, bVs, bWd: byte;

	public

	function Init(const sFileName: string): boolean;
	function Done: boolean;
	procedure Out(const s: string);
    end;

function isletter(const ch: char): boolean;
function isinword(const ch: char): boolean;
function IsDigit(const ch: char): Boolean;
function IsUpper(const ch: char): Boolean;
function ConformCase(const sPat, sSrc: string): string;
function BookNameToNumber(const sBookName: string): byte;

implementation

function isletter(const ch: char): boolean;
begin
    case ch of
	'A'..'Z': isletter := true;
	'a'..'z': isletter := true;
	else
	isletter := false;
    end;
end;

function isinword(const ch: char): boolean;
begin
    case ch of
	'-': isinword := true;
	'A'..'Z': isinword := true;
	'a'..'z': isinword := true;
	else
	isinword := false;
    end;
end;

function IsUpper(const ch: char): Boolean;
begin
    case ch of
	'A'..'Z': IsUpper := true;
	else
	    IsUpper := false;
    end;
end;

function IsDigit(const ch: char): Boolean;
begin
    case ch of
	'0'..'9': IsDigit := true;
	else
	    IsDigit := false;
    end;
end;


function MatchAbbrev(const sName, sAbbrev: string): boolean;
var i: integer;
begin
    if Length(sName) < Length(sAbbrev) then
	Result := false
    else
	Result := true;
    i := 1;
    while (i <= Length(sAbbrev)) and Result do
	begin
	if UpCase(sName[i]) <> sAbbrev[i] then
	    Result := false;
	inc(i);
	end;
end;

function BookNameToNumber(const sBookName: string): byte;
var i: integer;
begin
    Result := 0;
    try
    if IsDigit(sBookName[Length(sBookName)]) and IsDigit(sBookName[1]) then
	Result := StrToInt(sBookName);
    except
    Result := 0;
    end;
    i := 0;
    while (Result = 0) and (i <= 115) do  // Yuk! Linear search.
	begin
	if MatchAbbrev(sBookName,BookAbbr[i].Abbr) then
	    begin
	    Result := BookAbbr[i].Num;
	    end;
	inc(i);
	end;
end;

function BookNumberToName(const bBookNum: byte): string;
begin
    if bBookNum <= 115 then
	Result := BookAbbr[bBookNum].Name
    else
	Result := '';
end;

function ConformCase(const sPat, sSrc: string): string;
var i: integer;
begin
    Result := sSrc;
    if (Length(sPat) > 0) and (Length(sSrc) > 0) then
	begin
	Result := LowerCase(sSrc);
	if IsUpper(sPat[1]) then
	    Result[1] := UpCase(Result[1]);
	if (Length(sPat) > 1) and (Length(sSrc) > 1) then
	    begin
	    if IsUpper(sPat[2]) then
		begin
		for i := 2 to Length(Result) do
		    Result[i] := UpCase(Result[i]);
		end;
	    end;
	end;
end;

function TReadGBF.Init(const sFileName: string): boolean;
var s: string;
    tok: TToken;
begin
    try
    fParagraphEnd := false;
    bBk := 0;
    bChap := 0;
    bVs := 0;
    bWd := 0;
    iTotalWords := 0;
    FName := sFileName;
    Assign(F, FName);
    reset(F);
    readln(F, TokenLine);
    TokenPos := 1;
    fFileIsOpen := true;
    repeat
	s := GetToken(tok)
    until (tok = tokEOF) or ((tok = tokHeader) and (s[3] = '0'));
    Init := true;
    except
    Init := false;
    fFileIsOpen := false;
    end
end;

procedure TReadGBF.Done;
begin
    if fFileIsOpen then
	begin
	closefile(F);
	fFileIsOpen := false;
	end;
end;

function TReadGBF.GetToken(var TokenKind: TToken): string;
var m: integer;
begin
    Result := '';
    TokenKind := tokNull;
    if TokenPos = 0 then
	begin
	if (not fFileIsOpen) or EOF(F) then
	    TokenKind := tokEOF
	else
	    begin
	    ReadLn(F,TokenLine);
	    TokenPos := 1;
	    end;
	end;
    if TokenKind <> tokEOF then
	begin
	m := Length(TokenLine);
	if TokenPos > m then
	    begin
	    TokenKind := tokSpace;
	    if fParagraphEnd then
		fParagraphEnd := false
	    else
		Result := ' ';
	    TokenPos := 0;
	    end
	else
	    begin
	    if (TokenLine[TokenPos] = '<') then
		begin
		fParagraphEnd := false;
		repeat
		    Result := Result + TokenLine[TokenPos];
		    inc(TokenPos);
		until (TokenLine[TokenPos] = '>') or (TokenPos > m);
		Result := Result + '>';
		inc(TokenPos);
		case result[2] of
		    'B': begin // File body text type
			 TokenKind := tokContent;
			 sContext := Result;
			 end;
		    'C': begin // Special characters
			 TokenKind := tokControl;
			 if (Result[3] = 'M') or (Result[3] = 'L') then
			     fParagraphEnd := true;
			 end;
		    'D': begin // Direction
			 TokenKind := tokControl;
			 chDirection := Result[3];
			 end;
		    'H': begin
			 TokenKind := tokHeader;
			 sContext := Result;
			 end;
		    'F': begin // Font attributes
			 TokenKind := tokFont;
			 case Result[3] of
			     'B': CharAttribs := CharAttribs + [caBold];
			     'C': CharAttribs := CharAttribs + [caSmallCaps];
			     'I': CharAttribs := CharAttribs + [caItalic];
			     'N': sFontName := copy(Result,4,Length(Result)-4);
			     'O': CharAttribs := CharAttribs + [caOTQuote];
			     'R': CharAttribs := CharAttribs + [caRed];
			     'S': CharAttribs := CharAttribs + [caSuperscript];
			     'U': CharAttribs := CharAttribs + [caUnderline];
			     'V': CharAttribs := CharAttribs + [caSubscript];
			     'b': CharAttribs := CharAttribs - [caBold];
			     'c': CharAttribs := CharAttribs - [caSmallCaps];
			     'i': CharAttribs := CharAttribs - [caItalic];
			     'n': sFontName := '';
			     'o': CharAttribs := CharAttribs - [caOTQuote];
			     'r': CharAttribs := CharAttribs - [caRed];
			     's': CharAttribs := CharAttribs - [caSuperscript];
			     'u': CharAttribs := CharAttribs - [caUnderline];
			     'v': CharAttribs := CharAttribs - [caSubscript];

			 end;
			 end;
		    'J': begin // Justification
			 TokenKind := tokStyle;
			 chJustification := Result[3];
			 end;
		    'P': begin // Poetry/prose, indent
			 TokenKind := tokControl;
			 case Result[3] of
			     'I': fIndent := true;
			     'P': fPoetry := true;
			     'i': fIndent := false;
			     'p': fPoetry := false;
			 end;
			 end;
		    'R': begin // References and footnotes
			 TokenKind := tokControl;
			 end;
		    'S': begin // sync mark
			 TokenKind := TokSync;
			 case Result[3] of
			     'B': begin // Book
			          sBook := system.copy(Result, 4, length(Result)-4);
			          sPsalmBookTitle := '';
			          if sBook = '' then
			              begin
			              inc(bBk);
			              sBook := BookNumberToName(bBk);
			              end
			          else
			              bBk := BookNameToNumber(sBook);
			          sTitle := sBook;
			          end;
			     'C': begin //chapter
			          sChapter := system.copy(Result, 4, length(Result)-4);
			          if sChapter = '' then
			              begin
			              inc(bChap);
			              sChapter := IntToStr(bChap);
			              end
			          else
			              begin
			              try
			              bChap := StrToInt(sChapter);
			              except
			              showmessage('Non-numeric chapter: '+sBook+' '+sChapter);
			              end;
			              end;
			          sHebrewTitle := '';    
			          end;
			     'V': begin // Verse
			          bWd := 0;
			          sVerse := system.copy(Result, 4, length(Result)-4);
			          if sVerse = '' then
			              begin
			              inc(bVs);
			              sVerse := IntToStr(bVs);
			              end
			          else
			              begin
			              try
			              bVs := StrToInt(sVerse);
			              except
			              showmessage('Non-numeric verse: '+sBook+' '+sChapter+':'+sVerse);
			              end;
			              end;
			          end;
			     'D': begin // Date
			          sDate := system.copy(Result, 3, length(Result)-4);
			          end;
			 end;
			 end;
		    'T': begin // Titles
			 TokenKind := TokContent;
			 case Result[3] of
			     'B':
			         begin
			         sPsalmBookTitle := '';
			         fInPsalmBookTitle := true;
			         end;
			     'b': fInPsalmBookTitle := true;
			     'H':
			         begin
			         sHebrewTitle := '';
			         fInHebrewTitle := true;
			         end;
			     'h': fInHebrewTitle := false;
			     'S':
			         begin
			         sSectionTitle := '';
			         fInSectionTitle := true;
			         end;
			     's': fInSectionTitle := false;
			     'T':
			         begin
			         sTitle := '';
			         fInTitle := true;
			         end;
			     't': fInTitle := false;
			 end;
			 end;
		    'Z': begin // File tail
			 TokenKind := tokTail;
			 sContext := Result;
			 if Result[3] = 'Z' then
			     done;
			 end;
		    else
			 TokenKind := TokControl;

		end;
		end
	    else if isletter(TokenLine[TokenPos]) then
		begin {Word}
		fParagraphEnd := false;
		TokenKind := tokWord;
		repeat
		    Result := Result + TokenLine[TokenPos];
		    inc(TokenPos);
		until (TokenPos > m) or (not isinword(TokenLine[TokenPos]));
		inc(bWd);
		inc(iTotalWords);
		end
	    else if ((TokenLine[TokenPos] = ' ') or (TokenLine[TokenPos] = #9)) then
		begin
		fParagraphEnd := false;
		TokenKind := tokSpace;
		Result := Result + TokenLine[TokenPos];
		inc(TokenPos);
		end
	    else
		begin
		fParagraphEnd := false;
		TokenKind := tokChar;
		Result := Result + TokenLine[TokenPos];
		inc(TokenPos);
		end
	    end;
	end;
    if ((TokenKind = tokWord) or (TokenKind = tokSpace) or
       (TokenKind = tokChar)) then
	begin
	if fInTitle then
	    sTitle := sTitle + Result
	else if fInPsalmBookTitle then
	    sPsalmBookTitle := sPsalmBookTitle + Result
	else if fInHebrewTitle then
	    sHebrewTitle := sHebrewTitle + Result
	else if fInSectionTitle then
	    sSectionTitle := sSectionTitle + Result;
	end;
end;

function TWriteGBF.Init(const sFileName: string): boolean;
begin
    try
    bBk := 0;
    bChap := 0;
    bVs := 0;
    bWd := 0;
    LineOut := '';
    FName := sFileName;
    Assign(F, FName);
    filemode := 1;
    rewrite(F);
    fFileIsOpen := true;
    Init := true;
    except
    Init := false;
    fFileIsOpen := false;
    end
end;

function TWriteGBF.Done: boolean;
begin
    try
    if fFileIsOpen then
	begin
	if LineOut <> '' then
	    begin
	    WriteLn(F, LineOut);
	    LineOut := '';
	    end;
	CloseFile(F);
	end;
    Done := true;
    except
    Done := false;
    end;
end;

procedure TWriteGBF.Out(const s: string);
var sPrint, sSave, sBook, sChapter, sVerse: string;
    i: integer;
    b: byte;
begin
    if (Length(s) > 0) and IsLetter(s[1]) then
	begin
	inc(bWd);
	LineOut := LineOut + s;
	end
    else if Length(s) > 3 then
	begin
	if (s[1] = '<') and (s[2] = 'S') then
	    begin
	    case s[3] of
		'B': begin // Book
		     sBook := system.copy(s, 4, length(s)-4);
		     if sBook = '' then
			 begin
			 inc(bBk);
			 LineOut := LineOut + s;
			 end
		     else
			 begin
			 b := bBk;
			 bBk := BookNameToNumber(sBook);
			 if b <> bBk then
			     LineOut := LineOut + s;
			 end;
		     end;
		'C': begin //chapter
		     sChapter := system.copy(s, 4, length(s)-4);
		     if sChapter = '' then
			 begin
			 inc(bChap);
			 LineOut := LineOut + s;
			 end
		     else
			 begin
			 try
//			 b := bChap;
			 bChap := StrToInt(sChapter);
//			 if b <> bChap then
			     LineOut := LineOut + s;
			 except
			 showmessage('Non-numeric chapter: '+sBook+' '+sChapter);
			 end;
			 end;
		     end;
		'V': begin // Verse
		     bWd := 0;
		     sVerse := system.copy(s, 4, length(s)-4);
		     if sVerse = '' then
			 begin
			 inc(bVs);
			 LineOut := LineOut + s;
			 end
		     else
			 begin
			 try
//			 b := bVs;
			 bVs := StrToInt(sVerse);
//			 if b <> bVs then
			     LineOut := LineOut + s;
			 except
			 showmessage('Non-numeric verse: '+sBook+' '+sChapter+':'+sVerse);
			 end;
			 end;
		     end;
		else
		    LineOut := LineOut + s;
		end
	    end
	else
	    LineOut := LineOut + s; // Not a sync mark
	end
    else // other token, space, or punctuation
	LineOut := LineOut + s; // Length <= 3
    if ((s = '<CM>') or (s = '<CL>')) then
	begin
	if (Length(LineOut) > 78) then
	    begin
	    i := 78;
	    while (i > 0) and (LineOut[i] <> ' ') do
		dec(i);
	    if i < 1 then
		begin
		WriteLn(F,LineOut);
		LineOut := '';
		end
	    else
		begin
		sPrint := system.copy(LineOut,1,i-1);
		sSave := system.copy(LineOut,i+1,Length(LineOut)-i);
		WriteLn(F,sPrint);
		WriteLn(F, sSave);
		LineOut := '';
		end
	    end
	else
	    begin
	    WriteLn(F, LineOut);
	    LineOut := '';
	    end
	end
    else if (Length(LineOut) > 78) then
	begin
	i := 78;
	while (i > 0) and (LineOut[i] <> ' ') do
	    dec(i);
	if i < 1 then
	    begin
	    WriteLn(F,LineOut);
	    LineOut := '';
	    end
	else
	    begin
	    sPrint := system.copy(LineOut,1,i-1);
	    sSave := system.copy(LineOut,i+1,Length(LineOut)-i);
	    WriteLn(F,sPrint);
	    LineOut := sSave;
	    end
	end
end;

end.
