#!/bin/sh

TESTSUITE=`for i in *.good; do basename $i .good; done`

for i in $TESTSUITE; do
	echo -n "$i: "
	./runtest.sh $i -q
	if [ $? -ne 0 ]; then
		echo FAILED
		echo ""
		echo To see problems, try running:
		echo ./runtest.sh $i
		echo ""
		exit 1
	else
		echo PASSED.
	fi
done
echo "ALL PASSED!"
exit 0
fi
