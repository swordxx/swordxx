#include <entriesblk.h>
#include <stdlib.h>
#include <string.h>

EntriesBlock::EntriesBlock(const char *iBlock, long size) {
	block = (char *)calloc(1, size);
	memcpy(block, iBlock, size);
}


EntriesBlock::EntriesBlock() {
	block = (char *)calloc(1, sizeof(__u32));
}


EntriesBlock::~EntriesBlock() {
	free(block);
}


void EntriesBlock::setCount(int count) {
	__u32 rawCount = archtosword32(count);
	memcpy(block, &rawCount, sizeof(__u32));
}


int EntriesBlock::getCount() {
	__u32 count = 0;
	memcpy(&count, block, sizeof(__u32));
	count = swordtoarch32(count);
	return count;
}


void EntriesBlock::getMetaEntry(int index, long *offset, short *size) {
	__u32 rawOffset = 0;
	__u32 rawSize = 0;
	*offset = 0;
	*size = 0;
	if (index >= getCount())	// assert index < count
		return;

	// first 4 bytes is count, each 6 bytes after is each meta entry
	memcpy(&rawOffset, block + METAHEADERSIZE + (index * METAENTRYSIZE), sizeof(rawOffset));
	memcpy(&rawSize, block + METAHEADERSIZE + (index * METAENTRYSIZE) + sizeof(rawOffset), sizeof(rawSize));

	*offset = (long)swordtoarch32(rawOffset);
	*size   = (short)swordtoarch32(rawSize);
}


void EntriesBlock::setMetaEntry(int index, long offset, short size) {
	__u32 rawOffset = archtosword32(offset);
	__u32 rawSize = archtosword32(size);

	if (index >= getCount())	// assert index < count
		return;

	// first 4 bytes is count, each 6 bytes after is each meta entry
	memcpy(block + METAHEADERSIZE + (index * METAENTRYSIZE), &rawOffset, sizeof(rawOffset));
	memcpy(block + METAHEADERSIZE + (index * METAENTRYSIZE) + sizeof(rawOffset), &rawSize, sizeof(rawSize));
}


const char *EntriesBlock::getRawData(long *retSize) {
	long max = 4;
	int loop;
	long offset;
	short size;
	for (loop = 0; loop < getCount(); loop++) {
		getMetaEntry(loop, &offset, &size);
		max = ((offset + size) > max) ? (offset + size) : max;
	}
	*retSize = max;
	return block;
}


int EntriesBlock::addEntry(const char *entry) {
	long dataSize;
	getRawData(&dataSize);
	int len = strlen(entry);
	long offset;
	short size;
	int count = getCount();
	long dataStart = METAHEADERSIZE + (count * METAENTRYSIZE);
	// new meta entry + new data size + 1 because null 
	block = (char *)realloc(block, dataSize + METAENTRYSIZE + len + 1);
	// shift right to make room for new meta entry
	memmove(block + dataStart + METAENTRYSIZE, block + dataStart, dataSize - dataStart);

	for (int loop = 0; loop < count; loop++) {
		getMetaEntry(loop, &offset, &size);
		if (offset) {	// if not a deleted entry
			offset += METAENTRYSIZE;
			setMetaEntry(loop, offset, size);
		}
	}

	offset = dataSize;	// original dataSize before realloc
	size = len + 1;
	// add our text to the end
	memcpy(block + offset + METAENTRYSIZE, entry, size);
	// increment count
	setCount(count + 1);
	// add our meta entry
	setMetaEntry(count, offset + METAENTRYSIZE, size);
	// return index of our new entry
	return count;
}


const char *EntriesBlock::getEntry(int entryIndex) {
	long offset;
	short size;
	static char *empty = "";
	getMetaEntry(entryIndex, &offset, &size);
	return (offset) ? block+offset : empty;
}


void EntriesBlock::removeEntry(int entryIndex) {
	long offset;
	short size, size2;
	long dataSize;
	getRawData(&dataSize);
	getMetaEntry(entryIndex, &offset, &size);
	int len = size - 1;
	int count = getCount();
	long dataStart = METAHEADERSIZE + (count * METAENTRYSIZE);

	if (!offset)	// already deleted
		return;

	// shift left to retrieve space used for old entry
	memmove(block + offset, block + offset + size, dataSize - (offset + size));

	// fix offset for all entries after our entry that were shifted left
	for (int loop = entryIndex + 1; loop < count; loop++) {
		getMetaEntry(loop, &offset, &size2);
		if (offset) {	// if not a deleted entry
			offset -= size;
			setMetaEntry(loop, offset, size2);
		}
	}

	// zero out our meta entry
	setMetaEntry(entryIndex, 0L, 0);
}


