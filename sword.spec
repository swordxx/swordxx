Summary:	The SWORD Project framework for manipulating Bible texts
Name:		sword
Version:	1.4.5
Release:	1
Icon:		sword.gif
Copyright:	GPL
Packager:	CrossWire Bible Society
URL:		http://www.crosswire.org
Source:		http://www.crosswire.org/download/ftpmirror.tmp/pub/sword/source/v1.4/sword-1.4.5.tar.gz
Group:		Libraries

%description 
The SWORD Project is an effort to create an ever expanding software package for research and study of God and His Word.  The SWORD Bible Framework allows easy manipulation of Bible texts, commentaries, lexicons, dictionaries, etc.  Many frontends are build using this framework.  An installed module set may be shared between any frontend using the framework.


%prep
%setup -n sword


%build

%install

%files 
%config /etc/sword.conf
/usr/bin/cheatah
%doc /usr/doc/sword/INSTALL
%doc /usr/doc/sword/LICENSE
%doc /usr/doc/sword/README
%doc /usr/doc/sword/samples/mods.conf.large
%doc /usr/doc/sword/samples/mods.conf.small
%doc /usr/doc/sword/samples/mods.d/globals.conf
%doc /usr/doc/sword/samples/mods.d/rwp.conf
%doc /usr/doc/sword/samples/mods.d/strongsnt.conf
%doc /usr/doc/sword/samples/mods.d/strongsot.conf
%doc /usr/doc/sword/samples/mods.d/vines.conf
%doc /usr/doc/sword/samples/mods.d/web.conf
%doc /usr/doc/sword/samples/mods.d/websters.conf
/usr/include/sword/Greek2Greek.h
/usr/include/sword/GreekChars.h
/usr/include/sword/canon.h
/usr/include/sword/cipherfil.h
/usr/include/sword/echomod.h
/usr/include/sword/femain.h
/usr/include/sword/filemgr.h
/usr/include/sword/gbffootnotes.h
/usr/include/sword/gbfhtml.h
/usr/include/sword/gbfplain.h
/usr/include/sword/gbfrtf.h
/usr/include/sword/gbfstrongs.h
/usr/include/sword/hrefcom.h
/usr/include/sword/listkey.h
/usr/include/sword/plainhtml.h
/usr/include/sword/rawcom.h
/usr/include/sword/rawfiles.h
/usr/include/sword/rawgbf.h
/usr/include/sword/rawld.h
/usr/include/sword/rawstr.h
/usr/include/sword/rawtext.h
/usr/include/sword/rawverse.h
/usr/include/sword/regex.h
/usr/include/sword/rwphtml.h
/usr/include/sword/rwprtf.h
/usr/include/sword/sapphire.h
/usr/include/sword/strkey.h
/usr/include/sword/swcipher.h
/usr/include/sword/swcom.h
/usr/include/sword/swcomprs.h
/usr/include/sword/swconfig.h
/usr/include/sword/swdisp.h
/usr/include/sword/swdisprtf.h
/usr/include/sword/swdisprtfchap.h
/usr/include/sword/swfilter.h
/usr/include/sword/swkey.h
/usr/include/sword/swld.h
/usr/include/sword/swlog.h
/usr/include/sword/swmacs.h
/usr/include/sword/swmgr.h
/usr/include/sword/swmodule.h
/usr/include/sword/swtext.h
/usr/include/sword/swwinlog.h
/usr/include/sword/tbdisp.h
/usr/include/sword/unixstr.h
/usr/include/sword/utilconf.h
/usr/include/sword/utilfuns.h
/usr/include/sword/utilstr.h
/usr/include/sword/versekey.h
/usr/include/sword/zcom.h
/usr/include/sword/ztext.h
/usr/lib/libsword.a
/usr/lib/libsword.so
%config /usr/share/sword/mods.d/globals.conf
