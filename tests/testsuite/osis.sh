#!/bin/sh

rm -rf osis/
mkdir -p osis/mods.d
mkdir -p osis/modules

cat > osis/mods.d/osisreference.conf <<!
[OSISReference]
DataPath=./modules/
ModDrv=zText
Encoding=UTF-8
BlockType=BOOK
CompressType=ZIP
SourceType=OSIS
Lang=en
GlobalOptionFilter=OSISStrongs
GlobalOptionFilter=OSISMorph
GlobalOptionFilter=OSISFootnotes
GlobalOptionFilter=OSISHeadings
GlobalOptionFilter=OSISRedLetterWords
Feature=StrongsNumbers
!

../../utilities/osis2mod osis/modules/ osisReference.xml -z 2>&1 | grep -v \$Rev

cd osis && ../../osistest OSISReference
