#!/bin/sh
# This only works if --with-icu was passed to configure 

### German test keys
../parsekey "Matthäus 2:3-12" "de"
../parsekey "Römer 2:13" "de"
../parsekey "Matthäus 1:2-Röm 3:13" "de"
../parsekey "1. Könige 2" "de"

../parsekey "1. Könige - 2. Könige; Markus 1:1; Matthäus 2:1; Matthäus; 1.Kön" "de"
../parsekey "1. Könige - 2. Könige; Markus 1:1; Matthäus 2:1; Matthäus; 1.Kön-2.Kön;I Kings-Matthäus" "de"
../parsekey "Maleachi 1:1 - Matthäus 2:1" "de"
