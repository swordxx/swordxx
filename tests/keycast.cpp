#include <swmgr.h>
#include <iostream>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main (int argc, char* argv[]) {
        SWMgr mgr;

//the commented out code works
/* 
        StringList globalOptions = mgr.getGlobalOptions();
        for (StringList::iterator it = globalOptions.begin(); it != globalOptions.end(); it++) {
                std::cout << *it << std::endl;

                StringList values = mgr.getGlobalOptionValues((*it).c_str());
                for (StringList::iterator it2 = values.begin(); it2 != values.end(); it2++) {
                        std::cout << "\t"<< *it2 << std::endl;
                }
        }
*/

//crashes
	StringList values = mgr.getGlobalOptionValues("Footnotes");
        for (StringList::iterator it2 = values.begin(); it2 != values.end(); it2++) {
              std::cout << "\t"<< *it2 << std::endl;
        }    
};





