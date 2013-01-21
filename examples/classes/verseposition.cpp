#include <iostream>
#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>

using namespace sword;
using namespace std;

int main(int argc, char **argv) {

        const char *modName = "HunKar";
        SWMgr library;
        SWModule *book = library.getModule(modName);
        if (!book) {
                cerr << "Can't find module: " << modName << endl;
                return -1;
        }
        VerseKey* key = ((VerseKey *)book->getKey());

        key->setIntros(true);
        book->setSkipConsecutiveLinks(true);
        book->setPosition(TOP);

        cout << *key << endl;
        return 0;
}
