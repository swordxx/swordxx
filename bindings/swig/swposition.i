%{
 #include "swkey.h"
%}

class SW_POSITION {
public:
  SW_POSITION(char ipos);
};

%define POS_TOP ((char)1)
%define POS_BOTTOM ((char)2)

%define TOP SW_POSITION(POS_TOP)
%define BOTTOM SW_POSITION(POS_BOTTOM)
