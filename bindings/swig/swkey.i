%{
 #include "swkey.h"
%}

class SW_POSITION {
public:
  SW_POSITION(char ipos);
};

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
//  char Persist (signed char ikey);

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
  void next() {
  	(*self)++;
  };
  void prev() {
  	(*self)++;
  };
  void set(const SWKey* key) {
  	self->copyFrom(*key);
  };
}

};
