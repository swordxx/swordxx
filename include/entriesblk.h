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
	void getMetaEntry(int index, unsigned long *offset, unsigned long *size);
	void setMetaEntry(int index, unsigned long offset, unsigned long size);

public:
	EntriesBlock(const char *iBlock, unsigned long size);
	EntriesBlock();
	~EntriesBlock();

	int getCount();
	int addEntry(const char *entry);
	const char *getEntry(int entryIndex);
	unsigned long getEntrySize(int entryIndex);
	void removeEntry(int entryIndex);
	const char *getRawData(unsigned long *size);
};


#endif
