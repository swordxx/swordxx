#ifndef SWOBJECT_H
#define SWOBJECT_H

#include <utilfuns.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif
#ifdef WIN32
#include <string.h>
#endif

#include <defs.h>

#define SWDYNAMIC_CAST(className, object) (className *)((object->getClass()->isAssignableFrom(#className))?object:0)

class SWDLLEXPORT SWClass {
private:
        const char **descends;

public:

    SWClass(const char **descends) {
        this->descends = descends;
    }

    bool isAssignableFrom(const char *className) const {
        for (int i = 0; descends[i]; i++) {
            if (!stricmp(descends[i], className))
                return true;
        }
        return false;
    }
};

class SWObject {
protected:
	SWClass *myclass;
public:
	const SWClass *getClass() const { return myclass; }
};

#endif
