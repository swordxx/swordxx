#include <stdio.h>
#include <rawtext.h>
#include <swmgr.h>
#include <regex.h> // GNU
#include <listkey.h>

#include <pthread.h>

static SWModule* target;

void percentUpdate(char percent, void *userData) {
	cout << "percentUpdate";
	cout.flush();
	cout << (int)percent << "% ";
	cout.flush();
}

void* startSearch(void* p) {
	cout << "startSearch ... " << (char*)p << endl ;
	cout.flush();
	ListKey listKey = target->Search((char*)p, -2, 0, 0, 0/*, &percentUpdate */);

	return NULL;
}

int main(int argc, char **argv)
{
	SWMgr manager;
	ListKey listkey;
	VerseKey vk;
	ModMap::iterator it;

	if ((argc != 3) && (argc != 5)) {
		fprintf(stderr, "usage: %s <modname> <\"search string\"> [\"min\" \"max\"]\n", argv[0]);
		exit(-1);
	}

	it = manager.Modules.find(argv[1]);
	if (it == manager.Modules.end()) {
		fprintf(stderr, "Could not find module [%s].  Available modules:\n", argv[1]);
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			fprintf(stderr, "[%s]\t - %s\n", (*it).second->Name(), (*it).second->Description());
		}
		exit(-1);
	}

	target = (*it).second;

	if (argc == 5) {			// if min / max specified
		vk.LowerBound(argv[3]);
		vk.UpperBound(argv[4]);
		vk.Persist(1);
		target->SetKey(vk);
	}

	//create thread with, &startSearch is called wirg the argv[2]
//argument by the thread

	pthread_attr_t *attr = new pthread_attr_t;
	pthread_attr_init(attr);
	pthread_attr_setdetachstate(attr, PTHREAD_CREATE_DETACHED);

	pthread_t *thread = new pthread_t;
	int i = 0;
	i = pthread_create( thread, attr , &startSearch, argv[2]);

	cout << "Created the thread with result: " << i << endl;
	cout << "Waiting ..." << endl;
}
