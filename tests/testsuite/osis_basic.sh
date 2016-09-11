#!/bin/sh

rm -rf tmp/osis_basic/
mkdir -p tmp/osis_basic/mods.d
mkdir -p tmp/osis_basic/modules

cat > tmp/osis_basic/mods.d/osisreference.conf <<!
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

../../utilities/osis2mod tmp/osis_basic/modules/ osisReference.xml -z 2>&1 | grep -v \$Rev

cd tmp/osis_basic && ../../../osistest OSISReference
