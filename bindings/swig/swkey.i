%{
#include "swkey.h"
%}

%ignore sword::SWKey::userData;
%ignore sword::SWKey::Index;
%ignore sword::SWKey::Traversable;

%rename (getPosition) sword::SW_POSITION::operator char;

%include "swkey.h"

%extend sword::SWKey {
        %extend {
                char *__str__() {
                        return const_cast<char *>(self->getText());
                }
        }

};
