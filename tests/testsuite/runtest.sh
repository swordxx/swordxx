#!/bin/sh

if [ "$1" = "" ]; then
	echo usage: $0 "<test_to_run (no .sh)>"
	exit 1
fi

./$1.sh > $1.try
BAD=`diff -u $1.try $1.good`
if [ "$BAD" = "" ]; then
	if [ "$2" = "-q" ]; then
		exit 0
	else
		echo PASSED!
	fi
	exit 0
else
	if [ "$2" = "-q" ]; then
		exit 1
	else
		echo "Script failed at: (- bad output; + should have been)"
		diff -u $1.try $1.good
	fi
	exit 1
fi
