%{
#include "versekey.h"
#include "listkey.h"
using namespace sword;
%}

class VerseKey : public SWKey {
public:
  VerseKey(const char *ikey = 0);
  VerseKey(const SWKey * ikey);
  VerseKey(const char *min, const char *max);
  VerseKey(const VerseKey &k);
  virtual ~VerseKey();

  virtual SWKey *clone() const;

//bound management
//  VerseKey & LowerBound(const char *lb);
//  VerseKey & UpperBound(const char *ub);
  VerseKey & LowerBound() const;
  VerseKey & UpperBound() const;
  void ClearBounds();

//data functions
  virtual ListKey ParseVerseList (const char *buf, const char *defaultKey="Genesis 1:1", bool expandRange=false);

  virtual void setText(const char *ikey);
  virtual const char* getText () const;
  virtual const char *getRangeText() const;
  virtual const char *getShortText() const;


  virtual char Traversable();

  virtual void decrement(int step);
  virtual void increment(int step);

  virtual const char *getBookName() const;
  virtual const char *getBookAbbrev() const;

  virtual char Testament() const;
  virtual char Book() const;
  virtual int Chapter() const;
  virtual int Verse() const;

  //virtual char Testament(char itestament);
//  virtual char Book(char ibook);
//  virtual int Chapter(int ichapter);
//  virtual int Verse(int iverse);

  virtual void Normalize(char autocheck = 0);
  virtual char AutoNormalize(char iautonorm = MAXPOS (char));

  virtual char Headings(char iheadings = MAXPOS (char));
  virtual const char *getOSISRef() const;

  virtual int compare(const SWKey & ikey);
  virtual int _compare(const VerseKey & ikey);

  virtual void setLocale(const char *name);
  virtual const char *getLocale() const;

// extensions to Sword's class

%extend {
	/* Get number of books in the given testament
	* testament may be 1 (OT) or 2 (NT)
	*/
	const int bookCount( const int testament ) {
		if ( (testament < 1) || (testament > 2) ) {
			return 0;
		};
		return self->BMAX[testament-1];
	};

	/* Get name of book
	* Returns the name of the booknumber in the givn testament.
	* Testament may be 1 (OT) or 2 (NT)
	* book may be in the range of 1 <= bookCount(testament)
	*/
	const char* bookName( const int testament, const int book ) {
		if ( (testament < 1) || (testament > 2) ) {
			return "";
		};
		if ( (book < 1) || (book > self->BMAX[testament-1]) ) {
			return "";
		}

		return self->books[testament-1][book-1].name;
	};

	/* Get number of chapters in the given testament and book number
	* testament may be 1 (OT) or 2 (NT)
	* book may be in the range 1 <= bookCount(testament)
	*/
	const int chapterCount( const int testament, const int book ) {
		if ( (testament < 1) || (testament > 2) ) {
			return 0;
		};
		if ( (book < 1) || (book > self->BMAX[testament-1]) ) {
			return 0;
		}

		return self->books[testament-1][book-1].chapmax;
	};

	/* Get number of verses in the given chapter of the given in the given testament,
	* testament may be 1 (OT) or 2 (NT)
	* book may be in the range 1 <= bookCount(testament)
	* chapter may be in the range 1 <= chapterCount(testament, book)
	*/
	const int verseCount( const int testament, const int book, const int chapter ) {
		if ( (testament < 1) || (testament > 2) ) {
			return 0;
		};
		if ( (book < 1) || (book > self->BMAX[testament-1]) ) {
			return 0;
		}
		if ( (chapter < 1) || (chapter > self->books[testament-1][book-1].chapmax) ) {
			return 0;
		}

		return self->books[testament-1][book-1].versemax[chapter-1];

	};
}

};
