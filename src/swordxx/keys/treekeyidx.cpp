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

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <string>
#include <system_error>
#include <utility>
#include "../filemgr.h"
#include "../swlog.h"
#include "../sysdata.h"
#include "../utilstr.h"
#include <iostream>

namespace swordxx {

TreeKeyIdx::TreeKeyIdx(const TreeKeyIdx &ikey) : m_currentNode() {
    m_path = nullptr;
    m_idxfd = nullptr;
    m_datfd = nullptr;
    copyFrom(ikey);
}

TreeKeyIdx::TreeKeyIdx(const char *idxPath, int fileMode) : m_currentNode() {
    std::string buf;

    m_path = nullptr;
    stdstr(&m_path, idxPath);

    if (fileMode == -1) { // try read/write if possible
        fileMode = FileMgr::RDWR;
    }

    buf = m_path;
    buf += ".idx";
    m_idxfd = FileMgr::getSystemFileMgr()->open(buf.c_str(), fileMode, true);
    buf.resize(buf.size() - 4u);
    buf += ".dat";
    m_datfd = FileMgr::getSystemFileMgr()->open(buf.c_str(), fileMode, true);

    if (!m_datfd) {
        SWLog::getSystemLog()->logError("%d", errno);
        m_error = errno;
    }
    else {
        root();
    }
}


TreeKeyIdx::~TreeKeyIdx () {
    delete[] m_path;

    FileMgr::getSystemFileMgr()->close(m_idxfd);
    FileMgr::getSystemFileMgr()->close(m_datfd);
}


std::string const & TreeKeyIdx::getLocalName() {
    m_unsnappedKeyText.clear();
    return m_currentNode.name;
}


const char *TreeKeyIdx::getUserData(int *size) const {
    m_unsnappedKeyText.clear();
    if (size)
        *size = (int)m_currentNode.dsize;
    return m_currentNode.userData;
}


void TreeKeyIdx::setUserData(const char *userData, int size) {
    // this makes sure any unsnapped path exists
    assureKeyPath();
    delete m_currentNode.userData;

    if (!size)
        size = strlen(userData) + 1;

    m_currentNode.userData = new char [ size ];
    memcpy(m_currentNode.userData, userData, size);
    m_currentNode.dsize = size;
}

void TreeKeyIdx::setLocalName(std::string const & newName) {
    m_unsnappedKeyText.clear();
    m_currentNode.name = newName;
}

void TreeKeyIdx::setLocalName(std::string && newName) {
    m_unsnappedKeyText.clear();
    m_currentNode.name = std::move(newName);
}


void TreeKeyIdx::save() {
    saveTreeNode(&m_currentNode);
}


void TreeKeyIdx::root() {
    m_error = getTreeNodeFromIdxOffset(0, &m_currentNode);
    positionChanged();
}

int TreeKeyIdx::getLevel() {
    TreeNode iterator;
    iterator.parent = m_currentNode.parent;
    int level = 0;
    while (iterator.parent > -1) {
        level++;
        getTreeNodeFromIdxOffset(iterator.parent, &iterator);
    }
    return level;
}


bool TreeKeyIdx::parent() {
    if (m_currentNode.parent > -1) {
        m_error = getTreeNodeFromIdxOffset(m_currentNode.parent, &m_currentNode);
        positionChanged();
        return true;
    }
    return false;
}


bool TreeKeyIdx::firstChild() {
    if (m_currentNode.firstChild > -1) {
        m_error = getTreeNodeFromIdxOffset(m_currentNode.firstChild, &m_currentNode);
        positionChanged();
        return true;
    }
    return false;
}


bool TreeKeyIdx::nextSibling() {
    if (m_currentNode.next > -1) {
        m_error = getTreeNodeFromIdxOffset(m_currentNode.next, &m_currentNode);
        positionChanged();
        return true;
    }
    return false;
}


bool TreeKeyIdx::previousSibling() {
    TreeNode iterator;
    int32_t target = m_currentNode.offset;
    if (m_currentNode.parent > -1) {
        getTreeNodeFromIdxOffset(m_currentNode.parent, &iterator);
        getTreeNodeFromIdxOffset(iterator.firstChild, &iterator);
        if (iterator.offset != target) {
            while ((iterator.next != target) && (iterator.next > -1))
                getTreeNodeFromIdxOffset(iterator.next, &iterator);
            if (iterator.next > -1) {
                m_error = getTreeNodeFromIdxOffset(iterator.offset, &m_currentNode);
                positionChanged();
                return true;
            }
        }
    }
    return false;
}


bool TreeKeyIdx::hasChildren() {
    return (m_currentNode.firstChild > -1);
}


void TreeKeyIdx::append() {
    TreeNode lastSib;
    if (m_currentNode.offset) {
        getTreeNodeFromIdxOffset(m_currentNode.offset, &lastSib);
        while (lastSib.next > -1) {
            getTreeNodeFromIdxOffset(lastSib.next, &lastSib);
        }
        uint32_t idxOffset = m_idxfd->seek(0, SEEK_END);
        lastSib.next = idxOffset;
        saveTreeNodeOffsets(&lastSib);
        uint32_t parent = m_currentNode.parent;
        m_currentNode.clear();
        m_currentNode.offset = idxOffset;
        m_currentNode.parent = parent;
        positionChanged();
    }
}


void TreeKeyIdx::appendChild() {
    if (firstChild()) {
        append();
    }
    else {
        uint32_t idxOffset = m_idxfd->seek(0, SEEK_END);
        m_currentNode.firstChild = idxOffset;
        saveTreeNodeOffsets(&m_currentNode);
        uint32_t parent = m_currentNode.offset;
        m_currentNode.clear();
        m_currentNode.offset = idxOffset;
        m_currentNode.parent = parent;
    }
    positionChanged();
}


void TreeKeyIdx::insertBefore() {
}


void TreeKeyIdx::remove() {
    TreeNode node;
    bool done = false;
    if (m_currentNode.offset) {
        getTreeNodeFromIdxOffset(m_currentNode.offset, &node);
        if (node.parent > -1) {
            TreeNode parent;
            getTreeNodeFromIdxOffset(node.parent, &parent);
            if (parent.firstChild == node.offset) {
                parent.firstChild = node.next;
                saveTreeNodeOffsets(&parent);
                getTreeNodeFromIdxOffset(parent.offset, &m_currentNode);
                done = true;
            }
        }
        if (!done) {
            TreeNode iterator;
            int32_t target = m_currentNode.offset;
            if (m_currentNode.parent > -1) {
                getTreeNodeFromIdxOffset(m_currentNode.parent, &iterator);
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
                        getTreeNodeFromIdxOffset(prev.offset, &m_currentNode);
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

signed char TreeKeyIdx::create(NormalizedPath const & path) {
    std::string const datFilename(path.str() + ".dat");
    std::string const idxFilename(path.str() + ".idx");

    FileMgr::removeFile(datFilename.c_str());
    FileDesc * const fd =
            FileMgr::getSystemFileMgr()->open(datFilename.c_str(),
                                              FileMgr::CREAT | FileMgr::WRONLY,
                                              FileMgr::IREAD | FileMgr::IWRITE);
    fd->getFd();
    FileMgr::getSystemFileMgr()->close(fd);

    FileMgr::removeFile(idxFilename.c_str());
    FileDesc * const fd2 =
            FileMgr::getSystemFileMgr()->open(idxFilename.c_str(),
                                              FileMgr::CREAT | FileMgr::WRONLY,
                                              FileMgr::IREAD | FileMgr::IWRITE);
    fd2->getFd();
    FileMgr::getSystemFileMgr()->close(fd2);

    TreeKeyIdx newTree(path.c_str());
    TreeKeyIdx::TreeNode root;
    newTree.saveTreeNode(&root);

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
    m_unsnappedKeyText.clear();
    int32_t  tmp;
    uint16_t  tmp2;

    if (m_datfd) {

        m_datfd->seek(ioffset, SEEK_SET);

        m_datfd->read(&tmp, 4);
        node->parent = swordtoarch32(tmp);

        m_datfd->read(&tmp, 4);
        node->next = swordtoarch32(tmp);

        m_datfd->read(&tmp, 4);
        node->firstChild = swordtoarch32(tmp);

        std::string name;
        for (;;) {
            char ch;
            auto const r = m_datfd->read(&ch, 1u);
            assert(r <= 1);
            if (r < 0) {
                assert(r == -1);
                throw std::system_error(errno, std::system_category());
            }
            if ((r == 0) || (ch == '\0'))
                break;
            name += ch;
        };

        node->name = name;

        m_datfd->read(&tmp2, 2);
        node->dsize = swordtoarch16(tmp2);

        if (node->dsize) {
            delete[] node->userData;
            node->userData = new char [node->dsize];
            m_datfd->read(node->userData, node->dsize);
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
    m_unsnappedKeyText.clear();
    uint32_t offset;
    char error = KEYERR_OUTOFBOUNDS;

    if (ioffset < 0) {
        ioffset = 0;
        error = 77;    // out of bounds but still position to 0;
    }

    node->offset = ioffset;
    if (m_idxfd) {
        if (m_idxfd->getFd() > 0) {
            m_idxfd->seek(ioffset, SEEK_SET);
            if (m_idxfd->read(&offset, 4) == 4) {
                offset = swordtoarch32(offset);
                error = (error == 77) ? KEYERR_OUTOFBOUNDS : 0;
                getTreeNodeFromDatOffset(offset, node);
            }
            else {
                m_idxfd->seek(-4, SEEK_END);
                if (m_idxfd->read(&offset, 4) == 4) {
                    offset = swordtoarch32(offset);
                    getTreeNodeFromDatOffset(offset, node);
                }
            }
        }
    }
    return error;
}


unsigned long TreeKeyIdx::getOffset() const {
    m_unsnappedKeyText.clear();
    return m_currentNode.offset;
}

void TreeKeyIdx::setOffset(unsigned long offset) {
    m_error = getTreeNodeFromIdxOffset(offset, &m_currentNode);
    positionChanged();
}


void TreeKeyIdx::saveTreeNodeOffsets(TreeNode *node) {
    m_unsnappedKeyText.clear();
    long datOffset = 0;
    int32_t tmp;

    if (m_idxfd) {
        m_idxfd->seek(node->offset, SEEK_SET);
        if (m_idxfd->read(&tmp, 4) != 4) {
            datOffset = m_datfd->seek(0, SEEK_END);
            tmp = archtosword32(datOffset);
            m_idxfd->write(&tmp, 4);
        }
        else {
            datOffset = swordtoarch32(tmp);
            m_datfd->seek(datOffset, SEEK_SET);
        }

        tmp = archtosword32(node->parent);
        m_datfd->write(&tmp, 4);

        tmp = archtosword32(node->next);
        m_datfd->write(&tmp, 4);

        tmp = archtosword32(node->firstChild);
        m_datfd->write(&tmp, 4);
    }
}


void TreeKeyIdx::copyFrom(const TreeKeyIdx &ikey) {
    m_unsnappedKeyText.clear();

    SWKey::copyFrom(ikey);

    m_currentNode.offset = ikey.m_currentNode.offset;
    m_currentNode.parent = ikey.m_currentNode.parent;
    m_currentNode.next = ikey.m_currentNode.next;
    m_currentNode.firstChild = ikey.m_currentNode.firstChild;
    m_currentNode.name = ikey.m_currentNode.name;
    m_currentNode.dsize = ikey.m_currentNode.dsize;

    delete[] m_currentNode.userData;
    if (m_currentNode.dsize) {
        m_currentNode.userData = new char [ m_currentNode.dsize ];
        memcpy(m_currentNode.userData, ikey.m_currentNode.userData, m_currentNode.dsize);
    }
    else m_currentNode.userData = nullptr;

    bool newFiles = true;

    if (m_path && ikey.m_path)
        newFiles = strcmp(m_path, ikey.m_path);

    if (newFiles) {
        stdstr(&m_path, ikey.m_path);

        if (m_idxfd) {
            FileMgr::getSystemFileMgr()->close(m_idxfd);
            FileMgr::getSystemFileMgr()->close(m_datfd);
        }
        m_idxfd = FileMgr::getSystemFileMgr()->open(ikey.m_idxfd->path.c_str(), ikey.m_idxfd->mode, ikey.m_idxfd->perms);
        m_datfd = FileMgr::getSystemFileMgr()->open(ikey.m_datfd->path.c_str(), ikey.m_datfd->mode, ikey.m_datfd->perms);
    }
    positionChanged();
}


void TreeKeyIdx::saveTreeNode(TreeNode *node) {
    long datOffset = 0;
    int32_t tmp;
    if (m_idxfd) {
        m_idxfd->seek(node->offset, SEEK_SET);
        datOffset = m_datfd->seek(0, SEEK_END);
        tmp = archtosword32(datOffset);
        m_idxfd->write(&tmp, 4);

        saveTreeNodeOffsets(node);

        m_datfd->write(node->name.c_str(), node->name.size());
        char null = 0;
        m_datfd->write(&null, 1);

        uint16_t tmp2 = archtosword16(node->dsize);
        m_datfd->write(&tmp2, 2);

        if (node->dsize) {
            m_datfd->write(node->userData, node->dsize);
        }
    }
}


void TreeKeyIdx::setText(const char *ikey) {
    static auto const nextTokenBounds =
            [](char const * s) noexcept {
                assert(s);
                while (*s == '/') // Skip any leading '/' delimeters
                    ++s;
                char const * end = s;
                while (*end && *end != '/') // Skip until next '/' delimeter/end
                    ++end;
                return std::make_pair(s, end);
            };
    auto r = nextTokenBounds(ikey);
    std::string leaf(r.first, r.second);
    trimString(leaf);
    root();
    while ((leaf.size()) && (!popError())) {
        bool ok, inChild = false;
        m_error = KEYERR_OUTOFBOUNDS;
        for (ok = firstChild(); ok; ok = nextSibling()) {
            inChild = true;
            if (leaf == getLocalName()) {
                m_error = 0;
                break;
            }
        }
        r = nextTokenBounds(r.second);
        leaf.assign(r.first, r.second);
        trimString(leaf);
        if (!ok) {
                if (inChild) {    // if we didn't find a matching child node, default to first child
                parent();
                firstChild();
            }
            m_error = KEYERR_OUTOFBOUNDS;
        }
    }
    if (leaf.size())
        m_error = KEYERR_OUTOFBOUNDS;
    m_unsnappedKeyText = ikey;
    positionChanged();
}



void TreeKeyIdx::copyFrom(const SWKey &ikey) {
    m_unsnappedKeyText = ikey.getText();
    SWKey::copyFrom(ikey);
    positionChanged();
}

void TreeKeyIdx::positionToTop() {
    root();
    positionChanged();
    popError();    // clear error from normalize
}

void TreeKeyIdx::positionToBottom() {
    m_error = getTreeNodeFromIdxOffset(m_idxfd->seek(-4, SEEK_END), &m_currentNode);
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
    m_error = getTreeNodeFromIdxOffset(m_currentNode.offset - (4*steps), &m_currentNode);
    positionChanged();
}

void TreeKeyIdx::increment(int steps) {
    m_error = getTreeNodeFromIdxOffset(m_currentNode.offset + (4*steps), &m_currentNode);
    if (m_error) {
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
    fullPath = m_currentNode.name;
    parent.parent = m_currentNode.parent;
    while (parent.parent > -1) {
        getTreeNodeFromIdxOffset(parent.parent, &parent);
        fullPath = std::string(parent.name) + '/' + fullPath;
    }
    // we've snapped; clear our unsnapped text holder
    m_unsnappedKeyText.clear();
    return fullPath.c_str();
}


TreeKeyIdx::TreeNode::TreeNode() {
    userData   = nullptr;

    clear();
}


void TreeKeyIdx::TreeNode::clear() {
    offset     = 0;
    parent     = -1;
    next       = -1;
    firstChild = -1;
    dsize      = 0;

    name.clear();

    delete[] userData;
    userData   = nullptr;
}


TreeKeyIdx::TreeNode::~TreeNode() {
    delete[] userData;
}


SWKey *TreeKeyIdx::clone() const
{
    return new TreeKeyIdx(*this);
}

} /* namespace swordxx */
