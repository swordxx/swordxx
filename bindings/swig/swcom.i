%{
 #include "swcom.h"
%}

class SWCom : public SWModule {
  SWCom(const char *imodname = 0, const char *imoddesc = 0, SWDisplay * idisp = 0, SWTextEncoding enc = ENC_UNKNOWN, SWTextDirection dir = DIRECTION_LTR, SWTextMarkup mark = FMT_UNKNOWN, const char* ilang = 0);
  virtual ~SWCom();
};

