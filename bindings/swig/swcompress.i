%{
#include <swcomprs.h>
using namespace sword;
%}

class SWCompress {
public:
  SWCompress();
  virtual ~SWCompress();

  virtual char *Buf (const char *buf = 0, unsigned long *len = 0);
  virtual char *zBuf (unsigned long *len, char *buf = 0);
  virtual unsigned long GetChars (char *buf, unsigned long len);
  virtual unsigned long SendChars (char *buf, unsigned long len);
  virtual void Encode (void);
  virtual void Decode (void);
};
