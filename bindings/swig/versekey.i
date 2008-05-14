%{
#include "versekey.h"
%}


%ignore sword::sbook::versemax;
%ignore sword::VerseKey::setBookAbbrevs;
%ignore sword::VerseKey::setBooks;

%ignore sword::VerseKey::builtin_BMAX;
%ignore sword::VerseKey::builtin_books;
%ignore sword::VerseKey::BMAX;
%ignore sword::VerseKey::books;

%immutable sword::VerseKey::builtin_abbrevs;
%immutable sword::sbook::name;
%immutable sword::sbook::prefAbbrev;
%immutable sword::abbrev::ab;

%include "versekey.h"

%extend sword::abbrev {
	int getAbbrevCount() {
		int abbrevsCnt;
		for (abbrevsCnt = 0; *self[abbrevsCnt].ab; abbrevsCnt++) {}
		return abbrevsCnt-1;
	}

	const struct sword::abbrev* getAbbrevData(int i) {
		return &(self[i]);
	}
}

%extend sword::sbook {
	const int verseMax( int chapter ) {
		if ( chapter > 0  && chapter < self->chapmax ) {
			return self->versemax[chapter-1];
		} else {
			return 0;
		}
	}
};



%extend sword::VerseKey {
	/* C++-style cast */
	static sword::VerseKey *castTo(sword::SWKey *o) {
		return dynamic_cast<sword::VerseKey*>(o);
	}


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
};
