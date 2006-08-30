%{
#include "swkey.h"
%}

%ignore sword::SWKey::operator++;
%ignore sword::SWKey::operator+=;
%ignore sword::SWKey::operator--;
%ignore sword::SWKey::operator-;
%ignore sword::SWKey::operator-=;
%ignore sword::SWKey::operator=;
%ignore sword::SWKey::operator==;
%ignore sword::SWKey::operator<;
%ignore sword::SWKey::operator>;
%ignore sword::SWKey::operator!=;
%ignore sword::SWKey::operator>=;
%ignore sword::SWKey::operator<=;
%ignore sword::SWKey::operator const char *;

%ignore sword::SWKey::userData;

%rename (getPosition) sword::SW_POSITION::operator char;

%include "swkey.h"


//%extend {
//  void setPersist(signed char persists) {
//  	self->Persist(persists);
//  };
//}

 
//%extend {
//  /**
//  * Goes to the next key. Only useful for VerseKeys at the moment.
//  */
//  void next() {
//  	(*self)++;
//  };
//  void prev() {
//  	(*self)++;
//  };
//  void setKey(const SWKey* key) {
//  	self->copyFrom(*key);
//  };
//  
//  /**
//  * We define here some casting functions since Perl etc. don't support casts of pointers.
//  */
//  VerseKey* toVerseKey() {
//	return dynamic_cast<VerseKey*>(self);
//  };
//}
