#include <entriesblk.h>
#include <stdlib.h>
#include <string.h>

EntriesBlock::EntriesBlock(const char *iBlock, long size) {
	block = (char *)calloc(sizeof(char), size);
	memcpy(block, iBlock, size);
}


EntriesBlock::EntriesBlock() {
	block = (char *)calloc(sizeof(char), 4);
}


EntriesBlock::~EntriesBlock() {
	free(block);
}


void EntriesBlock::setCount(int count) {
	short rawCount=count;
	memcpy(block, &rawCount, sizeof(short));
}


int EntriesBlock::getCount() {
	short count=0;
	memcpy(&count, block, sizeof(short));
	return count;
}


void EntriesBlock::getMetaEntry(int index, long *offset, short *size) {
	*offset = 0;
	*size = 0;
	if (index >= getCount())	// assert index < count
		return;

	// first 4 bytes is count, each 6 bytes after is each meta entry
	memcpy(offset, block+4+(index*6), 4);
	memcpy(size, block+4+(index*6)+4, 2);
}


void EntriesBlock::setMetaEntry(int index, long offset, short size) {
	if (index >= getCount())	// assert index < count
		return;

	// first 4 bytes is count, each 6 bytes after is each meta entry
	memcpy(block+4+(index*6), &offset, 4);
	memcpy(block+4+(index*6)+4, &size, 2);
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
	long dataStart = 4 + (count*6);
	// +7 because null + new 6 byte meta entry
	block = (char *)realloc(block, dataSize + len + 7);
	// shift right to make room for new meta entry
	memmove(block+dataStart+6, block+dataStart, dataSize-dataStart);
	for (int loop = 0; loop < count; loop++) {
		getMetaEntry(loop, &offset, &size);
		if (offset) {	// if not a deleted entry
			offset+=6;
			setMetaEntry(loop, offset, size);
		}
	}

	offset = dataSize;	// original dataSize before realloc
	size = len + 1;
	// add our text to the end
	memcpy(block+offset+6, entry, size);
	// increment count
	setCount(count+1);
	// add our meta entry
	setMetaEntry(count, offset+6, size);
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
	short size;
	getMetaEntry(entryIndex, &offset, &size);

// addEntry recipricate this
/*
	long dataSize;
	getRawData(&dataSize);
	int len = strlen(entry);
	long offset;
	short size;
	int count = getCount();
	long dataStart = 4 + (count*6);
	// +7 because null + new 6 byte meta entry
	block = (char *)realloc(block, sizeof(char), dataSize + len + 7);
	// shift right to make room for new meta entry
	memmove(block+dataStart+6, block+dataStart, dataSize-dataStart);
	for (int loop = 0; loop < count; loop++) {
		getMetaEntry(loop, &offset, &size);
		if (offset) {	// if not a deleted entry
			offset+=6;
			setMetaEntry(loop, offset, size);
		}
	}

	offset = dataSize;	// original dataSize before realloc
	size = len + 1;
	// add our text to the end
	memcpy(block+offset, entry, size);
	// increment count
	setCount(count+1);
	// add our meta entry
	setMetaEntry(count, offset, size);
	// return index of our new entry
	return count;
*/
	
}


