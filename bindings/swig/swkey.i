%{
 #include "swkey.h"
%}

class SW_POSITION {
public:
  SW_POSITION(char ipos);
};

#define POS_TOP ((char)1)
#define POS_BOTTOM ((char)2)

#define TOP SW_POSITION(POS_TOP)
#define BOTTOM SW_POSITION(POS_BOTTOM)


/**
* The SWKey class for the SWIG-Perl interface.
*/
class SWKey {
public:
  SWKey(const char *ikey = 0);
  //SWKey(SWKey const &k);
  virtual SWKey *clone () const;

  char Persist() const;

%extend {
  
  void setPersist(signed char persists) {
  	self->Persist(persists);
  };

}
  virtual char Error ();

  virtual void setText(const char *ikey);
  virtual const char *getText() const;
  virtual const char *getShortText() const;

  virtual int compare (const SWKey & ikey);
  virtual bool equals(const SWKey &ikey);
  //virtual void setPosition(SW_POSITION);

  virtual void decrement(int steps = 1);
  virtual void increment(int steps = 1);
  virtual char Traversable ();

  virtual long Index() const;
  //virtual long Index(long iindex);
  
%extend {

  /**
  * Goes to the next key. Only useful for VerseKeys at the moment.
  */
  void next() {
  	(*self)++;
  };
  void prev() {
  	(*self)++;
  };
  void setKey(const SWKey* key) {
  	self->copyFrom(*key);
  };

}

};
