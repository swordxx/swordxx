%{
 #include "swmodule.h"
%}

//%rename(write) operator<<(const char *);
//%rename(writeLink) operator<<(const SWKey*);
//%rename(data) operator const char *();

//%rename(decrement) operator-=(int decrement);
//%rename(increment) operator+=(int increment);
//%rename(next) operator++(int);
//%rename(prev) operator--(int);
%rename(setPosition) operator=(SW_POSITION p);

class SWModule {
public:
//member data
  bool terminateSearch;

//member functions
  SWModule (const char *imodname = 0, const char *imoddesc = 0, SWDisplay * idisp = 0, char *imodtype = 0, SWTextEncoding encoding = ENC_UNKNOWN, SWTextDirection dir = DIRECTION_LTR, SWTextMarkup markup = FMT_UNKNOWN, const char* modlang = 0);
  virtual char Error();
  virtual const bool isUnicode() const;
  //virtual const ConfigEntMap& getConfig() const;
  virtual const char *getConfigEntry(const char *key) const;

  virtual char SetKey (const SWKey *ikey);
  virtual SWKey& Key() const;
  virtual SWKey *CreateKey ();
  virtual const char* KeyText(const char *imodtype = 0);
  virtual char Display();

//navigation functions
%extend {
  void next() {
	(*self)++;
  };
  void prev() {
	(*self)++;
  };
  void inc(const int howFar) {
  	(*self)+=howFar;
  };
  void dec(const int howFar) {
  	(*self)-=howFar;
  };
}

//data functions
%extend {
  const char* text() {
  	return (const char*)*self;
  };
  const char* StripText() {
  	return self->StripText();
  };

}

//module info functions
  virtual char* Name (const char *imodname = 0);
  virtual char* Description (const char *imoddesc = 0);
  virtual char* Type (const char *imodtype = 0);
  virtual char  Direction(signed char newdir = -1);
  virtual char  Encoding(signed char enc = -1);
  virtual char  Markup(signed char enc = -1);
  virtual char* Lang (const char *imodlang = 0);

//write interface
  virtual bool isWritable ();
  static signed char createModule (const char *);
  virtual SWModule& setentry (const char *inbuf, long len);
  virtual void deleteEntry ();
  
%extend {
  void write(const char* text) {
    (*self)<<text;
  };
  
  void writeLink(const SWKey* key) {
    (*self)<<key;
  };
}
};
