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
};
