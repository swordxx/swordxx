#!/bin/sh


OPTIONS="--host=arm-linux $OPTIONS"
OPTIONS="--without-curl $OPTIONS"
OPTIONS="--disable-shared $OPTIONS"
OPTIONS="--without-lucene $OPTIONS"

export PATH=$PATH:/usr/local/arm/2.95.3/bin/

. ./usrinst.sh

