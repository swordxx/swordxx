/******************************************************************************
 *
 *  treekeyidx.cpp -
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
    m_idxfd = nullptr;
    m_datfd = nullptr;
    copyFrom(ikey);
}

TreeKeyIdx::TreeKeyIdx(const char *idxPath, int fileMode)
    : m_currentNode()
    , m_path(idxPath ? idxPath : "")
{
    if (fileMode == -1) { // try read/write if possible
        fileMode = FileMgr::RDWR;
    }

    std::string buf(m_path + ".idx");
    auto const fileMgr(FileMgr::getSystemFileMgr());
    m_idxfd = fileMgr->open(buf.c_str(), fileMode, true);
    buf.resize(buf.size() - 4u);
    buf += ".dat";
    m_datfd = fileMgr->open(buf.c_str(), fileMode, true);

    if (m_datfd->getFd() < 0) {
        // couldn't find datafile but this might be fine if we're
        // merely instantiating a remote InstallMgr SWMgr
        SWLog::getSystemLog()->logDebug("Couldn't open file: %s. errno: %d",
                                        buf.c_str(),
                                        errno);
        m_error = errno;
    }
    else {
        root();
    }
}


TreeKeyIdx::~TreeKeyIdx() = default;


std::string const & TreeKeyIdx::getLocalName() {
    m_unsnappedKeyText.clear();
    return m_currentNode.name;
}


const char *TreeKeyIdx::getUserData(int *size) const {
    m_unsnappedKeyText.clear();
    if (size)
        *size = (int)m_currentNode.dsize;
    return m_currentNode.userData.get();
}


void TreeKeyIdx::setUserData(const char *userData, int size) {
    // this makes sure any unsnapped path exists
    assureKeyPath();

    if (!size)
        size = std::strlen(userData) + 1;

    m_currentNode.userData = std::make_unique<char[]>(size);
    std::memcpy(m_currentNode.userData.get(), userData, size);
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

    auto const fileMgr(FileMgr::getSystemFileMgr());

    FileMgr::removeFile(datFilename.c_str());
    fileMgr->touch(datFilename.c_str());

    FileMgr::removeFile(idxFilename.c_str());
    fileMgr->touch(idxFilename.c_str());

    TreeKeyIdx newTree(path.c_str());
    TreeKeyIdx::TreeNode root;
    newTree.saveTreeNode(&root);

    return 0;
}


/******************************************************************************
 * zStr::getidxbufdat    - Gets the index string at the given dat offset
 *                NOTE: buf is std::calloc'd, or if not null, realloc'd and must
 *                    be free'd by calling function
 *
 * ENT:    ioffset    - offset in dat file to lookup
 *        node        - address of pointer to allocate for storage of string
 */

void TreeKeyIdx::getTreeNodeFromDatOffset(long ioffset, TreeNode *node) const {
    m_unsnappedKeyText.clear();
    int32_t  tmp;
    uint16_t  tmp2;

    if (m_datfd && m_datfd->getFd() >= 0) {

        m_datfd->seek(ioffset, SEEK_SET);

        m_datfd->read(&tmp, sizeof(tmp));
        node->parent = swordtoarch32(tmp);

        m_datfd->read(&tmp, sizeof(tmp));
        node->next = swordtoarch32(tmp);

        m_datfd->read(&tmp, sizeof(tmp));
        node->firstChild = swordtoarch32(tmp);

        std::string name;
        for (;;) {
            char ch;
            auto const r = m_datfd->read(&ch, sizeof(ch));
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

        m_datfd->read(&tmp2, sizeof(tmp2));
        node->dsize = swordtoarch16(tmp2);

        if (node->dsize) {
            node->userData = std::make_unique<char[]>(node->dsize);
            m_datfd->read(node->userData.get(), node->dsize);
        }
    }
}


/******************************************************************************
 * zStr::getidxbuf    - Gets the index string at the given idx offset
 *                        NOTE: buf is std::calloc'd, or if not null, realloc'd
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
    if (m_idxfd && m_idxfd->getFd() >= 0) {
        m_idxfd->seek(ioffset, SEEK_SET);
        if (m_idxfd->read(&offset, sizeof(offset)) == sizeof(offset)) {
            offset = swordtoarch32(offset);
            error = (error == 77) ? KEYERR_OUTOFBOUNDS : 0;
            getTreeNodeFromDatOffset(offset, node);
        }
        else {
            m_idxfd->seek(-sizeof(offset), SEEK_END);
            if (m_idxfd->read(&offset, sizeof(offset)) == sizeof(offset)) {
                offset = swordtoarch32(offset);
                getTreeNodeFromDatOffset(offset, node);
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

    if (m_idxfd && m_idxfd->getFd() >= 0) {
        m_idxfd->seek(node->offset, SEEK_SET);
        if (m_idxfd->read(&tmp, sizeof(tmp)) != sizeof(tmp)) {
            datOffset = m_datfd->seek(0, SEEK_END);
            tmp = archtosword32(datOffset);
            m_idxfd->write(&tmp, sizeof(tmp));
        }
        else {
            datOffset = swordtoarch32(tmp);
            m_datfd->seek(datOffset, SEEK_SET);
        }

        tmp = archtosword32(node->parent);
        m_datfd->write(&tmp, sizeof(tmp));

        tmp = archtosword32(node->next);
        m_datfd->write(&tmp, sizeof(tmp));

        tmp = archtosword32(node->firstChild);
        m_datfd->write(&tmp, sizeof(tmp));
    }
}


void TreeKeyIdx::copyFrom(const TreeKeyIdx &ikey) {
    m_unsnappedKeyText.clear();

    SWKey::copyFrom(ikey);
    m_currentNode = ikey.m_currentNode;

    if (m_path != ikey.m_path) {
        m_path = ikey.m_path;

        m_idxfd.reset();
        m_datfd.reset();
        auto const fileMgr(FileMgr::getSystemFileMgr());
        m_idxfd = fileMgr->open(ikey.m_idxfd->path().c_str(), ikey.m_idxfd->mode(), ikey.m_idxfd->perms());
        m_datfd = fileMgr->open(ikey.m_datfd->path().c_str(), ikey.m_datfd->mode(), ikey.m_datfd->perms());
    }
    positionChanged();
}


void TreeKeyIdx::saveTreeNode(TreeNode *node) {
    long datOffset = 0;
    if (m_idxfd && m_idxfd->getFd() >= 0) {
        m_idxfd->seek(node->offset, SEEK_SET);
        datOffset = m_datfd->seek(0, SEEK_END);
        int32_t const tmp = archtosword32(datOffset);
        m_idxfd->write(&tmp, sizeof(tmp));

        saveTreeNodeOffsets(node);

        m_datfd->write(node->name.c_str(), node->name.size());
        char const null = '\0';
        m_datfd->write(&null, sizeof(null));

        uint16_t const tmp2 = archtosword16(node->dsize);
        m_datfd->write(&tmp2, sizeof(tmp2));

        if (node->dsize)
            m_datfd->write(node->userData.get(), node->dsize);
    }
}


void TreeKeyIdx::setText(std::string newText) {
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
    auto r = nextTokenBounds(newText.c_str());
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
    m_unsnappedKeyText = newText;
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
    m_error = getTreeNodeFromIdxOffset(m_idxfd->seek(-sizeof(uint32_t), SEEK_END), &m_currentNode);
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

bool TreeKeyIdx::isTraversable() const { return true; }

std::string TreeKeyIdx::getText() const {
    TreeNode parent;
    std::string fullPath(m_currentNode.name);
    parent.parent = m_currentNode.parent;
    while (parent.parent > -1) {
        getTreeNodeFromIdxOffset(parent.parent, &parent);
        fullPath = std::string(parent.name) + '/' + fullPath;
    }
    // we've snapped; clear our unsnapped text holder
    m_unsnappedKeyText.clear();
    return fullPath;
}

TreeKeyIdx::TreeNode & TreeKeyIdx::TreeNode::operator=(TreeNode const & copy) {
    offset = copy.offset;
    parent = copy.parent;
    next = copy.next;
    firstChild = copy.firstChild;
    name = copy.name;
    dsize = copy.dsize;
    if (dsize) {
        userData = std::make_unique<char[]>(dsize);
        std::memcpy(userData.get(), copy.userData.get(), dsize);
    } else {
        userData.reset();
    }
    return *this;
}

void TreeKeyIdx::TreeNode::clear() {
    offset = 0;
    parent = -1;
    next = -1;
    firstChild = -1;
    name.clear();
    dsize = 0;
    userData.reset();
}

std::shared_ptr<SWKey> TreeKeyIdx::clone() const
{ return std::make_shared<TreeKeyIdx>(*this); }

} /* namespace swordxx */
