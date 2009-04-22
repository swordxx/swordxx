#!/bin/sh
#******************************************************************************
#
# $Id$
#
# Copyright 1998-2009 CrossWire Bible Society (http://www.crosswire.org)
#	CrossWire Bible Society
#	P. O. Box 2528
#	Tempe, AZ  85280-2528
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation version 2.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#

../parsekey "1jn.2.3"
../parsekey "1 jn.2.3"
../parsekey "Ijn.2.3"
../parsekey "I jn.2.3"
../parsekey "1jn 2.3"
../parsekey "1 jn 2.3"
../parsekey "Ijn 2.3"
../parsekey "I jn 2.3"
../parsekey "1jn.2:3"
../parsekey "1 jn.2:3"
../parsekey "Ijn.2:3"
../parsekey "I jn.2:3"
../parsekey "1jn 2:3"
../parsekey "1 jn 2:3"
../parsekey "Ijn 2:3"
../parsekey "I jn 2:3"

../parsekey "1.jn.2.3"
../parsekey "1. jn.2.3"
../parsekey "I.jn.2.3"
../parsekey "I. jn.2.3"
../parsekey "1.jn 2.3"
../parsekey "1. jn 2.3"
../parsekey "I.jn 2.3"
../parsekey "I. jn 2.3"
../parsekey "1.jn.2:3"
../parsekey "1. jn.2:3"
../parsekey "I.jn.2:3"
../parsekey "I. jn.2:3"
../parsekey "1.jn 2:3"
../parsekey "1. jn 2:3"
../parsekey "I.jn 2:3"
../parsekey "I. jn 2:3"

### German test keys
../parsekey "1. Johannes 2:3" "de"

### Range parsing tests
../parsekey "1. Johannes 1:1-3:10" "de"
../parsekey "1. Joh 1:1-3:10" "de"
../parsekey "1Jn 1:1-3:10"
../parsekey "1. Johannes 1:1 - 3:10" "de"
../parsekey "1. Joh 1:1 - 3:10" "de"
../parsekey "1Jn 1:1 - 3:10"
../parsekey "1. Johannes 1:1 -3:10" "de"
../parsekey "1. Joh 1:1 -3:10" "de"
../parsekey "1Jn 1:1 -3:10"
../parsekey "1. Johannes 1:1- 3:10" "de"
../parsekey "1. Joh 1:1- 3:10" "de"
../parsekey "1Jn 1:1- 3:10"

### List parsing tests
../parsekey "1Jn 1:1 3:10"
../parsekey "1Jn 1:1  3:10"
../parsekey "1Jn 1:1,3:10"
../parsekey "1Jn 1:1, 3:10"
../parsekey "1Jn 1:1 ,3:10"
../parsekey "1Jn 1:1 , 3:10"
../parsekey "1Jn 1:1;3:10"
../parsekey "1Jn 1:1; 3:10"
../parsekey "1Jn 1:1 ;3:10"
../parsekey "1Jn 1:1 ; 3:10"
