%{
#include <lzsscomprs.h>
using namespace sword;
%}

class LZSSCompress : public SWCompress {
public:
  LZSSCompress ();
  virtual ~LZSSCompress ();
  virtual void Encode (void);
  virtual void Decode (void);
};
