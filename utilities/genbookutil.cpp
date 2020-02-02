/******************************************************************************
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#include <cstdio>
#include <iostream>
#include <swordxx/keys/treekeyidx.h>
#include <swordxx/modules/common/entriesblk.h>
#include <swordxx/modules/genbook/rawgenbook.h>
#include <swordxx/utilstr.h>


using namespace swordxx;

void printTree(TreeKeyIdx treeKey,
               TreeKeyIdx & target,
               int level = 1)
{
    unsigned long currentOffset = target.getOffset();
    std::cout << ((currentOffset == treeKey.getOffset()) ? "==>" : "");
    for (int i = 0; i < level; i++) std::cout << "\t";
    std::cout << treeKey.getLocalName() << "/\n";
    if (treeKey.firstChild()) {
        printTree(treeKey, target, level+1);
        treeKey.parent();
    }
    if (treeKey.nextSibling())
        printTree(treeKey, target, level);

}


void printLocalName(TreeKeyIdx & treeKey) {
    std::cout << "locaName: " << treeKey.getLocalName() << "\n";
}


void setLocalName(TreeKeyIdx & treeKey) {
    char buf[1023];
    std::cout << "Enter New Node Name: ";
    fgets(buf, 1000, stdin);
    std::string name = buf;
    trimString(name);
    treeKey.setLocalName(std::move(name));
    treeKey.save();
}


void gotoPath(TreeKeyIdx & treeKey) {
    char buf[1023];
    std::cout << "Enter Path: ";
    fgets(buf, 1000, stdin);
    std::string path = buf;
    trimString(path);
    treeKey.setText(path.c_str());
}


void assurePath(TreeKeyIdx & treeKey) {
    char buf[1023];
    std::cout << "Enter Path: ";
    fgets(buf, 1000, stdin);
    std::string path = buf;
    trimString(path);
    treeKey.assureKeyPath(path.c_str());
}


void viewEntryText(RawGenBook const & book) {
    std::cout << "\n";
    std::cout << book.renderText();
    std::cout << "\n";
}


void setEntryText(RawGenBook & book) {
    std::string body;
    auto const treeKey = book.getKeyAs<TreeKeyIdx>();
    if (treeKey->getOffset()) {
        char buf[1023];
        std::cout << "Enter New Entry Text ('.' on a line by itself to end): \n";
        do {
            fgets(buf, 1000, stdin);
            std::string text = buf;
            trimString(text);
            if ((text[0] == '.') && (text[1] == 0))
                break;
            body += text;
            body += "\n";
        } while (true);

        book.setEntry(body);
    }
    else    std::cout << "Can't add entry text to root node\n";
}


void appendSibbling(TreeKeyIdx & treeKey) {
    if (treeKey.getOffset()) {
        char buf[1023];
        std::cout << "Enter New Sibbling Name: ";
        fgets(buf, 1000, stdin);
        std::string name = buf;
        trimString(name);
        treeKey.append();
        treeKey.setLocalName(std::move(name));
        treeKey.save();
    }
    else    std::cout << "Can't add sibling to root node\n";
}


void appendChild(TreeKeyIdx & treeKey) {
    char buf[1023];
    std::cout << "Enter New Child Name: ";
    fgets(buf, 1000, stdin);
    std::string name = buf;
    trimString(name);
    treeKey.appendChild();
    treeKey.setLocalName(std::move(name));
    treeKey.save();
}


void deleteNode(TreeKeyIdx & treeKey) {
    std::cout << "Removing entry [" << treeKey.getText() << "]\n";
    treeKey.remove();
}

int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "usage: %s <tree/key/data/path>\n", *argv);
        std::exit(-1);
    }

    {
        TreeKeyIdx treeKey(argv[1]);
        if (treeKey.popError())
            RawGenBook::createModule(argv[1]);
    }

    RawGenBook book(argv[1]);
    auto const treeKey = book.getKeyAs<TreeKeyIdx>();
    auto const root = treeKey;

    std::string input;
    char line[1024];

    do {
        std::cout << "[" << treeKey->getText() << "] > ";
        fgets(line, 1000, stdin);
        input = line;
        trimString(input);
        if (input.length() > 0) {
            switch (input[0]) {
                case 'n': printLocalName(*treeKey); break;
                case 's': setLocalName(*treeKey); break;
                case 'g': gotoPath(*treeKey); break;
                case 'G': assurePath(*treeKey); break;
                case 'p':    root->root(); printTree(*root, *treeKey); break;
                case 'a':    appendSibbling(*treeKey); break;
                case 'c':    appendChild(*treeKey); break;
                case 'd':    deleteNode(*treeKey); break;
                case 'j':    treeKey->nextSibling(); break;
                case 'k':    treeKey->previousSibling(); break;
                case 'h':    treeKey->parent(); break;
                case 'l':    treeKey->firstChild(); break;
                case 'r':    treeKey->root(); break;
                case 't':    setEntryText(book); break;
                case 'v':    viewEntryText(book); break;
                case 'q': break;
                case '?':
                default:
                    std::cout << "\n p - print tree\n";
                    std::cout << " n - get local name\n";
                    std::cout << " s - set local name\n";
                    std::cout << " j - next sibbling\n";
                    std::cout << " k - previous sibbling\n";
                    std::cout << " h - parent\n";
                    std::cout << " l - first child\n";
                    std::cout << " r - root\n";
                    std::cout << " g - goto path\n";
                    std::cout << " G   goto path; create if it doesn't exist\n";
                    std::cout << " a - append sibbling\n";
                    std::cout << " c - append child\n";
                    std::cout << " d - delete node\n";
                    std::cout << " v - view entry text\n";
                    std::cout << " t - set entry text\n";
                    std::cout << " q - quit\n\n";
                    break;
            }
        }
    }
    while (input.compare("q"));
}
