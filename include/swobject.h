#ifndef SWOBJECT_H
#define SWOBJECT_H

#include <utilfuns.h>
#if !defined(__GNUC__) && !defined(_WIN32_WCE)
#else
#include <unixstr.h>
#endif

#include <defs.h>
#include <string.h>

#define SWDYNAMIC_CAST(className, object) (className *)((object)?((object->getClass()->isAssignableFrom(#className))?object:0):0)

/**
* Class used for SWDYNAMIC_CAST to save the inheritance order.
*/
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

/** Base class for major Sword classes.
* SWObject is the base class for major Sword classes like SWKey.
* It is used because dynamic_cast is not available on all plattforms supported
* by Sword. Use SWDYNAMIC_CAST(classname, object) instead of dynamic_cast<classname>(object).
*/
class SWObject {
protected:
	SWClass * myclass;
     
public:
	/** Use this to get the class definition and inheritance order.
	* @return The class definition of this object
	*/
	const SWClass *getClass () const {
		return myclass;
	}
};

#endif
