/******************************************************************************
 *
 *  treekeyidx.cpp -
 *
 * $Id$
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


#include "treekeyidx.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <string>
#include "../filemgr.h"
#include "../swlog.h"
#include "../sysdata.h"
#include "../utilstr.h"


namespace swordxx {

TreeKeyIdx::TreeKeyIdx(const TreeKeyIdx &ikey) : currentNode() {
    path = nullptr;
    idxfd = nullptr;
    datfd = nullptr;
    copyFrom(ikey);
}

TreeKeyIdx::TreeKeyIdx(const char *idxPath, int fileMode) : currentNode() {
    std::string buf;

    path = nullptr;
    stdstr(&path, idxPath);

    if (fileMode == -1) { // try read/write if possible
        fileMode = FileMgr::RDWR;
    }

    buf = path;
    buf += ".idx";
    idxfd = FileMgr::getSystemFileMgr()->open(buf.c_str(), fileMode, true);
    buf.resize(buf.size() - 4u);
    buf += ".dat";
    datfd = FileMgr::getSystemFileMgr()->open(buf.c_str(), fileMode, true);

    if (!datfd) {
        SWLog::getSystemLog()->logError("%d", errno);
        error = errno;
    }
    else {
        root();
    }
}


TreeKeyIdx::~TreeKeyIdx () {
    delete[] path;

    FileMgr::getSystemFileMgr()->close(idxfd);
    FileMgr::getSystemFileMgr()->close(datfd);
}


const char *TreeKeyIdx::getLocalName() {
    unsnappedKeyText = "";
    return currentNode.name;
}


const char *TreeKeyIdx::getUserData(int *size) const {
    unsnappedKeyText = "";
    if (size)
        *size = (int)currentNode.dsize;
    return currentNode.userData;
}


void TreeKeyIdx::setUserData(const char *userData, int size) {
    // this makes sure any unsnapped path exists
    assureKeyPath();
    delete currentNode.userData;

    if (!size)
        size = strlen(userData) + 1;

    currentNode.userData = new char [ size ];
    memcpy(currentNode.userData, userData, size);
    currentNode.dsize = size;
}

const char *TreeKeyIdx::setLocalName(const char *newName) {
    unsnappedKeyText = "";
    stdstr(&(currentNode.name), newName);
    return currentNode.name;
}


void TreeKeyIdx::save() {
    saveTreeNode(&currentNode);
}


void TreeKeyIdx::root() {
    error = getTreeNodeFromIdxOffset(0, &currentNode);
    positionChanged();
}

int TreeKeyIdx::getLevel() {
    TreeNode iterator;
    iterator.parent = currentNode.parent;
    int level = 0;
    while (iterator.parent > -1) {
        level++;
        getTreeNodeFromIdxOffset(iterator.parent, &iterator);
    }
    return level;
}


bool TreeKeyIdx::parent() {
    if (currentNode.parent > -1) {
        error = getTreeNodeFromIdxOffset(currentNode.parent, &currentNode);
        positionChanged();
        return true;
    }
    return false;
}


bool TreeKeyIdx::firstChild() {
    if (currentNode.firstChild > -1) {
        error = getTreeNodeFromIdxOffset(currentNode.firstChild, &currentNode);
        positionChanged();
        return true;
    }
    return false;
}


bool TreeKeyIdx::nextSibling() {
    if (currentNode.next > -1) {
        error = getTreeNodeFromIdxOffset(currentNode.next, &currentNode);
        positionChanged();
        return true;
    }
    return false;
}


bool TreeKeyIdx::previousSibling() {
    TreeNode iterator;
    int32_t target = currentNode.offset;
    if (currentNode.parent > -1) {
        getTreeNodeFromIdxOffset(currentNode.parent, &iterator);
        getTreeNodeFromIdxOffset(iterator.firstChild, &iterator);
        if (iterator.offset != target) {
            while ((iterator.next != target) && (iterator.next > -1))
                getTreeNodeFromIdxOffset(iterator.next, &iterator);
            if (iterator.next > -1) {
                error = getTreeNodeFromIdxOffset(iterator.offset, &currentNode);
                positionChanged();
                return true;
            }
        }
    }
    return false;
}


bool TreeKeyIdx::hasChildren() {
    return (currentNode.firstChild > -1);
}


void TreeKeyIdx::append() {
    TreeNode lastSib;
    if (currentNode.offset) {
        getTreeNodeFromIdxOffset(currentNode.offset, &lastSib);
        while (lastSib.next > -1) {
            getTreeNodeFromIdxOffset(lastSib.next, &lastSib);
        }
        uint32_t idxOffset = idxfd->seek(0, SEEK_END);
        lastSib.next = idxOffset;
        saveTreeNodeOffsets(&lastSib);
        uint32_t parent = currentNode.parent;
        currentNode.clear();
        currentNode.offset = idxOffset;
        currentNode.parent = parent;
        positionChanged();
    }
}


void TreeKeyIdx::appendChild() {
    if (firstChild()) {
        append();
    }
    else {
        uint32_t idxOffset = idxfd->seek(0, SEEK_END);
        currentNode.firstChild = idxOffset;
        saveTreeNodeOffsets(&currentNode);
        uint32_t parent = currentNode.offset;
        currentNode.clear();
        currentNode.offset = idxOffset;
        currentNode.parent = parent;
    }
    positionChanged();
}


void TreeKeyIdx::insertBefore() {
}


void TreeKeyIdx::remove() {
    TreeNode node;
    bool done = false;
    if (currentNode.offset) {
        getTreeNodeFromIdxOffset(currentNode.offset, &node);
        if (node.parent > -1) {
            TreeNode parent;
            getTreeNodeFromIdxOffset(node.parent, &parent);
            if (parent.firstChild == node.offset) {
                parent.firstChild = node.next;
                saveTreeNodeOffsets(&parent);
                getTreeNodeFromIdxOffset(parent.offset, &currentNode);
                done = true;
            }
        }
        if (!done) {
            TreeNode iterator;
            int32_t target = currentNode.offset;
            if (currentNode.parent > -1) {
                getTreeNodeFromIdxOffset(currentNode.parent, &iterator);
                getTreeNodeFromIdxOffset(iterator.firstChild, &iterator);
                if (iterator.offset != target) {
                    while ((iterator.next != target) && (iterator.next > -1)) {
                        getTreeNodeFromIdxOffset(iterator.next, &iterator);
                    }
                    if (iterator.next > -1) {
                        TreeNode prev;
                        getTreeNodeFromIdxOffset(iterator.offset, &prev);
                        prev.next = node.next;
                        saveTreeNodeOffsets(&prev);
                        getTreeNodeFromIdxOffset(prev.offset, &currentNode);
                    }
                }
            }
        }
        positionChanged();
    }
}


/******************************************************************************
 * TreeKeyIdx::Create    - Creates new key idx/dat files
 *
 * ENT: path    - directory to store module files
 * RET: error status
 */

signed char TreeKeyIdx::create(const char *ipath) {
    char * path = nullptr;
    char *buf = new char [ strlen (ipath) + 20 ];
    FileDesc *fd, *fd2;

    stdstr(&path, ipath);

    if ((path[strlen(path)-1] == '/') || (path[strlen(path)-1] == '\\'))
        path[strlen(path)-1] = 0;

    sprintf(buf, "%s.dat", path);
    FileMgr::removeFile(buf);
    fd = FileMgr::getSystemFileMgr()->open(buf, FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    fd->getFd();
    FileMgr::getSystemFileMgr()->close(fd);

    sprintf(buf, "%s.idx", path);
    FileMgr::removeFile(buf);
    fd2 = FileMgr::getSystemFileMgr()->open(buf, FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    fd2->getFd();
    FileMgr::getSystemFileMgr()->close(fd2);

    TreeKeyIdx newTree(path);
    TreeKeyIdx::TreeNode root;
    stdstr(&(root.name), "");
    newTree.saveTreeNode(&root);

    delete [] path;

    return 0;
}


/******************************************************************************
 * zStr::getidxbufdat    - Gets the index string at the given dat offset
 *                NOTE: buf is calloc'd, or if not null, realloc'd and must
 *                    be free'd by calling function
 *
 * ENT:    ioffset    - offset in dat file to lookup
 *        node        - address of pointer to allocate for storage of string
 */

void TreeKeyIdx::getTreeNodeFromDatOffset(long ioffset, TreeNode *node) const {
    unsnappedKeyText = "";
    char ch;
    int32_t  tmp;
    uint16_t  tmp2;

    if (datfd) {

        datfd->seek(ioffset, SEEK_SET);

        datfd->read(&tmp, 4);
        node->parent = swordtoarch32(tmp);

        datfd->read(&tmp, 4);
        node->next = swordtoarch32(tmp);

        datfd->read(&tmp, 4);
        node->firstChild = swordtoarch32(tmp);

        std::string name;
        do {
            datfd->read(&ch, 1);
            name += ch;
        } while (ch);

        stdstr(&(node->name), name.c_str());

        datfd->read(&tmp2, 2);
        node->dsize = swordtoarch16(tmp2);

        if (node->dsize) {
            delete[] node->userData;
            node->userData = new char [node->dsize];
            datfd->read(node->userData, node->dsize);
        }
    }
}


/******************************************************************************
 * zStr::getidxbuf    - Gets the index string at the given idx offset
 *                        NOTE: buf is calloc'd, or if not null, realloc'd
 *                            and must be freed by calling function
 *
 * ENT:    ioffset    - offset in idx file to lookup
 *        buf        - address of pointer to allocate for storage of string
 */

char TreeKeyIdx::getTreeNodeFromIdxOffset(long ioffset, TreeNode *node) const {
    unsnappedKeyText = "";
    uint32_t offset;
    char error = KEYERR_OUTOFBOUNDS;

    if (ioffset < 0) {
        ioffset = 0;
        error = 77;    // out of bounds but still position to 0;
    }

    node->offset = ioffset;
    if (idxfd) {
        if (idxfd->getFd() > 0) {
            idxfd->seek(ioffset, SEEK_SET);
            if (idxfd->read(&offset, 4) == 4) {
                offset = swordtoarch32(offset);
                error = (error == 77) ? KEYERR_OUTOFBOUNDS : 0;
                getTreeNodeFromDatOffset(offset, node);
            }
            else {
                idxfd->seek(-4, SEEK_END);
                if (idxfd->read(&offset, 4) == 4) {
                    offset = swordtoarch32(offset);
                    getTreeNodeFromDatOffset(offset, node);
                }
            }
        }
    }
    return error;
}


unsigned long TreeKeyIdx::getOffset() const {
    unsnappedKeyText = "";
    return currentNode.offset;
}

void TreeKeyIdx::setOffset(unsigned long offset) {
    error = getTreeNodeFromIdxOffset(offset, &currentNode);
    positionChanged();
}


void TreeKeyIdx::saveTreeNodeOffsets(TreeNode *node) {
    unsnappedKeyText = "";
    long datOffset = 0;
    int32_t tmp;

    if (idxfd) {
        idxfd->seek(node->offset, SEEK_SET);
        if (idxfd->read(&tmp, 4) != 4) {
            datOffset = datfd->seek(0, SEEK_END);
            tmp = archtosword32(datOffset);
            idxfd->write(&tmp, 4);
        }
        else {
            datOffset = swordtoarch32(tmp);
            datfd->seek(datOffset, SEEK_SET);
        }

        tmp = archtosword32(node->parent);
        datfd->write(&tmp, 4);

        tmp = archtosword32(node->next);
        datfd->write(&tmp, 4);

        tmp = archtosword32(node->firstChild);
        datfd->write(&tmp, 4);
    }
}


void TreeKeyIdx::copyFrom(const TreeKeyIdx &ikey) {
    unsnappedKeyText = "";

    SWKey::copyFrom(ikey);

    currentNode.offset = ikey.currentNode.offset;
    currentNode.parent = ikey.currentNode.parent;
    currentNode.next = ikey.currentNode.next;
    currentNode.firstChild = ikey.currentNode.firstChild;
    stdstr(&(currentNode.name), ikey.currentNode.name);
    currentNode.dsize = ikey.currentNode.dsize;

    delete[] currentNode.userData;
    if (currentNode.dsize) {
        currentNode.userData = new char [ currentNode.dsize ];
        memcpy(currentNode.userData, ikey.currentNode.userData, currentNode.dsize);
    }
    else currentNode.userData = nullptr;

    bool newFiles = true;

    if (path && ikey.path)
        newFiles = strcmp(path, ikey.path);

    if (newFiles) {
        stdstr(&path, ikey.path);

        if (idxfd) {
            FileMgr::getSystemFileMgr()->close(idxfd);
            FileMgr::getSystemFileMgr()->close(datfd);
        }
        idxfd = FileMgr::getSystemFileMgr()->open(ikey.idxfd->path, ikey.idxfd->mode, ikey.idxfd->perms);
        datfd = FileMgr::getSystemFileMgr()->open(ikey.datfd->path, ikey.datfd->mode, ikey.datfd->perms);
    }
    positionChanged();
}


void TreeKeyIdx::saveTreeNode(TreeNode *node) {
    long datOffset = 0;
    int32_t tmp;
    if (idxfd) {
        idxfd->seek(node->offset, SEEK_SET);
        datOffset = datfd->seek(0, SEEK_END);
        tmp = archtosword32(datOffset);
        idxfd->write(&tmp, 4);

        saveTreeNodeOffsets(node);

        datfd->write(node->name, strlen(node->name));
        char null = 0;
        datfd->write(&null, 1);

        uint16_t tmp2 = archtosword16(node->dsize);
        datfd->write(&tmp2, 2);

        if (node->dsize) {
            datfd->write(node->userData, node->dsize);
        }
    }
}


void TreeKeyIdx::setText(const char *ikey) {
    char * buf = nullptr;
    stdstr(&buf, ikey);
    static auto const slashTokenOrEmpty =
            [](char * const str) noexcept {
                auto const nextToken(std::strtok(str, "/"));
                return nextToken ? nextToken : "";
            };
    std::string leaf(slashTokenOrEmpty(buf));
    trimString(leaf);
    root();
    while ((leaf.size()) && (!popError())) {
        bool ok, inChild = false;
        error = KEYERR_OUTOFBOUNDS;
        for (ok = firstChild(); ok; ok = nextSibling()) {
            inChild = true;
            if (leaf == getLocalName()) {
                error = 0;
                break;
            }
        }
        leaf = slashTokenOrEmpty(nullptr);
        trimString(leaf);
        if (!ok) {
                if (inChild) {    // if we didn't find a matching child node, default to first child
                parent();
                firstChild();
            }
            error = KEYERR_OUTOFBOUNDS;
        }
    }
    if (leaf.size())
        error = KEYERR_OUTOFBOUNDS;
    delete [] buf;
    unsnappedKeyText = ikey;
    positionChanged();
}



void TreeKeyIdx::copyFrom(const SWKey &ikey) {
    unsnappedKeyText = ikey.getText();
    SWKey::copyFrom(ikey);
    positionChanged();
}

void TreeKeyIdx::setPosition(SW_POSITION p) {
    switch (p) {
    case TOP:
        root();
        break;
    case BOTTOM:
        error = getTreeNodeFromIdxOffset(idxfd->seek(-4, SEEK_END), &currentNode);
        break;
    }
    positionChanged();
    popError();    // clear error from normalize
}


int TreeKeyIdx::_compare(TreeKeyIdx const & ikey) const noexcept
{ return (getOffset() - ikey.getOffset()); }

int TreeKeyIdx::compare(SWKey const & ikey) const noexcept {
    if (TreeKeyIdx const * const treeKey =
            dynamic_cast<TreeKeyIdx const *>(&ikey))
        return _compare(*treeKey);
    return SWKey::compare(ikey);
}


void TreeKeyIdx::decrement(int steps) {
    error = getTreeNodeFromIdxOffset(currentNode.offset - (4*steps), &currentNode);
    positionChanged();
}

void TreeKeyIdx::increment(int steps) {
    error = getTreeNodeFromIdxOffset(currentNode.offset + (4*steps), &currentNode);
    if (error) {
//        SWLog::getSystemLog()->logError("error: %d", error);
    }
    positionChanged();

/*
    // assert positive
    if (steps < 0) {
        decrement(steps * -1);
        return;
    }

    while (steps > 0) {
        if (!firstChild()) {
            if (!nextSibbling() {
                error = KEYERR_OUTOFBOUNDS;
                return;
            }
        }
        steps--;
    }
*/
}



const char *TreeKeyIdx::getText() const {
    TreeNode parent;
    static std::string fullPath;
    fullPath = currentNode.name;
    parent.parent = currentNode.parent;
    while (parent.parent > -1) {
        getTreeNodeFromIdxOffset(parent.parent, &parent);
        fullPath = std::string(parent.name) + '/' + fullPath;
    }
    // we've snapped; clear our unsnapped text holder
    unsnappedKeyText = "";
    return fullPath.c_str();
}


TreeKeyIdx::TreeNode::TreeNode() {

    name       = nullptr;
    stdstr(&name, "");
    userData   = nullptr;

    clear();
}


void TreeKeyIdx::TreeNode::clear() {
    offset     = 0;
    parent     = -1;
    next       = -1;
    firstChild = -1;
    dsize      = 0;

    delete[] name;
    name = nullptr;
    stdstr(&name, "");

    delete[] userData;
    userData   = nullptr;
}


TreeKeyIdx::TreeNode::~TreeNode() {
    delete[] name;
    delete[] userData;
}


SWKey *TreeKeyIdx::clone() const
{
    return new TreeKeyIdx(*this);
}

} /* namespace swordxx */
