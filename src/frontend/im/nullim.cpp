#include <nullim.h>


NullIM::NullIM() {
}

int *NullIM::translate(char ch) {
	static int retVal[1];
	*retVal = ch;
	return retVal;
}
