%{
 #include "zcom.h"
%}

class zCom : public SWCom {
    zCom (const char *ipath, const char *iname = 0, const char *idesc = 0, int blockType = CHAPTERBLOCKS, SWCompress * icomp = 0, SWDisplay * idisp = 0, SWTextEncoding encoding = ENC_UNKNOWN, SWTextDirection dir = DIRECTION_LTR, SWTextMarkup markup = FMT_UNKNOWN, const char* ilang = 0);
    virtual ~ zCom ();
    
   static char createModule (const char *path, int blockBound);
};

