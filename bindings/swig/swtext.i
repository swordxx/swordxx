%{
 #include "swtext.h"
%}

class SWText : public SWModule {
public:
  SWText (const char *imodname = 0, const char *imoddesc = 0, SWDisplay * idisp = 0, SWTextEncoding encoding = ENC_UNKNOWN, SWTextDirection dir = DIRECTION_LTR, SWTextMarkup markup = FMT_UNKNOWN, const char* ilang = 0);
  virtual ~ SWText ();
};

