#include <entriesblk.h>
#include <iostream.h>
#include <string>
#include <stdio.h>
#include <treekeyidx.h>
#include <rawgenbook.h>


void printTree(TreeKeyIdx treeKey, TreeKeyIdx *target = 0, int level = 1) {
	if (!target)
		target = &treeKey;
	
	unsigned long currentOffset = target->getOffset();
	cout << ((currentOffset == treeKey.getOffset()) ? "==>" : "");
	for (int i = 0; i < level; i++) cout << "\t";
	cout << treeKey.getLocalName() << "/\n";
	if (treeKey.firstChild()) {
		printTree(treeKey, target, level+1);
		treeKey.parent();
	}
	if (treeKey.nextSibling())
		printTree(treeKey, target, level);

}


void printLocalName(TreeKeyIdx *treeKey) {
	cout << "locaName: " << treeKey->getLocalName() << "\n";
}


void setLocalName(TreeKeyIdx *treeKey) {
	char buf[1023];
	cout << "Enter New Node Name: ";
	gets(buf);
	treeKey->setLocalName(buf);
	treeKey->save();
}


void gotoPath(TreeKeyIdx *treeKey) {
	char buf[1023];
	cout << "Enter Path: ";
	gets(buf);
	(*treeKey) = buf;
}


void viewEntryText(RawGenBook *book) {
	cout << "\n";
	cout << book->RenderText();
	cout << "\n";
}


void setEntryText(RawGenBook *book) {
	string body;
	TreeKeyIdx *treeKey = (TreeKeyIdx *)(SWKey *)(*book);
	if (treeKey->getOffset()) {
		char buf[1023];
		cout << "Enter New Entry Text ('.' on a line by itself to end): \n";
		do {
			gets(buf);
			if ((buf[0] == '.') && (buf[1] == 0))
				break;
			body += buf;
			body += "\n";
		} while (true);

		(*book) << body.c_str();
	}
	else	cout << "Can't add entry text to root node\n";
}


void appendSibbling(TreeKeyIdx *treeKey) {
	if (treeKey->getOffset()) {
		char buf[1023];
		cout << "Enter New Sibbling Name: ";
		gets(buf);
		treeKey->append();
		treeKey->setLocalName(buf);
		treeKey->save();
	}
	else	cout << "Can't add sibling to root node\n";
}


void appendChild(TreeKeyIdx *treeKey) {
	char buf[1023];
	cout << "Enter New Child Name: ";
	gets(buf);
	treeKey->appendChild();
	treeKey->setLocalName(buf);
	treeKey->save();
}


void removeEntry(EntriesBlock *eb, int index) {
	if (index < eb->getCount()) {
		cout << "Removing entry [" << index << "]\n";
		eb->removeEntry(index);
	}
	else cout << "Invalid entry number\n\n";
}


int main(int argc, char **argv) {

	if (argc != 2) {
		fprintf(stderr, "usage: %s <tree/key/data/path>\n", *argv);
		exit(-1);
	}

	TreeKeyIdx *treeKey = new TreeKeyIdx(argv[1]);

	if (treeKey->Error()) {
		RawGenBook::createModule(argv[1]);
	}
	delete treeKey;

	RawGenBook *book = new RawGenBook(argv[1]);
	TreeKeyIdx root = *((TreeKeyIdx *)((SWKey *)(*book)));
	treeKey = (TreeKeyIdx *)(SWKey *)(*book);

	string input;
	char line[1024];

	do {
		cout << "[" << treeKey->getFullName() << "] > ";
		gets(line);
		input = line;
		if (input.length() > 0) {
			switch (input[0]) {
				case 'n': printLocalName(treeKey); break;
				case 's': setLocalName(treeKey); break;
				case 'g': gotoPath(treeKey); break;
				case 'p':	root.root(); printTree(root, treeKey); break;
				case 'a':	appendSibbling(treeKey); break;
				case 'c':	appendChild(treeKey); break;
				case 'j':	treeKey->nextSibling(); break;
				case 'k':	treeKey->previousSibling(); break;
				case 'h':	treeKey->parent(); break;
				case 'l':	treeKey->firstChild(); break;
				case 'r':	treeKey->root(); break;
				case 't':	setEntryText(book); break;
				case 'v':	viewEntryText(book); break;
				case 'q': break;
				case '?':
				default:
					cout << "\n p - print tree\n";
					cout << " n - get local name\n";
					cout << " s - set local name\n";
					cout << " j - next sibbling\n";
					cout << " k - previous sibbling\n";
					cout << " h - parent\n";
					cout << " l - first child\n";
					cout << " r - root\n";
					cout << " g - goto path\n";
					cout << " a - append sibbling\n";
					cout << " c - append child\n";
					cout << " v - view entry text\n";
					cout << " t - set entry text\n";
					cout << " q - quit\n\n";
					break;
			}
		}
	}
	while (input.compare("q"));

	delete treeKey;

	return 0;
}
