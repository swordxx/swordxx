#ifndef SWOBJECT_H
#define SWOBJECT_H

class SWClass {
private:
        char **descends;

public:

    SWClass(const char **descends) {
        this->descends = descends;
    }

    bool isAssignableFrom(const char *className) {
        for (int i = 0; descendents[i]; i++) {
            if (!stricmp(descentents[i], className))
                return true;
        }
        return false;
    }
}

class SWObject {
    private SWClass *myclass;
    SWClass *getClass() { return myclass; }
}

#endif
