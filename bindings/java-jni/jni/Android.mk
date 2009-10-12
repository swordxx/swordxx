# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# the purpose of this sample is to demonstrate how one can
# generate two distinct shared libraries and have them both
# uploaded in
#


LOCAL_PATH:= $(call my-dir)

# first lib, which will be built statically
#
include $(CLEAR_VARS)

STLPORT_BASE	:= $(NDK_WRAPPERS_BASE)/stlport

LOCAL_MODULE    := libswordcore
LOCAL_C_INCLUDES := ../sword/include
LOCAL_CFLAGS	+= -I$(STLPORT_BASE)/stlport \
		   -D__NEW__ \
		   -D__SGI_STL_INTERNAL_PAIR_H \
		   -DANDROID \
		   -DOS_ANDROID
LOCAL_SRC_FILES := ../../../../../sword/src/modules/comments/zcom/zcom.cpp \
../../../../../sword/src/modules/comments/rawfiles/rawfiles.cpp \
../../../../../sword/src/modules/comments/rawcom4/rawcom4.cpp \
../../../../../sword/src/modules/comments/rawcom/rawcom.cpp \
../../../../../sword/src/modules/comments/swcom.cpp \
../../../../../sword/src/modules/comments/hrefcom/hrefcom.cpp \
../../../../../sword/src/modules/swmodule.cpp \
../../../../../sword/src/modules/tests/echomod.cpp \
../../../../../sword/src/modules/genbook/swgenbook.cpp \
../../../../../sword/src/modules/genbook/rawgenbook/rawgenbook.cpp \
../../../../../sword/src/modules/lexdict/swld.cpp \
../../../../../sword/src/modules/lexdict/rawld4/rawld4.cpp \
../../../../../sword/src/modules/lexdict/zld/zld.cpp \
../../../../../sword/src/modules/lexdict/rawld/rawld.cpp \
../../../../../sword/src/modules/texts/rawtext/rawtext.cpp \
../../../../../sword/src/modules/texts/rawtext4/rawtext4.cpp \
../../../../../sword/src/modules/texts/swtext.cpp \
../../../../../sword/src/modules/texts/ztext/ztext.cpp \
../../../../../sword/src/modules/common/rawstr4.cpp \
../../../../../sword/src/modules/common/lzsscomprs.cpp \
../../../../../sword/src/modules/common/zipcomprs.cpp \
../../../../../sword/src/modules/common/rawverse4.cpp \
../../../../../sword/src/modules/common/swcipher.cpp \
../../../../../sword/src/modules/common/swcomprs.cpp \
../../../../../sword/src/modules/common/rawverse.cpp \
../../../../../sword/src/modules/common/sapphire.cpp \
../../../../../sword/src/modules/common/zstr.cpp \
../../../../../sword/src/modules/common/entriesblk.cpp \
../../../../../sword/src/modules/common/zverse.cpp \
../../../../../sword/src/modules/common/rawstr.cpp \
../../../../../sword/src/modules/filters/gbfwordjs.cpp \
../../../../../sword/src/modules/filters/utf8latin1.cpp \
../../../../../sword/src/modules/filters/utf8greekaccents.cpp \
../../../../../sword/src/modules/filters/utf16utf8.cpp \
../../../../../sword/src/modules/filters/gbfwebif.cpp \
../../../../../sword/src/modules/filters/plainfootnotes.cpp \
../../../../../sword/src/modules/filters/utf8transliterator.cpp \
../../../../../sword/src/modules/filters/gbfstrongs.cpp \
../../../../../sword/src/modules/filters/osisplain.cpp \
../../../../../sword/src/modules/filters/thmlhtmlhref.cpp \
../../../../../sword/src/modules/filters/thmlgbf.cpp \
../../../../../sword/src/modules/filters/utf8utf16.cpp \
../../../../../sword/src/modules/filters/utf8cantillation.cpp \
../../../../../sword/src/modules/filters/utf8arshaping.cpp \
../../../../../sword/src/modules/filters/cipherfil.cpp \
../../../../../sword/src/modules/filters/thmlheadings.cpp \
../../../../../sword/src/modules/filters/thmlscripref.cpp \
../../../../../sword/src/modules/filters/latin1utf8.cpp \
../../../../../sword/src/modules/filters/gbfhtml.cpp \
../../../../../sword/src/modules/filters/thmlosis.cpp \
../../../../../sword/src/modules/filters/utf8nfkd.cpp \
../../../../../sword/src/modules/filters/thmlstrongs.cpp \
../../../../../sword/src/modules/filters/osisvariants.cpp \
../../../../../sword/src/modules/filters/thmlmorph.cpp \
../../../../../sword/src/modules/filters/gbfplain.cpp \
../../../../../sword/src/modules/filters/gbfhtmlhref.cpp \
../../../../../sword/src/modules/filters/utf8html.cpp \
../../../../../sword/src/modules/filters/utf8nfc.cpp \
../../../../../sword/src/modules/filters/rtfhtml.cpp \
../../../../../sword/src/modules/filters/gbfredletterwords.cpp \
../../../../../sword/src/modules/filters/latin1utf16.cpp \
../../../../../sword/src/modules/filters/osisscripref.cpp \
../../../../../sword/src/modules/filters/thmlhtml.cpp \
../../../../../sword/src/modules/filters/gbfthml.cpp \
../../../../../sword/src/modules/filters/teihtmlhref.cpp \
../../../../../sword/src/modules/filters/gbfrtf.cpp \
../../../../../sword/src/modules/filters/gbfosis.cpp \
../../../../../sword/src/modules/filters/teirtf.cpp \
../../../../../sword/src/modules/filters/thmlwordjs.cpp \
../../../../../sword/src/modules/filters/papyriplain.cpp \
../../../../../sword/src/modules/filters/osisfootnotes.cpp \
../../../../../sword/src/modules/filters/osiswordjs.cpp \
../../../../../sword/src/modules/filters/osismorph.cpp \
../../../../../sword/src/modules/filters/osislemma.cpp \
../../../../../sword/src/modules/filters/osisredletterwords.cpp \
../../../../../sword/src/modules/filters/osisrtf.cpp \
../../../../../sword/src/modules/filters/gbfheadings.cpp \
../../../../../sword/src/modules/filters/osisruby.cpp \
../../../../../sword/src/modules/filters/osishtmlhref.cpp \
../../../../../sword/src/modules/filters/utf8bidireorder.cpp \
../../../../../sword/src/modules/filters/thmlrtf.cpp \
../../../../../sword/src/modules/filters/swoptfilter.cpp \
../../../../../sword/src/modules/filters/utf8arabicpoints.cpp \
../../../../../sword/src/modules/filters/osisstrongs.cpp \
../../../../../sword/src/modules/filters/osisheadings.cpp \
../../../../../sword/src/modules/filters/unicodertf.cpp \
../../../../../sword/src/modules/filters/gbffootnotes.cpp \
../../../../../sword/src/modules/filters/greeklexattribs.cpp \
../../../../../sword/src/modules/filters/osiswebif.cpp \
../../../../../sword/src/modules/filters/thmlfootnotes.cpp \
../../../../../sword/src/modules/filters/thmlplain.cpp \
../../../../../sword/src/modules/filters/osisosis.cpp \
../../../../../sword/src/modules/filters/utf8hebrewpoints.cpp \
../../../../../sword/src/modules/filters/osismorphsegmentation.cpp \
../../../../../sword/src/modules/filters/thmlwebif.cpp \
../../../../../sword/src/modules/filters/thmlvariants.cpp \
../../../../../sword/src/modules/filters/plainhtml.cpp \
../../../../../sword/src/modules/filters/thmllemma.cpp \
../../../../../sword/src/modules/filters/gbfmorph.cpp \
../../../../../sword/src/modules/filters/teiplain.cpp \
../../../../../sword/src/modules/filters/swbasicfilter.cpp \
../../../../../sword/src/mgr/stringmgr.cpp \
../../../../../sword/src/mgr/swmgr.cpp \
../../../../../sword/src/mgr/swsearchable.cpp \
../../../../../sword/src/mgr/localemgr.cpp \
../../../../../sword/src/mgr/swconfig.cpp \
../../../../../sword/src/mgr/markupfiltmgr.cpp \
../../../../../sword/src/mgr/encfiltmgr.cpp \
../../../../../sword/src/mgr/swfiltermgr.cpp \
../../../../../sword/src/mgr/swcacher.cpp \
../../../../../sword/src/mgr/installmgr.cpp \
../../../../../sword/src/mgr/swlocale.cpp \
../../../../../sword/src/mgr/filemgr.cpp \
../../../../../sword/src/mgr/versemgr.cpp \
../../../../../sword/src/mgr/ftptrans.cpp \
../../../../../sword/src/utilfuns/swobject.cpp \
../../../../../sword/src/utilfuns/roman.cpp \
../../../../../sword/src/utilfuns/swbuf.cpp \
../../../../../sword/src/utilfuns/utilstr.cpp \
../../../../../sword/src/utilfuns/url.cpp \
../../../../../sword/src/utilfuns/swversion.cpp \
../../../../../sword/src/utilfuns/utilxml.cpp \
../../../../../sword/src/utilfuns/swunicod.cpp \
../../../../../sword/src/utilfuns/regex.c \
../../../../../sword/src/keys/swkey.cpp \
../../../../../sword/src/keys/versetreekey.cpp \
../../../../../sword/src/keys/treekeyidx.cpp \
../../../../../sword/src/keys/versekey.cpp \
../../../../../sword/src/keys/strkey.cpp \
../../../../../sword/src/keys/treekey.cpp \
../../../../../sword/src/keys/listkey.cpp \
../../../../../sword/src/frontend/swdisp.cpp \
../../../../../sword/src/frontend/swlog.cpp

#../../../../../sword/src/mgr/ftplibftpt.cpp \
#../../../../../sword/src/mgr/curlftpt.cpp \
#../../../../../sword/src/mgr/curlhttpt.cpp \
#../../../../../sword/src/utilfuns/win32/dirent.cpp \
#../../../../../sword/src/frontend/framework/femain.cpp \
#../../../../../sword/src/frontend/im/nullim.cpp \
#../../../../../sword/src/frontend/im/swinputmeth.cpp \
#../../../../../sword/src/frontend/im/hebrewmcim.cpp \

include $(BUILD_STATIC_LIBRARY)

# second lib, which will depend on and include the first one
#
include $(CLEAR_VARS)

STLPORT_BASE	:= $(NDK_WRAPPERS_BASE)/stlport

LOCAL_MODULE    := libsword
LOCAL_C_INCLUDES := ../sword/include
LOCAL_CFLAGS	+= -I$(STLPORT_BASE)/stlport \
		   -D__NEW__ \
		   -D__SGI_STL_INTERNAL_PAIR_H \
		   -DANDROID \
		   -DOS_ANDROID

LOCAL_LDLIBS	+= -L$(STLPORT_BASE)/build/lib/obj/arm-linux-gcc/so \
		   -lstlport -lz

LOCAL_SRC_FILES := swordstub.cpp

LOCAL_STATIC_LIBRARIES := libswordcore

include $(BUILD_SHARED_LIBRARY)
