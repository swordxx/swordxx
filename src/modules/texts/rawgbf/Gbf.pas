type
    TToken = (tokNull, tokEOF, tokHeader, tokContent, tokTail, tokStyle,
              tokWord, tokSpace, tokSync, tokControl, tokChar, tokFont);
    TCharacterAttribute = (caBold, caSmallCaps, caItalic, caOTQuote, caRed,
                            caSuperscript, caUnderline, caSubscript);
    TCharAttribs = set of TCharacterAttribute;


    TBookNameRec = record
        Name,
        Abbr: string;
        Num: byte
    end;

    TBookAbbr = array[0..115] of TBookNameRec;

const
    BookAbbr: TBookAbbr = (
        (Name: '1 Chronicles';    Abbr: '1CH'; Num: 13),   {//0}
        (Name: '1 Corinthians';   Abbr: '1CO'; Num: 70),   {//1}
        (Name: '1 Esdras';        Abbr: '1E'; Num: 52),    {//2}
        (Name: '1 John';          Abbr: '1J'; Num: 86),    {//3}
        (Name: '1 Kings';         Abbr: '1K'; Num: 11),    {//4}
        (Name: '1 Maccabees';     Abbr: '1M'; Num: 50),    {//5}
        (Name: '1 Peter';         Abbr: '1P'; Num: 84),    {//6}
        (Name: '1 Samuel';        Abbr: '1S'; Num: 9),     {//7}
        (Name: '1 Thessalonians'; Abbr: '1TH'; Num: 76),   {//8}
        (Name: '1 Timothy';       Abbr: '1TI'; Num: 78),   {//9}
        (Name: '2 Chronicles';    Abbr: '2CH'; Num: 14),   {//10}
        (Name: '2 Corinthians';   Abbr: '2CO'; Num: 71),   {//11}
        (Name: '2 Esdras';        Abbr: '2E'; Num: 56),    {//12}
        (Name: '2 John';          Abbr: '2J'; Num: 87),    {//13}
        (Name: '2 Kings';         Abbr: '2K'; Num: 12),    {//14}
        (Name: '2 Maccabees';     Abbr: '2M'; Num: 51),    {//15}
        (Name: '2 Peter';         Abbr: '2P'; Num: 85),    {//16}
        (Name: '2 Samuel';        Abbr: '2S'; Num: 10),    {//17}
        (Name: '2 Thessalonians'; Abbr: '2TH'; Num: 77),   {//18}
        (Name: '2 Timothy';       Abbr: '2TI'; Num: 79),   {//19}
        (Name: '3 John';          Abbr: '3J'; Num: 88),    {//20}
        (Name: '3 Maccabees';     Abbr: '3M'; Num: 55),    {//21}
        (Name: '4 Maccabees';     Abbr: '4M'; Num: 57),    {//22}
        (Name: '1 Chronicles';    Abbr: '1 CH'; Num: 13),   {//0}
        (Name: '1 Corinthians';   Abbr: '1 CO'; Num: 70),   {//1}
        (Name: '1 Esdras';        Abbr: '1 E'; Num: 52),    {//2}
        (Name: '1 John';          Abbr: '1 J'; Num: 86),    {//3}
        (Name: '1 Kings';         Abbr: '1 K'; Num: 11),    {//4}
        (Name: '1 Maccabees';     Abbr: '1 M'; Num: 50),    {//5}
        (Name: '1 Peter';         Abbr: '1 P'; Num: 84),    {//6}
        (Name: '1 Samuel';        Abbr: '1 S'; Num: 9),     {//7}
        (Name: '1 Thessalonians'; Abbr: '1 TH'; Num: 76),   {//8}
        (Name: '1 Timothy';       Abbr: '1 TI'; Num: 78),   {//9}
        (Name: '2 Chronicles';    Abbr: '2 CH'; Num: 14),   {//10}
        (Name: '2 Corinthians';   Abbr: '2 CO'; Num: 71),   {//11}
        (Name: '2 Esdras';        Abbr: '2 E'; Num: 56),    {//12}
        (Name: '2 John';          Abbr: '2 J'; Num: 87),    {//13}
        (Name: '2 Kings';         Abbr: '2 K'; Num: 12),    {//14}
        (Name: '2 Maccabees';     Abbr: '2 M'; Num: 51),    {//15}
        (Name: '2 Peter';         Abbr: '2 P'; Num: 85),    {//16}
        (Name: '2 Samuel';        Abbr: '2 S'; Num: 10),    {//17}
        (Name: '2 Thessalonians'; Abbr: '2 TH'; Num: 77),   {//18}
        (Name: '2 Timothy';       Abbr: '2 TI'; Num: 79),   {//19}
        (Name: '3 John';          Abbr: '3 J'; Num: 88),    {//20}
        (Name: '3 Maccabees';     Abbr: '3 M'; Num: 55),    {//21}
        (Name: '4 Maccabees';     Abbr: '4 M'; Num: 57),    {//22}
        (Name: 'Acts';            Abbr: 'AC'; Num: 68),    {//23}
        (Name: 'Amos';            Abbr: 'AM'; Num: 30),    {//24}
        (Name: 'Prayer of Asariah and the Song of the Three Jews'; Abbr: 'AZ'; Num: 47),
        (Name: 'Baruch';          Abbr: 'BA'; Num: 45),    {//26}
        (Name: 'Bel and the Dragon';Abbr: 'BE'; Num: 49),  {//27}
        (Name: 'Colossians';      Abbr: 'CO'; Num: 75),    {//28}
        (Name: 'Daniel';          Abbr: 'DA'; Num: 27),    {//29}
        (Name: 'Deuteronomy';     Abbr: 'DE'; Num: 5),     {//30}
        (Name: 'Deuteronomy';     Abbr: 'DT'; Num: 5),     {//31}
        (Name: 'Ecclesiasties';   Abbr: 'EC'; Num: 21),    {//32}
        (Name: 'Esther';          Abbr: 'ES'; Num: 17),    {//33}
        (Name: 'Exodus';          Abbr: 'EX'; Num: 2),     {//34}
        (Name: 'Ezekiel';         Abbr: 'EZE'; Num: 26),   {//35}
        (Name: 'Ezra';            Abbr: 'EZR'; Num: 15),   {//36}
        (Name: 'Galatians';       Abbr: 'GA'; Num: 72),    {//37}
        (Name: 'Genesis';         Abbr: 'GE'; Num: 1),     {//38}
        (Name: 'Genesis';         Abbr: 'GN'; Num: 1),     {//39}
        (Name: 'Ephesians';       Abbr: 'EP'; Num: 73),    {//40}
        (Name: 'Esther (Greek)';  Abbr: 'GR'; Num: 42),    {//41}
        (Name: 'Habakkuk';        Abbr: 'HAB'; Num: 35),   {//42}
        (Name: 'Haggai';          Abbr: 'HAG'; Num: 37),   {//43}
        (Name: 'Hebrews';         Abbr: 'HE'; Num: 82),    {//44}
        (Name: 'Hosea';           Abbr: 'HO'; Num: 28),    {//45}
        (Name: 'Isaiah';          Abbr: 'IS'; Num: 23),    {//46}
        (Name: 'James';           Abbr: 'JA'; Num: 83),    {//47}
        (Name: 'Jeremiah';        Abbr: 'JE'; Num: 24),    {//48}
        (Name: 'Job';             Abbr: 'JOB'; Num: 18),   {//49}
        (Name: 'Joel';            Abbr: 'JOE'; Num: 29),   {//50}
        (Name: 'John';            Abbr: 'JOH'; Num: 67),   {//51}
        (Name: 'Jonah';           Abbr: 'JON'; Num: 32),   {//52}
        (Name: 'Joshua';          Abbr: 'JOS'; Num: 6),    {//53}
        (Name: 'Jude';            Abbr: 'JUDE'; Num: 89),  {//54}
        (Name: 'Judges';          Abbr: 'JUDG'; Num: 7),   {//55}
        (Name: 'Judith';          Abbr: 'JUDI'; Num: 41),  {//56}
        (Name: 'Lamentations';    Abbr: 'LA'; Num: 25),    {//57}
        (Name: 'Letter of Jeremiah';Abbr:'LET'; Num: 46),  {//58}
        (Name: 'Leviticus';       Abbr: 'LEV'; Num: 3),    {//59}
        (Name: 'Luke';            Abbr: 'LK'; Num: 66),    {//60}
        (Name: 'Leviticus';       Abbr: 'LV'; Num: 3),     {//61}
        (Name: 'Luke';            Abbr: 'LU'; Num: 66),    {//62}
        (Name: 'Malachi';         Abbr: 'MAL'; Num: 39),   {//63}
        (Name: 'Prayer of Manasseh';Abbr:'MAN'; Num: 53),  {//64}
        (Name: 'Mark';            Abbr: 'MAR'; Num: 65),   {//65}
        (Name: 'Matthew';         Abbr: 'MAT'; Num: 64),   {//66}
        (Name: 'Micah';           Abbr: 'MI'; Num: 33),    {//67}
        (Name: 'Nahum';           Abbr: 'NA'; Num: 34),    {//68}
        (Name: 'Nehemiah';        Abbr: 'NE'; Num: 16),    {//69}
        (Name: 'Numbers';         Abbr: 'NU'; Num: 4),     {//70}
        (Name: 'Obadiah';         Abbr: 'OB'; Num: 31),    {//71}
        (Name: 'Psalm 151';       Abbr: 'P1'; Num: 54),    {//72}
        (Name: 'Philemon';        Abbr: 'PHILE'; Num: 81), {//73}
        (Name: 'Philippians';     Abbr: 'PHILI'; Num: 74), {//74}
        (Name: 'Philemon';        Abbr: 'PHM'; Num: 81),   {//75}
        (Name: 'Philippians';     Abbr: 'PHP'; Num: 74),   {//76}
        (Name: 'Proverbs';        Abbr: 'PR'; Num: 20),    {//77}
        (Name: 'Psalms';          Abbr: 'PS'; Num: 19),    {//78}
        (Name: 'Revelation';      Abbr: 'RE'; Num: 90),    {//79}
        (Name: 'Romans';          Abbr: 'RM'; Num: 69),    {//80}
        (Name: 'Romans';          Abbr: 'RO'; Num: 69),    {//81}
        (Name: 'Ruth';            Abbr: 'RU'; Num: 8),     {//82}
        (Name: 'Sirach';          Abbr: 'SI'; Num: 44),    {//83}
        (Name: 'Song of Solomon'; Abbr: 'SOL'; Num: 22),   {//84}
        (Name: 'Song of Solomon'; Abbr: 'SON'; Num: 22),   {//85}
        (Name: 'Song of Solomon'; Abbr: 'SS'; Num: 22),    {//86}
        (Name: 'Susanna';         Abbr: 'SU'; Num: 48),    {//87}
        (Name: 'Titus';           Abbr: 'TI'; Num: 80),    {//88}
        (Name: 'Tobit';           Abbr: 'TO'; Num: 40),    {//89}
        (Name: 'Wisdom';          Abbr: 'WI'; Num: 43),    {//90}
        (Name: 'Zechariah';       Abbr: 'ZEC'; Num: 38),   {//91}
        (Name: 'Zephaniah';       Abbr: 'ZEP'; Num: 36)    {//92}
        );

    BookFileName: array[0..90] of string = (
        '','Genesis','Exodus','Lev','Num','Deut','Joshua','Judges', {// 0 - 7}
        'Ruth','1Sam','2Sam','1Kings','2Kings','1Chron','2Chron',   {// 8 - 14}
        'Ezra','Nehemiah','Esther','Job','Psalms','Proverbs',       {// 15-20}
        'Eccl','Song','Isaiah','Jeremiah','Lament','Ezekiel',       {// 21-26}
        'Daniel','Hosea','Joel','Amos','Obadiah','Jonah','Micah',   {// 27-33}
        'Nahum','Habakkuk','Zeph','Haggai','Zech','Malachi',        {// 34-39}
        'Tobit','Judith','Esther','Wisdom','Sirach','Baruch',       {// 40-45}
        'Let','Azar','Susanna','Bel','1Mac','2Mac','1Esdras',       {// 46-52}
        'Man','P1','3Mac','2Esdras','4Mac','','','','','','',       {// 53-63}
        'Matthew','Mark','Luke','John','Acts','Romans','1Cor',      {// 64-70}
        '2Cor','Gal','Eph','Philip','Col','1Thes','2Thes','1Tim',   {// 71-78}
        '2Tim','Titus','Philemon','Hebrews','James','1Peter',       {// 79-84}
        '2Peter','1John','2John','3John','Jude','Rev');             {// 85-90}

type
    TReadGBF = record
{        private}
        F: File;
        FName, TokenLine: string;
        TokenPos: integer;
        fFileIsOpen, fParagraphEnd, fInTitle, fInPsalmBookTitle, fInHebrewTitle,
        fInSectionTitle: boolean;

{        public}
        sBook, sChapter, sVerse, sMode: string;
        sContext, {// Last text type (header, body, or tail)}
        sTitle,   {// Title of this book of the Bible}
        sPsalmBookTitle, {// Title of this Psalm book}
        sHebrewTitle,    {// Psalm Hebrew title}
        sSectionTitle,   {// Section headings}
        sDate,
        sFontName: string;
        iTotalWords: integer;
        chJustification,
        chDirection: char;
        fIndent, fPoetry: boolean;
        CharAttribs: TCharAttribs;
        bBk, bChap, bVs, bWd: byte;
{
        function Init({const}{sFileName: string): boolean;
        procedure Done;
        function GetToken(var TokenKind: TToken): string;
}
    end;

    TWriteGBF = record
{        private}
        F: File;
        FName, LineOut: string;
        fFileIsOpen: boolean;
        bBk, bChap, bVs, bWd: byte;

{        public}
{
        function Init({const}{sFileName: string): boolean;
        function Done: boolean;
        procedure Out({const}{s: string);
}
    end;

{implementation }

function isletter({const}ch: char): boolean;
begin
    case ch of
        'A'..'Z': isletter := true;
        'a'..'z': isletter := true;
        else
        isletter := false;
    end;
end;

function isinword({const}ch: char): boolean;
begin
    case ch of
        '-': isinword := true;
        'A'..'Z': isinword := true;
        'a'..'z': isinword := true;
        else
        isinword := false;
    end;
end;

function IsUpper({const}ch: char): Boolean;
begin
    case ch of
        'A'..'Z': IsUpper := true;
        else
            IsUpper := false;
    end;
end;

function IsDigit({const}ch: char): Boolean;
begin
    case ch of
        '0'..'9': IsDigit := true;
        else
            IsDigit := false;
    end;
end;


function MatchAbbrev({const}sName, sAbbrev: string): boolean;
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

function BookNameToNumber({const}sBookName: string): byte;
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
    while (Result = 0) and (i <= 115) do  {// Yuk! Linear search.}
        begin
        if MatchAbbrev(sBookName,BookAbbr[i].Abbr) then
            begin
            Result := BookAbbr[i].Num;
            end;
        inc(i);
        end;
end;

function BookNumberToName({const}bBookNum: byte): string;
begin
    if bBookNum <= 115 then
        Result := BookAbbr[bBookNum].Name
    else
        Result := '';
end;

function ConformCase({const}sPat, sSrc: string): string;
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

function TReadGBF.Init({const}sFileName: string): boolean;
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
                    'B': begin {// File body text type}
                         TokenKind := tokContent;
                         sContext := Result;
                         end;
                    'C': begin {// Special characters}
                         TokenKind := tokControl;
                         if (Result[3] = 'M') or (Result[3] = 'L') then
                             fParagraphEnd := true;
                         end;
                    'D': begin {// Direction}
                         TokenKind := tokControl;
                         chDirection := Result[3];
                         end;
                    'H': begin
                         TokenKind := tokHeader;
                         sContext := Result;
                         end;
                    'F': begin {// Font attributes}
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
                    'J': begin {// Justification}
                         TokenKind := tokStyle;
                         chJustification := Result[3];
                         end;
                    'P': begin {// Poetry/prose, indent}
                         TokenKind := tokControl;
                         case Result[3] of
                             'I': fIndent := true;
                             'P': fPoetry := true;
                             'i': fIndent := false;
                             'p': fPoetry := false;
                         end;
                         end;
                    'R': begin {// References and footnotes}
                         TokenKind := tokControl;
                         end;
                    'S': begin {// sync mark}
                         TokenKind := TokSync;
                         case Result[3] of
                             'B': begin {// Book}
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
                             'C': begin {//chapter}
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
                             'V': begin {// Verse}
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
                             'D': begin {// Date}
                                  sDate := system.copy(Result, 3, length(Result)-4);
                                  end;
                         end;
                         end;
                    'T': begin {// Titles}
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
                    'Z': begin {// File tail}
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

function TWriteGBF.Init({const}sFileName: string): boolean;
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

procedure TWriteGBF.Out({const}s: string);
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
                'B': begin {// Book}
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
                'C': begin {//chapter}
                     sChapter := system.copy(s, 4, length(s)-4);
                     if sChapter = '' then
                         begin
                         inc(bChap);
                         LineOut := LineOut + s;
                         end
                     else
                         begin
                         try
{//                         b := bChap;}
                         bChap := StrToInt(sChapter);
{//                         if b <> bChap then}
                             LineOut := LineOut + s;
                         except
                         showmessage('Non-numeric chapter: '+sBook+' '+sChapter);
                         end;
                         end;
                     end;
                'V': begin {// Verse}
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
{//                         b := bVs;}
                         bVs := StrToInt(sVerse);
{//                         if b <> bVs then}
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
            LineOut := LineOut + s; {// Not a sync mark}
        end
    else {// other token, space, or punctuation}
        LineOut := LineOut + s; {// Length <= 3}
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
