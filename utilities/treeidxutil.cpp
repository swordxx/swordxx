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
#include <memory>
#include <string>
#include <swordxx/keys/treekeyidx.h>


using namespace swordxx;


void printTree(TreeKeyIdx treeKey,
               TreeKeyIdx & target,
               int level = 1)
{
    unsigned long currentOffset = target.getOffset();
    std::cout << ((currentOffset == treeKey.getOffset()) ? "==>" : "");
    for (int i = 0; i < level; i++) std::cout << "\t";
    std::cout << treeKey.getLocalName() << std::endl;
    if (treeKey.firstChild()) {
        printTree(treeKey, target, level+1);
        treeKey.parent();
    }
    if (treeKey.nextSibling())
        printTree(treeKey, target, level);

}


void printLocalName(TreeKeyIdx & treeKey) {
    std::cout << "locaName: " << treeKey.getLocalName() << std::endl;
}


void setLocalName(TreeKeyIdx & treeKey) {
    char buf[1023];
    std::cout << "Enter New Node Name: ";
    fgets(buf, 1000, stdin);
    treeKey.setLocalName(buf);
    treeKey.save();
}


void assurePath(TreeKeyIdx & treeKey) {
    char buf[1023];
    std::cout << "Enter path: ";
    fgets(buf, 1000, stdin);
    treeKey.assureKeyPath(buf);
}


void appendSibbling(TreeKeyIdx & treeKey) {
    if (treeKey.getOffset()) {
        char buf[1023];
        std::cout << "Enter New Sibbling Name: ";
        fgets(buf, 1000, stdin);
        treeKey.append();
        treeKey.setLocalName(buf);
        treeKey.save();
    }
    else    std::cout << "Can't add sibling to root node\n";
}


void appendChild(TreeKeyIdx & treeKey) {
    char buf[1023];
    std::cout << "Enter New Child Name: ";
    fgets(buf, 1000, stdin);
    treeKey.appendChild();
    treeKey.setLocalName(buf);
    treeKey.save();
}

int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "usage: %s <tree/key/data/path>\n", *argv);
        std::exit(-1);
    }

    auto treeKey(std::make_unique<TreeKeyIdx>(argv[1]));

    if (treeKey->popError()) {
        treeKey->create(argv[1]);
        treeKey = std::make_unique<TreeKeyIdx>(argv[1]);
    }
    TreeKeyIdx root = *treeKey;

    std::string input;
    char line[1024];

    do {
        std::cout << "[" << treeKey->getText() << "] > ";
        fgets(line, 1000, stdin);
        input = line;
        if (input.length() > 0) {
            switch (input[0]) {
                case 'n': printLocalName(*treeKey); break;
                case 's': setLocalName(*treeKey); break;
                case 'p':    root.root(); printTree(root, *treeKey); break;
                case 'a':    appendSibbling(*treeKey); break;
                case 'c':    appendChild(*treeKey); break;
                case 'j':    treeKey->nextSibling(); break;
                case 'g': assurePath(*treeKey); break;
                case 'k':    treeKey->previousSibling(); break;
                case 'h':    treeKey->parent(); break;
                case 'l':    treeKey->firstChild(); break;
                case 'r':    treeKey->root(); break;
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
                    std::cout << " a - append sibbling\n";
                    std::cout << " c - append child\n";
                    std::cout << " u - get user data\n";
                    std::cout << " d - set user data\n";
                    std::cout << " g - goto path; create if it doesn't exist\n";
                    std::cout << " q - quit\n\n";
                    break;
            }
        }
    }
    while (input.compare("q"));
}
