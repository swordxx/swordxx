//---------------------------------------------------------------------------
#ifndef prn2swordfrmH
#define prn2swordfrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *Label1;
	TEdit *FilePathEdit;
	TButton *FSButton;
	TPanel *Panel2;
	TButton *Button1;
	TMemo *Memo1;
	TOpenDialog *FSDialog;
	void __fastcall FSButtonClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
	int fp, vfp, cfp, bfp, dfp;
	void openFiles(char *fileName);
	void filterVerse(char *finalBuf, char lang);
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
	void ProcessFile(char *fileName);
	char getVerse(int fp, int *verseNum, char *verseBuf, char testament);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
