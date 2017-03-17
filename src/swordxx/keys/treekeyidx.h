/******************************************************************************
 *
 *  treekeyidx.h -    code for class 'TreeKeyIdx'
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


#ifndef TREEKEYIDX_H
#define TREEKEYIDX_H

#include <cstdint>
#include "../normalizedpath.h"
#include "treekey.h"


namespace swordxx {

class FileDesc;

/**
 * Class TreeKeyIdx
 * The TreeKey implementation used for all tree-based modules in Sword++, such as GenBooks.
 */
class SWDLLEXPORT TreeKeyIdx : public TreeKey {

    class TreeNode {
    public:
        TreeNode();
        ~TreeNode();
        void clear();
        int32_t offset;
        int32_t parent;
        int32_t next;
        int32_t firstChild;
        std::string name;
        uint16_t dsize;
        char *userData;
    } m_currentNode;

    char *m_path;

    FileDesc *m_idxfd;
    FileDesc *m_datfd;

    void getTreeNodeFromDatOffset(long ioffset, TreeNode *buf) const;
    char getTreeNodeFromIdxOffset(long ioffset, TreeNode *node) const;
    void saveTreeNode(TreeNode *node);
    void saveTreeNodeOffsets(TreeNode *node);


public:
    TreeKeyIdx(const TreeKeyIdx &ikey);
    TreeKeyIdx(const char *idxPath, int fileMode = -1);
    ~TreeKeyIdx() override;

    SWKey *clone() const override;

    std::string const & getLocalName() override;
    void setLocalName(std::string const & newName) override;
    void setLocalName(std::string && newName) override;

    char const * getUserData(int * size = nullptr) const override;
    void setUserData(char const * userData, int size = 0) override;

    void root() override;
    bool parent() override;

    bool firstChild() override;
    bool nextSibling() override;
    bool previousSibling() override;

    bool hasChildren() override;

    void append() override;
    void appendChild() override;
    void insertBefore() override;

    void remove() override;
    void save() override;

    virtual void copyFrom(const TreeKeyIdx &ikey);
    void copyFrom(SWKey const & ikey) override;

    void setOffset(unsigned long offset) override;
    unsigned long getOffset() const override;

    int getLevel() override;


    // OPERATORS ------------------------------------------------------------


    virtual SWKey &operator = (const TreeKeyIdx &ikey) { copyFrom(ikey); return *this; }
    SWKEY_OPERATORS(TreeKeyIdx)

    void setText(char const * ikey) override;
    void positionToTop() override;
    void positionToBottom() override;
    char const * getText() const override;
    int _compare(TreeKeyIdx const & ikey) const noexcept;
    int compare(SWKey const & ikey) const noexcept override;
    void decrement(int steps = 1) override;
    void increment(int steps = 1) override;
    bool isTraversable() const override { return true; }

    static signed char create(NormalizedPath const & path);
};

} /* namespace swordxx */

#endif
