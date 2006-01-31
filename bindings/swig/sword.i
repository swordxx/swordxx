/** @mainpage
* This is the API documentation of the SWIG interface to Sword!<BR>
* The interface contains the most useful classes of Sword to read, navigate and create/write modules. The SWIG intreface is interesting
* for all who want to write Perl/PHP/Phyton/Ruby/TCL/Java code using the Sword library.<BR>
* My aim is to support all who do so much work for the Sword project (Troy, Chris and all the others) and to make their work more easy with the SWIG interface.
* The interface tries to use the function names of the Sword C++ API, but since operators are not (yet) supported in SWIG there are some additions to the functions, the operators can't be used.<BR>
* Here are the basic classes you should know of:
* @ref SWMgr, SWModule, SWKey, VerseKey and SWConfig.
*/

%module Sword
%{
#undef bool
#undef assert
#undef LOCAL
#undef list
%}

%include "../../include/defs.h"

%include "swconfig.i"
%include "swmgr.i"
%include "swmodule.i"

%include "swkey.i"
%include "versekey.i"
%include "listkey.i"
%include "treekey.i"
%include "treekeyidx.i"

%include "localemgr.i"
%include "swfiltermgr.i"
%include "encfiltmgr.i"
%include "markupfiltmgr.i"

%include "swtext.i"
%include "rawtext.i"
%include "ztext.i"

%include "swcom.i"
%include "rawcom.i"
%include "zcom.i"

%include "swgenbook.i"
%include "rawgenbook.i"

%include "swld.i"
%include "rawld.i"
%include "rawld4.i"
%include "zld.i"

%include "swcompress.i"
%include "lzsscompress.i"
%include "zipcompress.i"

%include "swbuf.i"
