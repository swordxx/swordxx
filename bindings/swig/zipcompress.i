%{
#include <zipcomprs.h>
using namespace sword;
%}

class ZipCompress : public SWCompress {
public:
  ZipCompress();
  virtual ~ZipCompress ();
  virtual void Encode (void);
  virtual void Decode (void);
};
