unit GBFMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Buttons, StdCtrls, ExtCtrls, GBF;

const
  sTitlePar = '\pard\plain \s1\fi432\sb240\sa60\keepn\widctlpar \b\f5\fs28\kerning28 ';
  sNormalPar = '\pard\plain \fi432\widctlpar \f4 ';
  sNormalQuotePar = '\pard\plain \s20\fi432\li432\widctlpar \f4 ';
  sPoetryPar = '\pard\plain \s18\fi-432\li432\widctlpar \f4 ';
  sPoetryQuotePar = '\pard\plain \s21\fi-432\li864\widctlpar \f4 ';
  sHebrewTitlePar = '\pard\plain \s16\fi432\keep\keepn\widctlpar \f4\fs20 ';
  sSelahPar = '\pard\plain \s19\qr\widctlpar \f4 ';
  ANSI2OEM: array[0..255] of char =
      (  #0,  #1,  #2,  #3,  #4,  #5,  #6,  #7,
         #8,  #9, #10, #11, #12, #13, #14, #15,
        #16, #17, #18, #19, #20, #21, #22, #23,
        #24, #25, #26, #27, #28, #29, #30, #31,
        #32, #33, #34, #35, #36, #37, #38, #39,
        #40, #41, #42, #43, #44, #45, #46, #47,
        #48, #49, #50, #51, #52, #53, #54, #55,
        #56, #57, #58, #59, #60, #61, #62, #63,
        #64, #65, #66, #67, #68, #69, #70, #71,
        #72, #73, #74, #75, #76, #77, #78, #79,
        #80, #81, #82, #83, #84, #85, #86, #87,
        #88, #89, #90, #91, #92, #93, #94, #95,
        #96, #97, #98, #99,#100,#101,#102,#103,
       #104,#105,#106,#107,#108,#109,#110,#111,
       #112,#113,#114,#115,#116,#117,#118,#119,
       #120,#121,#122,#123,#124,#125,#126,#127,
       #128,#129, ',', 'a', '"',#133,#197,#216,
        '^', '%', 'S', '<',#140,#141,#142,#143,
       #144, #96, #97, '"', '"',#249,#150,#151,
        '~',#153, 's', '>',#156,#157,#158, 'Y',
        ' ',#173,#155,#156,#232,#157,#124, #21,
       #168,#169, 'a',#174,#170, '-',#174, '_',
       #167,#241,#253, '3', #39,#230, #20,#254,
        ',', '1', 'o',#175,#172,#171,#190,#168,
        'A', 'A', 'A', 'A',#142,#143,#198,#128,
        'E',#144, 'E',#142, 'I', 'I', 'I', 'I',
        'D',#165, 'O', 'O', 'O', 'O',#153, 'x',
        '0', 'U', 'U', 'U',#154, 'Y', 'b',#225,
       #133,#130,#131, 'a',#132,#134,#230,#135,
       #138,#130,#136,#137,#141,#161,#140,#139,
       #148,#164,#149,#162,#147, 'o',#148,#246,
        'o',#151,#163,#150,#129, 'y', 'b',#152);

type
  TGBFConverterMainForm = class(TForm)
    SourceEdit: TEdit;
    Label1: TLabel;
    BrowseButton: TButton;
    SaveDialog1: TSaveDialog;
    OpenDialog1: TOpenDialog;
    DestEdit: TEdit;
    Label2: TLabel;
    BrowseDestButton: TButton;
    FormatRadioGroup: TRadioGroup;
    GoBitBtn: TBitBtn;
    CloseBitBtn: TBitBtn;
    Timer1: TTimer;
    VerseLabel: TLabel;
    ApocryphaCheckBox: TCheckBox;
    WdLabel: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    WEBDraftCheckBox: TCheckBox;
    QuickButton: TButton;
    procedure CloseBitBtnClick(Sender: TObject);
    procedure GoBitBtnClick(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure FormatRadioGroupClick(Sender: TObject);
    procedure QuickConversion;
    procedure DoConversion;
    procedure QuickButtonClick(Sender: TObject);
    procedure FormActivate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  GBFConverterMainForm: TGBFConverterMainForm;

implementation

{$R *.DFM}

var InFile: TReadGBF;
    OutGBF: TWriteGBF;
    OutFile: TextFile;

function ANSIToOEM(s: string): string;
var i, j: integer;
begin
    Result := s;
    j := 1;
    for i := 1 to length(s) do
        begin
        case s[i] of
                #133:
                    begin
                    Result[j] := '.';
                    inc(j);
                    Insert('..', Result, j);
                    inc(j);
                    end;
                #140:
                    begin
                    Result[j] := 'O';
                    inc(j);
                    Insert('E', Result, j);
                    end;
                #150:
                    begin
                    Result[j] := '-';
                    inc(j);
                    Insert('-', Result, j);
                    end;
                #151:
                    begin
                    Result[j] := '-';
                    inc(j);
                    Insert('-', Result, j);
                    end;
                #153:
                    begin
                    Result[j] := '(';
                    inc(j);
                    Insert('TM)', Result, j);
                    inc(j,2);
                    end;
                #156:
                    begin
                    Result[j] := 'o';
                    inc(j);
                    Insert('e', Result, j);
                    end;
                #169:
                    begin
                    Result[j] := '(';
                    inc(j);
                    Insert('C)',Result, j);
                    inc(j);
                    end;
                #174:
                    begin
                    Result[j] := '(';
                    inc(j);
                    Insert('R)',Result, j);
                    inc(j);
                    end;
                #198:
                    begin
                    Result[j] := 'A';
                    inc(j);
                    Insert('E', Result, j);
                    end;
                #230:
                    begin
                    Result[j] := 'a';
                    inc(j);
                    Insert('e', Result, j);
                    end;
        else
            Result[j] := ANSI2OEM[ord(s[i])];
        end;
        inc(j);
        end;
end;

procedure TGBFConverterMainForm.CloseBitBtnClick(Sender: TObject);
begin
    Close;
end;

procedure TGBFConverterMainForm.DoConversion;
var LastBook, wd, ParagraphAttributes, s, sLine, sPrint, sSave,
    OutFileName: string;
    LinePos, i, iFileNumber: integer;
    tok: TToken;
    fInclude, fProse, fSkip, fHTMLisOpen, fRed, fASCIIisOpen: boolean;
    bLastBook, bChap: byte;

    procedure CheckEOL;
    begin
        if Length(sLine) > 65 then
            begin
            i := 65;
            while (i > 0) and (sLine[i] <> ' ') do
                dec(i);
            if i < 1 then
                begin
                if fASCIIisOpen then WriteLn(OutFile,sLine);
                if fProse then
                    sLine := ''
                else
                    sLine := '      ';
                end
            else
                begin
                sPrint := system.copy(sLine,1,i-1);
                if fProse then
                    sSave := system.copy(sLine,i+1,Length(sLine)-i)
                else
                    sSave := '      '+system.copy(sLine,i+1,Length(sLine)-i);
                if fASCIIisOpen then WriteLn(OutFile,sPrint);
                sLine := sSave;
                end
            end;
    end;

    procedure StartNewLine;
    begin
        if fInclude then
            begin
            CheckEol;
            if fASCIIisOpen then WriteLn(OutFile, sLine);
            sLine := '';
            end;
    end;

    procedure CloseHTML;
    begin
        if fHTMLisOpen then
            begin
            WriteLn(OutFile,sLine);
            sLine := '';
            WriteLn(OutFile,'</P>');
            WriteLn(OutFile,'<P></P><HR><A HREF="index.htm">[Index]</A> &nbsp;');
            WriteLn(OutFile,'<A HREF="http://www.ebible.net/bible/">[Home]</A>');
            WriteLn(OutFile,'</BODY></HTML>');
            CloseFile(OutFile);
            fHTMLisOpen := false;
            end;
    end;

    procedure CloseASCII;
    begin
        if fASCIIisOpen then
            begin
            WriteLn(OutFile,sLine);
            sLine := '';
            WriteLn(OutFile);
            if WEBDraftCheckBox.Checked then
                begin
                WriteLn(OutFile,'______________________________________________________________');
                WriteLn(OutFile);
                WriteLn(OutFile,'The above is from the public domain World English Bible (WEB).');
                WriteLn(OutFile,'See http://www.ebible.org/bible/WEB for more about this Bible.');
                WriteLn(OutFile,'Please report typos to mpj@ebible.org.');
                end;
            CloseFile(OutFile);
            fASCIIisOpen := false;
            end;
    end;

    procedure OpenHTML;
    begin
        if fHTMLisOpen then CloseHTML;
        sLine := '';
        OutFileName := ExtractFilePath(DestEdit.Text)+BookFileName[InFile.bBk]+'.htm';
        AssignFile(OutFile,OutFileName);
        Rewrite(OutFile);
        WriteLn(OutFile,'<HTML>');
        WriteLn(OutFile,'<HEAD>');
        WriteLn(OutFile,'<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=windows-1252">');
        WriteLn(OutFile,'<META http-equiv="PICS-Label" content=''(PICS-1.1 "http://www.rsac.org/ratingsv01.html"');
        WriteLn(OutFile,' l gen true comment "RSACi North America Server" by "mpj@csn.net" for "http://www.csn.net/~mpj"');
        WriteLn(OutFile,' on "1996.08.29T12:42-0500" r (n 0 s 0 v 0 l 0))''>');
        WriteLn(OutFile,'<META NAME="description" CONTENT="'+InFile.sTitle+' from the World English Bible -- a Public Domain Modern English translation of the Holy Bible.">');
        WriteLn(OutFile,'<META NAME="keywords" CONTENT="'+BookFileName[InFile.bBk]+', '+InFile.sTitle+', Bible, Christian, Holy Bible, Bible search, WEB, World English Bible, Scriptures, Scripture, Bibles, Gospel, Gospels, bible">');
        WriteLn(OutFile,'<TITLE>'+InFile.sTitle+'</TITLE>');
        WriteLn(OutFile,'<LINK REL=Home HREF="http://www.ebible.org/bible">');
        WriteLn(OutFile,'<LINK REL=Glossary HREF="glossary.htm">');
        WriteLn(OutFile,'</HEAD>');
        WriteLn(OutFile,'<BODY BGCOLOR="#80ffff">');
        WriteLn(OutFile,'<H1>');
        WriteLn(OutFile,InFile.sTitle);
        WriteLn(OutFile,'</H1><P>');
        fHTMLisOpen := true;
    end;

    procedure OpenASCII;
    begin
        if fASCIIisOpen then CloseASCII;
        if fProse then
            sLine := '   '
        else
            sLine := '';
        OutFileName := ExtractFilePath(DestEdit.Text)+BookFileName[InFile.bBk]+'.txt';
        AssignFile(OutFile,OutFileName);
        Rewrite(OutFile);
        WriteLn(OutFile);
        WriteLn(OutFile,InFile.sTitle);
        WriteLn(OutFile);
        fASCIIisOpen := true;
    end;

    procedure OpenNTChapter;
    var s: string;
    begin
        if InFile.bBk >= 64 then
            begin
            if fASCIIisOpen then CloseASCII;
            inc(iFileNumber);
            s := IntToStr(iFileNumber);
            if Length(s) < 3 then s := '0'+s;
            if Length(s) < 3 then s := '0'+s;
            OutFileName := ExtractFilePath(DestEdit.Text)+'n'+s+'.txt';
            AssignFile(OutFile,OutFileName);
            Rewrite(OutFile);
            WriteLn(OutFile,'Subject: '+BookFileName[InFile.bBk]+' '+InFile.sChapter+', World English Bible');
            if iFileNumber = 260 then
                WriteLn(OutFile,'X-Reset: 1');
            WriteLn(OutFile);
            WriteLn(OutFile);
            WriteLn(OutFile,InFile.sTitle+', Chapter '+InFile.sChapter);
            WriteLn(OutFile);
            fASCIIisOpen := true;
            if fProse then
                sLine := '   '
            else
                sLine := '';
            end
        else
            begin
            inc(bChap);
            if (bLastBook <> Infile.bBk) or ((bChap mod 3) = 1) then
                begin
                if (bLastBook <> Infile.bBk) then
                    begin
                    bLastBook := Infile.bBk;
                    bChap := 1;
                    end;
                if fASCIIisOpen then CloseASCII;
                inc(iFileNumber);
                s := IntToStr(iFileNumber);
                if Length(s) < 3 then s := '0'+s;
                if Length(s) < 3 then s := '0'+s;
                OutFileName := ExtractFilePath(DestEdit.Text)+s+'.txt';
                AssignFile(OutFile,OutFileName);
                Rewrite(OutFile);
                WriteLn(OutFile,'Subject: '+BookFileName[InFile.bBk]+' '+InFile.sChapter+', World English Bible');
                if (Infile.bBk = 39) and (bChap = 4) then
                    WriteLn(OutFile,'X-Reset: 1');
                WriteLn(OutFile);
                WriteLn(OutFile);
                WriteLn(OutFile,InFile.sTitle+', starting at chapter '+InFile.sChapter);
                WriteLn(OutFile);
                fASCIIisOpen := true;
                if fProse then
                    sLine := '   '
                else
                    sLine := '';
                end;
            end;
    end;

    procedure CheckHTMLEOL;
    begin
        if Length(sLine) > 75 then
            begin
            i := 75;
            while (i > 0) and (sLine[i] <> ' ') do
                dec(i);
            if i < 1 then
                begin
                if fHTMLisOpen then WriteLn(OutFile,sLine);
                sLine := ''
                end
            else
                begin
                sPrint := system.copy(sLine,1,i-1);
                sSave := system.copy(sLine,i+1,Length(sLine)-i);
                if fHTMLisOpen then WriteLn(OutFile,sPrint);
                sLine := sSave;
                end
            end;
    end;

    procedure StartNewHTMLLine;
    begin
        if fInclude then
            begin
            CheckHTMLEOL;
            if fHTMLisOpen then WriteLn(OutFile, sLine+'</P>');
            sLine := '<P>';
            end;
    end;


begin
    QuickButton.Enabled := false;
    GoBitBtn.Enabled := false;
    fInclude := false;
    fSkip := false;
    fProse := true;
    fRed := false;
    LastBook := '';
    ParagraphAttributes := sNormalPar;
    try
    InFile := TReadGBF.Create;
    if InFile.Init(Trim(SourceEdit.Text)) then
        begin
        LinePos := 0;
        case FormatRadioGroup.ItemIndex of
            -1: showmessage('No destination format selected!');
             0: begin
               Label3.Caption := 'Converting to ASCII';
               AssignFile(OutFile, DestEdit.Text);
               FileMode := 1;
               Rewrite(OutFile);
               fASCIIisOpen := true;
               sLine := '';
               repeat
                   wd := ANSIToOEM(InFile.GetToken(tok));
                   Application.ProcessMessages;
                   case tok of
                       tokWord:
                           begin
                           if fInclude and (not fSkip) then
                               begin
                               sLine := sLine + wd;
                               CheckEOL;
                               end;
                           end;
                       tokSpace:
                           begin
                           if fInclude and (not fSkip) then
                               begin
                               sLine := sLine + wd;
                               CheckEOL;
                               end;
                           end;
                       tokSync:
                           begin
                           if fInclude and (length(wd) > 3) then
                               begin
                               if wd[3] = 'V' then
                                   begin
                                   sLine := sLine + '{' + InFile.sChapter+':'+
                                           InFile.sVerse+'} ';
                                   CheckEOL
                                   end
                               else if (wd[3] = 'C') and (InFile.bBk = 19) then
                                   begin
                                       StartNewLine;
                                       WriteLn(OutFile, 'Psalm '+InFile.sChapter);
                                       WriteLn(OutFile);
                                   end;
                               if wd[3] = 'B' then
                                   fProse := true;
                               end;
                           end;
                       tokContent:
                           begin
                           if wd = '<BO>' then
                               fInclude := true
                           else if wd = '<BN>' then
                               fInclude := true
                           else if wd = '<BO>' then
                               fInclude := ApocryphaCheckBox.Checked
                           end;
                       tokControl:
                           begin
                           if wd = '<CM>' then
                               begin
                               StartNewLine;
                               if fProse then
                                   begin
                                   WriteLn(OutFile);
                                   sLine := '   '
                                   end
                               end
                           else if wd = '<CL>' then
                               begin
                               StartNewLine;
                               sLine := '   ';
                               end
                           else if wd = '<Pp>' then
                               fProse := true
                           else if wd = '<PP>' then
                               fProse := false
                           else if wd = '<RF>' then
                               fSkip := true
                           else if wd = '<Rf>' then
                               fSkip := false
                           else if wd = '<RN>' then
                               fSkip := true
                           else if wd = '<Rn>' then
                               fSkip := false
                           else if wd = '<ZZ>' then
                               fInclude := false
                           end;
                       tokChar:
                           begin
                           if fInclude and (not fSkip) then
                               begin
                               sLine := sLine + wd;
                               CheckEOL;
                               end;
                           end;
                       tokFont:
                           begin
                           if wd = '<FI>' then
                               begin
                                           if fInclude then
                                               sLine := sLine + '[';
                               end
                           else if wd = '<Fi>' then
                               begin
                                           if fInclude then
                                               sLine := sLine + ']';
                               end
                           end;
                   end
               until tok = tokEOF;
               writeln(OutFile, sLine);
               CloseFile(OutFile);
               fASCIIisOpen := false;
               Label3.Caption := '';
               end;
             1: begin
               Label3.Caption := 'Converting to ASCII (one file/book)';
               FileMode := 1;
               fASCIIisOpen := false;
               sLine := '';
               repeat
                   Application.ProcessMessages;
                   wd := ANSIToOEM(InFile.GetToken(tok));
                   case tok of
                       tokEOF:
                           CloseASCII;
                       tokWord:
                           begin
                           if fInclude and (not fSkip) then
                               begin
                               sLine := sLine + wd;
                               CheckEOL;
                               end;
                           end;
                       tokSpace:
                           begin
                           if fInclude and (not fSkip) then
                               begin
                               sLine := sLine + wd;
                               CheckEOL;
                               end;
                           end;
                       tokSync:
                           begin
                           if fInclude and (length(wd) > 3) then
                               begin
                               if wd[3] = 'V' then
                                   begin
                                   sLine := sLine + '{' + InFile.sChapter+':'+
                                           InFile.sVerse+'} ';
                                   CheckEOL
                                   end
                               else if (wd[3] = 'C') and (InFile.bBk = 19) then
                                   begin
                                       StartNewLine;
                                       WriteLn(OutFile, 'Psalm '+InFile.sChapter);
                                       WriteLn(OutFile);
                                   end;
                               if wd[3] = 'B' then
                                   begin
                                   fProse := true;
                                   CloseASCII;
                                   end;
                               end;
                           end;
                       tokContent:
                           begin
                           if wd = '<BO>' then
                               fInclude := true
                           else if wd = '<BN>' then
                               fInclude := true
                           else if wd = '<BO>' then
                               fInclude := ApocryphaCheckBox.Checked
                           else if wd = '<Tt>' then
                               OpenASCII;
                           end;
                       tokControl:
                           begin
                           if wd = '<CM>' then
                               begin
                               StartNewLine;
                               if fProse then
                                   begin
                                   if fASCIIisOpen then WriteLn(OutFile);
                                   sLine := '   '
                                   end
                               end
                           else if wd = '<CL>' then
                               begin
                               StartNewLine;
                               sLine := '   ';
                               end
                           else if wd = '<Pp>' then
                               fProse := true
                           else if wd = '<PP>' then
                               fProse := false
                           else if wd = '<RF>' then
                               fSkip := true
                           else if wd = '<Rf>' then
                               fSkip := false
                           else if wd = '<RN>' then
                               fSkip := true
                           else if wd = '<Rn>' then
                               fSkip := false
                           else if wd = '<ZZ>' then
                               fInclude := false
                           end;
                       tokChar:
                           begin
                           if fInclude and (not fSkip) then
                               begin
                               sLine := sLine + wd;
                               CheckEOL;
                               end;
                           end;
                       tokFont:
                           begin
                           if wd = '<FI>' then
                               begin
                               if fInclude then
                                   sLine := sLine + '[';
                               end
                           else if wd = '<Fi>' then
                               begin
                               if fInclude then
                                   sLine := sLine + ']';
                               end
                           end;
                   end
               until tok = tokEOF;
               if fASCIIisOpen then writeln(OutFile, sLine);
               CloseASCII;
               Label3.Caption := '';
               end;
             2: begin
               Label3.Caption := 'Converting ASCII postings';
               bLastBook := 255;
               bChap := 0;
               FileMode := 1;
               iFileNumber := 0;
               fASCIIisOpen := false;
               sLine := '';
               repeat
                   Application.ProcessMessages;
                   wd := ANSIToOEM(InFile.GetToken(tok));
                   case tok of
                       tokEOF:
                           CloseASCII;
                       tokWord:
                           begin
                           if fInclude and (not fSkip) then
                               begin
                               sLine := sLine + wd;
                               CheckEOL;
                               end;
                           end;
                       tokSpace:
                           begin
                           if fInclude and (not fSkip) then
                               begin
                               sLine := sLine + wd;
                               CheckEOL;
                               end;
                           end;
                       tokSync:
                           begin
                           if fInclude and (length(wd) > 3) then
                               begin
                               if wd[3] = 'V' then
                                   begin
                                   sLine := sLine + '{' + InFile.sChapter+':'+
                                           InFile.sVerse+'} ';
                                   CheckEOL
                                   end
                               else if (wd[3] = 'C') then
                                   begin
                                   OpenNTChapter;
                                   if (InFile.bBk = 19) then
                                       begin
                                       StartNewLine;
                                       if fASCIIisOpen then
                                           begin
                                           WriteLn(OutFile, 'Psalm '+InFile.sChapter);
                                           WriteLn(OutFile);
                                           end;
                                       end;
                                   end;
                               if wd[3] = 'B' then
                                   begin
                                   fProse := true;
                                   CloseASCII;
                                   end;
                               end;
                           end;
                       tokContent:
                           begin
                           if wd = '<BO>' then
                               fInclude := true
                           else if wd = '<BN>' then
                               begin
                               fInclude := true;
                               iFileNumber := 0;
                               end
                           else if wd = '<BO>' then
                               fInclude := ApocryphaCheckBox.Checked
                           end;
                       tokControl:
                           begin
                           if wd = '<CM>' then
                               begin
                               StartNewLine;
                               if fProse then
                                   begin
                                   if fASCIIisOpen then WriteLn(OutFile);
                                   sLine := '   '
                                   end
                               end
                           else if wd = '<CL>' then
                               begin
                               StartNewLine;
                               sLine := '   ';
                               end
                           else if wd = '<Pp>' then
                               fProse := true
                           else if wd = '<PP>' then
                               fProse := false
                           else if wd = '<RF>' then
                               fSkip := true
                           else if wd = '<Rf>' then
                               fSkip := false
                           else if wd = '<RN>' then
                               fSkip := true
                           else if wd = '<Rn>' then
                               fSkip := false
                           else if wd = '<ZZ>' then
                               fInclude := false
                           end;
                       tokChar:
                           begin
                           if fInclude and (not fSkip) then
                               begin
                               sLine := sLine + wd;
                               CheckEOL;
                               end;
                           end;
                       tokFont:
                           begin
                           if wd = '<FI>' then
                               begin
                               if fInclude then
                                   sLine := sLine + '[';
                               end
                           else if wd = '<Fi>' then
                               begin
                               if fInclude then
                                   sLine := sLine + ']';
                               end
                           end;
                   end
               until tok = tokEOF;
               if fASCIIisOpen then writeln(OutFile, sLine);
               CloseASCII;
               Label3.Caption := '';
               end;
            3: begin
               Label3.Caption := 'Converting to RTF';
               AssignFile(OutFile, DestEdit.Text);
               FileMode := 1;
               Rewrite(OutFile);
               repeat
                   Application.ProcessMessages;
                   wd := InFile.GetToken(tok);
                   case tok of
                       tokWord:
                           begin
                           if fInclude then
                               begin
                               LinePos := LinePos + Length(wd);
                               write(OutFile,wd);
                               end;
                           end;
                       tokSpace:
                           begin
                           if fInclude then
                               begin
                               LinePos := LinePos + Length(wd);
                               if LinePos > 78 then
                                   begin
                                   WriteLn(OutFile,wd);
                                   LinePos := 0;
                                   end
                               else
                                   write(OutFile,wd);
                               end
                           end;
                       tokSync:
                           begin
                           if length(wd) > 1 then
                               begin
                               case wd[2] of
                                   'B': begin
                                       if InFile.sBook <> LastBook then
                                           begin
                                           LastBook := InFile.sBook;
                                           WriteLn(OutFile,'\par '+sTitlePar+
                                               LastBook+'\par '+ParagraphAttributes);
                                           LinePos := 0;
                                           end;
                                       end;
                                   'V':  begin
                                       s := '{\f5\super '+InFile.sChapter+':'+
                                           InFile.sVerse+'}';
                                       Write(OutFile,s);
                                       LinePos := LinePos+Length(s);
                                       end;
                               end;
                               end;
                           end;
                       tokControl:
                           begin
                           if length(wd) > 1 then
                               begin
                               case wd[2] of
                                   'A':  fInclude := false;
                                   'E':  begin
                                       Write(OutFile,'{\b\cf1 ');
                                       LinePos := LinePos + 8;
                                       end;
                                   'F':  fInclude := false;
                                   'H':  begin
                                       fInclude := true;
                                       ParagraphAttributes := sHebrewTitlePar;
                                       Write(OutFile,ParagraphAttributes);
                                       LinePos := LinePos + Length(ParagraphAttributes);
                                       end;
                                   'I' : begin
                                       Write(OutFile,'{\i\cf1 ');
                                       LinePos := LinePos + 7;
                                       end;
                                   'J' : begin
                                       Write(OutFile,'{\scaps ');
                                       LinePos := LinePos + 8;
                                       end;
                                   'K':  fInclude := false;
                                   'M':  begin
                                       if fInclude then
                                           begin
                                           writeln(OutFile);
                                           write(OutFile,'\par '+ParagraphAttributes);
                                           LinePos := Length(ParagraphAttributes) + 5;
                                           end;
                                       end;
                                   'N':  begin
                                       fInclude := true;
                                       ParagraphAttributes := sNormalPar;
                                       Write(OutFile,ParagraphAttributes);
                                       LinePos := LinePos + Length(ParagraphAttributes);
                                       end;
                                   'P':  begin
                                       fInclude := true;
                                       ParagraphAttributes := sPoetryPar;
                                       Write(OutFile,ParagraphAttributes);
                                       LinePos := LinePos + Length(ParagraphAttributes);
                                       end;
                                   'Q':  begin
                                       fInclude := true;
                                       ParagraphAttributes := sTitlePar;
                                       Write(OutFile,ParagraphAttributes);
                                       LinePos := LinePos + Length(ParagraphAttributes);
                                       end;
                                   'R' : begin
                                       Write(OutFile,'\cf6 ');
                                       LinePos := LinePos + 4;
                                       end;
                                   'S':  begin
                                       fInclude := true;
                                       ParagraphAttributes := sSelahPar;
                                       Write(OutFile,ParagraphAttributes);
                                       LinePos := LinePos + Length(ParagraphAttributes);
                                       end;
                                   'T':  begin
                                       fInclude := true;
                                       ParagraphAttributes := sTitlePar;
                                       Write(OutFile,ParagraphAttributes);
                                       LinePos := LinePos + Length(ParagraphAttributes);
                                       end;
                                   'U' : begin
                                       Write(OutFile,'{\ul ');
                                       LinePos := LinePos + 4;
                                       end;
                                   'W':  begin
                                       fInclude := true;
                                       ParagraphAttributes := sNormalQuotePar;
                                       Write(OutFile,ParagraphAttributes);
                                       LinePos := LinePos + Length(ParagraphAttributes);
                                       end;
                                   'X':  fInclude := false;
                                   'Y':  begin
                                       fInclude := true;
                                       ParagraphAttributes := sPoetryQuotePar;
                                       Write(OutFile,ParagraphAttributes);
                                       LinePos := LinePos + Length(ParagraphAttributes);
                                       end;
                                   'Z':  fInclude := false;
                                   'a':  fInclude := false;
                                   'c':  fInclude := false;
                                   'e':  begin
                                       Write(OutFile,'}');
                                       inc(LinePos);
                                       end;
                                   'h':  fInclude := false;
                                   'i':  begin
                                       Write(OutFile,'}');
                                       inc(LinePos);
                                       end;
                                   'j':  begin
                                       Write(OutFile,'}');
                                       inc(LinePos);
                                       end;
                                   'n':  fInclude := false;
                                   'p':  fInclude := false;
                                   'r':  begin
                                       Write(OutFile,'}');
                                       inc(LinePos);
                                       end;
                                   'u':  begin
                                       Write(OutFile,'}');
                                       inc(LinePos);
                                       end;

                               end;
                               end;
                           end;
                       tokChar:
                           begin
                           if fInclude then
                               begin
                               write(OutFile,wd);
                               LinePos := LinePos + length(wd);
                               end;
                           end;
                    end;
               until tok = tokEOF;
               writeln(OutFile,'\par }');
               CloseFile(OutFile);
               Label3.Caption := '';
               end;
            4: begin // GBF
                   Label3.Caption := 'Converting to GBF';
                   OutGBF := TWriteGBF.Create;
                   OutGBF.Init(Trim(DestEdit.Text));
                   OutGBF.Out('<H000>');
                   repeat
                       Application.ProcessMessages;
                       wd := InFile.GetToken(tok);
                       if tok <> tokEOF then OutGBF.Out(wd);
                   until tok = tokEOF;
                   OutGBF.Done;
                   OutGBF.Free;
                   Label3.Caption := '';
               end;
            5: begin // HTML
                   Label3.Caption := 'Converting to HTML';
                   fHTMLisOpen := false;
                   repeat
                       Application.ProcessMessages;
                       wd := Infile.GetToken(tok);
                       case tok of
                       tokEOF:
                           CloseHTML;
                       tokWord:
                           begin
                           if fInclude and (not fSkip) then
                               begin
                               sLine := sLine + wd;
                               CheckHTMLEOL;
                               end;
                           end;
                       tokSpace:
                           begin
                           if fInclude and (not fSkip) then
                               begin
                               sLine := sLine + wd;
                               CheckHTMLEOL;
                               end;
                           end;
                       tokSync:
                           begin
                           if fInclude and (length(wd) > 3) then
                               begin
                               if wd[3] = 'V' then
                                   begin
                                   if fRed then
                                       sLine := sLine + '</FONT>';
                                   sLine := sLine + '<FONT COLOR="#0000ff"><FONT SIZE=-1><SUP>'+
                                       InFile.sChapter+':'+
                                       InFile.sVerse+'</SUP></FONT></FONT>';
                                   if fRed then
                                       sLine := sLine + '<FONT COLOR="#ff0000">';
                                   CheckHTMLEOL
                                   end
                               else if (wd[3] = 'C') and (InFile.bBk = 19) then
                                   begin
                                       StartNewHTMLLine;
                                       if fHTMLisOpen then
                                           begin
                                           WriteLn(OutFile, '<P><H2>Psalm '+
                                               InFile.sChapter+'</H2>');
                                           WriteLn(OutFile);
                                           end;
                                   end;
                               if wd[3] = 'B' then
                                   begin
                                   fProse := true;
                                   CloseHTML;
                                   end;
                               end;
                           end;
                       tokContent:
                           begin
                           if wd = '<BO>' then
                               fInclude := true
                           else if wd = '<BN>' then
                               fInclude := true
                           else if wd = '<BO>' then
                               fInclude := ApocryphaCheckBox.Checked
                           else if wd = '<Tt>' then
                               OpenHTML;
                           end;
                       tokControl:
                           begin
                           if wd = '<CM>' then
                               begin
                               StartNewHTMLLine;
                               if not fProse then
                                   begin
                                   sLine := sLine + ' &nbsp;';
                                   end
                               end
                           else if wd = '<CL>' then
                               begin
                               StartNewHTMLLine;
                                   sLine := sLine + ' &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
                               end
                           else if wd = '<Pp>' then
                               fProse := true
                           else if wd = '<PP>' then
                               fProse := false
                           else if wd = '<RF>' then
                               fSkip := true
                           else if wd = '<Rf>' then
                               fSkip := false
                           else if wd = '<RN>' then
                               fSkip := true
                           else if wd = '<Rn>' then
                               fSkip := false
                           else if wd = '<ZZ>' then
                               fInclude := false
                           end;
                       tokChar:
                           begin
                           if fInclude and (not fSkip) then
                               begin
                               if wd = '"' then
                                   sLine := sLine + '&quot;'
                               else
                                   sLine := sLine + wd;
                               CheckHTMLEOL;
                               end;
                           end;
                       tokFont:
                           begin
                           if fInclude then
                               begin
                               if wd = '<FI>' then
                                   sLine := sLine + '<I>'
                               else if wd = '<Fi>' then
                                   sLine := sLine + '</I>'
                               else if wd = '<FR>' then
                                   begin
                                   if not fRed then
                                       begin
                                       sLine := sLine + '<FONT COLOR="#ff0000">';
                                       fRed := true
                                       end
                                   end
                               else if wd = '<Fr>' then
                                   begin
                                   if fRed then
                                       begin
                                       sLine := sLine + '</FONT>';
                                       fRed := false
                                       end
                                   end
                               end;
                           end;
                       end;
                   until tok = tokEOF;
                   Label3.Caption := '';
               end;
        end;
        InFile.Done;
        end;
    InFile.Free;
    except
    showmessage('Error!');
    end;
    GoBitBtn.Enabled := true;
    QuickButton.Enabled := true;
end;

procedure TGBFConverterMainForm.GoBitBtnClick(Sender: TObject);
begin
    DoConversion;
end;

procedure TGBFConverterMainForm.Timer1Timer(Sender: TObject);
begin
    If InFile <> nil then
        VerseLabel.Caption := InFile.sBook+' ['+IntToStr(InFile.bBk)+'] '+
        InFile.sChapter+':'+InFile.sVerse
    else
        VerseLabel.Caption := '';
end;

procedure TGBFConverterMainForm.QuickConversion;
begin
    FormatRadioGroup.ItemIndex := 1;
    DoConversion;
    FormatRadioGroup.ItemIndex := 2;
    DoConversion;
    FormatRadioGroup.ItemIndex := 5;
    DoConversion;
end;

procedure TGBFConverterMainForm.FormShow(Sender: TObject);
begin
    VerseLabel.Caption := '';
    WdLabel.Caption := '';
end;

(*
procedure TGBFConverterMainForm.TransformButtonClick(Sender: TObject);
var apoc: textfile;
    last, s, sBook, sChap, sVs: string;
    blankcount, i: integer;
begin
    TransformButton.Enabled := false;
    blankcount := 0;
    assignfile(apoc, trim(sourceedit.text));
    reset(apoc);
    assignfile(outfile, trim(destedit.text));
    rewrite(outfile);
    last := '';
    while not eof(apoc) do
        begin
        readln(apoc, s);
        if s = '' then
            begin
            inc(blankcount);
            if last <> '' then
                begin
                writeln(outfile, last, '~M');
                last := '';
                end;
            end
        else
            begin
            if blankcount >= 2 then
                writeln(outfile, '~T',s,'~N~M') // book title
            else if blankcount = 1 then
                begin
                sBook := '';
                sChap := '';
                sVs := '';
                i := 1;
                while (s[i] <> ' ') and (i <= Length(s)) do
                    begin
                    sBook := sBook + s[i];
                    inc(i);
                    end;
                while (s[i] = ' ') and (i <= Length(s)) do
                    inc(i);
                while (s[i] <> ':') and (i <= Length(s)) do
                    begin
                    sChap := sChap + s[i];
                    inc(i);
                    end;
                inc(i);
                while IsDigit(s[i]) and (i <= Length(s)) do
                    begin
                    sVs := sVs + s[i];
                    inc(i);
                    end;
                write(outfile, '~B'+sBook+';~C'+sChap+';');
                if sVs <> '' then
                    write(outfile, '~V'+sVs+';');
                end
            else
                begin
                if last <> '' then
                    begin
                    writeln(outfile, last);
                    end;
                last := s;
                end;
            blankcount := 0;
            end;
        end;
    if last <> '' then
        begin
        writeln(outfile, last);
        last := s;
        end;
    closefile(outfile);
    closefile(apoc);
    TransformButton.Enabled := true;
end;
*)

procedure TGBFConverterMainForm.FormatRadioGroupClick(Sender: TObject);
begin
    Case FormatRadioGroup.ItemIndex of
        0: // Plain ASCII (one file)
            DestEdit.Text := 'pub\web.txt';
        1: // Plain ASCII (one file per book)
            DestEdit.Text := 'pub\web.htm';
        2: // Daily posts
            DestEdit.Text := 'pub\queue\web.txt';
        3: // RTF
            DestEdit.Text := 'pub\web.rtf';
        4: // GBF
            DestEdit.Text := 'pub\web.gbf';
        5: // HTML
            DestEdit.Text := 'pub\htm\web.htm';
    end;
end;

procedure TGBFConverterMainForm.QuickButtonClick(Sender: TObject);
begin
    QuickConversion;
end;

procedure TGBFConverterMainForm.FormActivate(Sender: TObject);
begin
    if ParamCount > 0 then
        if ParamStr(1) = 'quick' then
            begin
            QuickConversion;
            close;
            end;
end;

end.
