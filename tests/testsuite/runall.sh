#!/bin/sh

TESTSUITE="verseparsing-utf8 $TESTSUITE"
TESTSUITE="verseparsing $TESTSUITE"

for i in $TESTSUITE; do
	./runtest.sh $i -q
	if [ $? -ne 0 ]; then
		echo $i: FAILED
		echo ""
		echo To see problems, try running:
		echo ./runtest.sh $i
		echo ""
		exit 1
	else
		echo $i: PASSED.
	fi
done
echo "ALL PASSED!"
exit 0
fi
