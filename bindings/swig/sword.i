%module sword
%{
#undef bool
#undef assert
#undef LOCAL
#undef list
%}

%include "swconfig.i"
%include "swmgr.i"
%include "swmodule.i"
%include "swkey.i"
%include "versekey.i"
%include "localemgr.i"
%include "swfiltermgr.i"
