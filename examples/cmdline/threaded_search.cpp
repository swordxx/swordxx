#include <stdio.h>
#include <rawtext.h>
#include <swmgr.h>
#include <regex.h> // GNU
#include <listkey.h>

#include <pthread.h>

int cms_currentProgress;

class CSwordModuleSearch {
public:
	CSwordModuleSearch();	
	~CSwordModuleSearch();

	char* m_searchedText;
	SWModule* m_module;
	ListKey m_searchResult;
	bool m_isSearching;

	void startThread();
	void search();
};

void* dummy(void* p) {
	CSwordModuleSearch* moduleSearch = (CSwordModuleSearch*)p;		
	moduleSearch->search();	

	return NULL;
}

void percentUpdate(char percent, void* userData)  {
	cms_currentProgress = (int)percent;
	cout << cms_currentProgress << "% ";
}

CSwordModuleSearch::CSwordModuleSearch() {
	m_isSearching = false;
	m_module = 0;
	m_searchedText = 0;
	cms_currentProgress = -1;
}

CSwordModuleSearch::~CSwordModuleSearch() {
}

void CSwordModuleSearch::startThread()  {
	cout << "startThread" << endl;
	cout.flush();

	pthread_attr_t* attr = new pthread_attr_t;
	pthread_attr_init(attr);
	pthread_attr_setdetachstate(attr, PTHREAD_CREATE_DETACHED);

	pthread_t *thread= new pthread_t;
	m_isSearching = true;
	int i = pthread_create(thread, attr, &dummy, this); 

	cout << "Created the thread: " << i << endl;
	cout.flush();
}	

void CSwordModuleSearch::search()  {
	
	if (!m_module) {
		cout << "Return." << endl;
		return;
	}

	ListKey scopeList = VerseKey().ParseVerseList("Luke;John;Revelation","", true);
	for (int i=0; i < scopeList.Count(); ++i) {
		cout << (const char*)*scopeList.GetElement(i) << endl;
	}
	SWKey* scope = &scopeList;

	m_searchResult = m_module->Search(m_searchedText, -2, REG_ICASE, scope, 0, &percentUpdate);

	if (!scope)
		cout << "bad scope!" << endl;
	m_isSearching = false;
}

int main(int argc, char **argv) {
	SWMgr manager;
	ModMap::iterator it;
	int oldProgress = 0; 

	CSwordModuleSearch* moduleSearch = new CSwordModuleSearch();

	if (argc != 3) {
		fprintf(stderr, "usage: %s <modname> <searched text>\n", argv[0]);
		exit(-1);
	}

	it = manager.Modules.find(argv[1]);
	if (it == manager.Modules.end()) {
		fprintf(stderr, "Could not find module [%s].  Available modules:\n",  argv[1]);
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			fprintf(stderr, "[%s]\t - %s\n", (*it).second->Name(), (*it).second->Description());
		}
		exit(-1);
	}
	
	moduleSearch->m_searchedText = argv[2];
	moduleSearch->m_module = (*it).second;
	moduleSearch->startThread();
	
	cout << "Start loop" << endl;
	cout.flush();
	while (true) {
		if (!moduleSearch->m_isSearching)
			break;
		else 
			cout.flush();
	};

	cout << endl << "Number of found items: " << moduleSearch->m_searchResult.Count() << endl;
	cout << "Finished program" << endl;
	cout.flush();

	delete moduleSearch;
	exit(0);
}

