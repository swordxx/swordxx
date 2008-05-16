#include <entriesblk.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

void addEntry(EntriesBlock *eb) {
	string input;
	string body;
	char line[1024];
	std::cout << "\nEnter new Entry's text. '.' on an empty line to finish:\n";
	do {
		std::cout << "> ";
		fgets(line, 1000, stdin);
		input = line;
		if (input.compare("."))
			body.append(input);
	}
	while (input.compare("."));
	std::cout << "Adding new entry.  Index is: " << eb->addEntry(body.c_str()) << "\n\n";
}


void printEntry(EntriesBlock *eb, int index) {
	if (index < eb->getCount()) {
		std::cout << "Contents of entry [" << index << "]:\n";
		std::cout << eb->getEntry(index) << "\n";
	}
	else std::cout << "Invalid entry number\n\n";
}


void printSize(EntriesBlock *eb) {
	unsigned long size;
	eb->getRawData(&size);
	std::cout << "Size of raw data: " << size << "\n\n";
}


void removeEntry(EntriesBlock *eb, int index) {
	if (index < eb->getCount()) {
		std::cout << "Removing entry [" << index << "]\n";
		eb->removeEntry(index);
	}
	else std::cout << "Invalid entry number\n\n";
}


int main(int argc, char **argv) {

	EntriesBlock *eb = new EntriesBlock();
	string input;
	char line[1024];

	std::cout << "Initial entry count should be 0: " << eb->getCount() << "\n";

	do {
		std::cout << "[" << eb->getCount() << "] > ";
		fgets(line, 1000, stdin);
		input = line;
		if (input.length() > 0) {
			switch (input[0]) {
				case 'a': addEntry(eb); break;
				case 'p':	printEntry(eb, atoi(input.c_str()+1)); break;
				case 'r':	removeEntry(eb, atoi(input.c_str()+1)); break;
				case 's': printSize(eb); break;
				case 'q': break;
				case '?':
				default:
					std::cout << "\n a - add a new entry\n";
					std::cout << " p <entry_index> - print entry\n";
					std::cout << " r <entry_index> - remove entry\n";
					std::cout << " s - print size of raw data\n";
					std::cout << " q - quit\n\n";
					break;
			}
		}
	}
	while (input.compare("q"));

	delete eb;

	return 0;
}
