#include <swmgr.h>

int main (int argc, char* argv[]) {
        SWMgr mgr;

//the commented out code works
/* 
        OptionsList globalOptions = mgr.getGlobalOptions();
        for (OptionsList::iterator it = globalOptions.begin(); it != globalOptions.end(); it++) {
                cout << *it << endl;

                OptionsList values = mgr.getGlobalOptionValues((*it).c_str());
                for (OptionsList::iterator it2 = values.begin(); it2 != values.end(); it2++) {
                        cout << "\t"<< *it2 << endl;
                }
        }
*/

//crashes
	OptionsList values = mgr.getGlobalOptionValues("Footnotes");
        for (OptionsList::iterator it2 = values.begin(); it2 != values.end(); it2++) {
              cout << "\t"<< *it2 << endl;
        }    
};





