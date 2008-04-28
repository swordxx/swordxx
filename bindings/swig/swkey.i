%{
#include "swkey.h"
%}

%ignore sword::SWKey::userData;

%rename (getPosition) sword::SW_POSITION::operator char;

%include "swkey.h"
