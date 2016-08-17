#!/bin/sh

rm -rf tmp/osis_osis2modcipher/
mkdir -p tmp/osis_osis2modcipher/mods.d
mkdir -p tmp/osis_osis2modcipher/modules

cat > tmp/osis_osis2modcipher/mods.d/osisreference.conf <<!
[OSISReference]
DataPath=./modules/
ModDrv=zText
CipherKey=abc123
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

../../utilities/osis2mod tmp/osis_osis2modcipher/modules/ osisReference.xml -z -c abc123 2>&1 | grep -v \$Rev|grep -v "with phrase"

cp osis_basic.good osis_osis2modcipher.good
cd tmp/osis_osis2modcipher
../../../osistest OSISReference
