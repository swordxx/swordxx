#include <entriesblk.h>
#include <iostream.h>
#include <string>
#include <stdio.h>


void addEntry(EntriesBlock *eb) {
	string input;
	string body;
	char line[1024];
	cout << "\nEnter new Entry's text. '.' on an empty line to finish:\n";
	do {
		cout << "> ";
		gets(line);
		input = line;
		if (input.compare("."))
			body.append(input);
	}
	while (input.compare("."));
	cout << "Adding new entry.  Index is: " << eb->addEntry(body.c_str()) << "\n\n";
}


void printEntry(EntriesBlock *eb, int index) {
	if (index < eb->getCount()) {
		cout << "Contents of entry [" << index << "]:\n";
		cout << eb->getEntry(index) << "\n";
	}
	else cout << "Invalid entry number\n\n";
}


void removeEntry(EntriesBlock *eb, int index) {
	if (index < eb->getCount()) {
		cout << "Removing entry [" << index << "]\n";
	}
	else cout << "Invalid entry number\n\n";
}


int main(int argc, char **argv) {

	EntriesBlock *eb = new EntriesBlock();
	string input;
	char line[1024];

	cout << "Initial entry count should be 0: " << eb->getCount() << "\n";

	do {
		cout << "[" << eb->getCount() << "] > ";
		gets(line);
		input = line;
		if (input.length() > 0) {
			switch (input[0]) {
				case 'a': addEntry(eb); break;
				case 'p':	printEntry(eb, atoi(input.c_str()+1)); break;
				case 'r':	removeEntry(eb, atoi(input.c_str()+1)); break;
				case 'q': break;
				case '?':
				default:
					cout << "\n a - add a new entry\n";
					cout << " p <entry_index> - print entry\n";
					cout << " r <entry_index> - remove entry\n";
					cout << " q - quit\n\n";
					break;
			}
		}
	}
	while (input.compare("q"));

	delete eb;

	return 0;
}
