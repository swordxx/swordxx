#ifndef ENTRIESBLK_H
#define ENTRIESBLK_H

class EntriesBlock {
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
	void deleteEntry(int entryIndex);
	const char *getRawData(long *size);
};


#endif
