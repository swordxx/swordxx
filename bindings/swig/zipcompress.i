%{
 #include <zipcomprs.h>
%}

class ZipCompress : public SWCompress {
public:
  ZipCompress();
  virtual ~ZipCompress ();
  virtual void Encode (void);
  virtual void Decode (void);
};
