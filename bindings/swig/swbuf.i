%{
#include "swbuf.h"
%}

%ignore sword::SWBuf::operator const char *;
%ignore sword::SWBuf::operator[];
%ignore sword::SWBuf::operator+;
%ignore sword::SWBuf::operator==;
%ignore sword::SWBuf::operator!=;
%ignore sword::SWBuf::operator+=;
%ignore sword::SWBuf::operator-=;
%ignore sword::SWBuf::operator<=;
%ignore sword::SWBuf::operator>=;
%ignore sword::SWBuf::operator<;
%ignore sword::SWBuf::operator>;
%ignore sword::SWBuf::operator<<;
%ignore sword::SWBuf::operator>>;
%ignore sword::SWBuf::operator=;
%ignore sword::SWBuf::operator--;
%ignore sword::SWBuf::charAt(unsigned long);

%include "swbuf.h"


%extend sword::SWBuf {
        char charAt2(unsigned int pos) {
                return self->charAt((unsigned long)pos);
        }
};

