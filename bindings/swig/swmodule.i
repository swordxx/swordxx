%{
#include "swmodule.h"
using namespace sword;
%}

%include "std_map.i"

typedef std::map < SWBuf, SWBuf > AttributeValue;
typedef std::map < SWBuf, AttributeValue > AttributeList;
typedef std::map < SWBuf, AttributeList > AttributeTypeList;

%template(AttributeValue) std::map<SWBuf, SWBuf>;
%template(AttributeList) std::map<SWBuf, AttributeValue>;
%template(AttributeTypeList) std::map<SWBuf, AttributeList>;

class SWModule {
public:
  bool terminateSearch;

  SWModule (const char *imodname = 0, const char *imoddesc = 0, SWDisplay * idisp = 0, char *imodtype = 0, SWTextEncoding encoding = ENC_UNKNOWN, SWTextDirection dir = DIRECTION_LTR, SWTextMarkup markup = FMT_UNKNOWN, const char* modlang = 0);
  virtual char Error();
  virtual const bool isUnicode() const;
  virtual const ConfigEntMap& getConfig() const;
  virtual const char *getConfigEntry(const char *key) const;

  virtual char SetKey (const SWKey *ikey);
  virtual SWKey& Key() const;
  virtual SWKey *CreateKey ();
  virtual const char* KeyText(const char *imodtype = 0);
  virtual char Display();

//Search functions
  static void nullPercent (char percent, void *userData);
  virtual ListKey & Search (const char *istr, int searchType = 0, int flags = 0, SWKey * scope = 0, bool * justCheckIfSupported = 0, void (*percent) (char, void *) = &nullPercent, void *percentUserData = 0);
  virtual signed char createSearchFramework();
  virtual bool hasSearchFramework();
  virtual bool isSearchOptimallySupported (const char *istr, int searchType, int flags, SWKey * scope);

//navigation functions
%extend {
  const bool next() {
	(*self)++;
	return !self->Error();
  };
  const bool prev() {
	(*self)--;
	return !self->Error();
  };
  const bool inc(const int howFar) {
  	(*self)+=howFar;
	return !self->Error();
  };
  const bool dec(const int howFar) {
  	(*self)-=howFar;
	return !self->Error();
  };
  void setPosition(SW_POSITION pos) {
  	(*self) = pos;
  };
  void top() {
	(*self) = TOP;
  };
  void bottom() {
	(*self) = BOTTOM;
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

  virtual char *getRawEntry();

  virtual void setSkipConsecutiveLinks(bool val);
  virtual bool getSkipConsecutiveLinks();
  virtual AttributeTypeList &getEntryAttributes() const;
  virtual void processEntryAttributes(bool val) const;
  virtual bool isProcessEntryAttributes() const;

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
  virtual void setEntry (const char *inbuf, long len);
  virtual void deleteEntry ();
  
%extend {
  void write(const char* text) {
    (*self) << text;
  };
  
  void writeLink(const SWKey* key) {
    (*self) << key;
  };
}
};
