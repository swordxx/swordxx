#ifndef ENTRIESBLK_H
#define ENTRIESBLK_H

#include <sysdata.h>

class EntriesBlock {
	static const int METAHEADERSIZE = 4;
		// count(4);

	static const int METAENTRYSIZE = 8;
		// offset(4); size(4);

private:
	char *block;
	void setCount(int count);
	void getMetaEntry(int index, long *offset, short *size);
	void setMetaEntry(int index, long offset, short size);

public:
	EntriesBlock(const char *iBlock, long size);
	EntriesBlock();
	~EntriesBlock();

	int getCount();
	int addEntry(const char *entry);
	const char *getEntry(int entryIndex);
	void removeEntry(int entryIndex);
	const char *getRawData(long *size);
};


#endif
