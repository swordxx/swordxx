#!/bin/sh

# Just let it run the default
../xmltest

#let's try some crazy stuff
../xmltest "<yo mama='stuff' />"
../xmltest "<yo mama='stuff'>"
../xmltest "<yo mama = 'stuff'>"
../xmltest "<yo mama = 'stuff' yoyoma=\"hohum\">"
../xmltest "yo mama = 'stuff' yoyoma=\"hohum\""
../xmltest "yo mama = 'stuff' yoyoma=\"hohum\"/"
