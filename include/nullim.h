#ifndef NULLIM_H
#define NULLIM_H

#include <swinputmeth.h>


class NullIM : public SWInputMethod {

public:
	NullIM();
	int * translate(char ch);
};

#endif
