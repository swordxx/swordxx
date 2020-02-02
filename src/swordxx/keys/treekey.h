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


#ifndef SWORDXX_TREEKEY_H
#define SWORDXX_TREEKEY_H

#include "../swkey.h"

#include <string>
#include "../defs.h"


namespace swordxx {

/**
 * Class TreeKey
 * The base class for all tree-based key implementations in Sword++.
 */
class SWDLLEXPORT TreeKey : public SWKey {

protected:
    // hold on to setText until we've done a snap action: getText or navigation
    // if we set, and then write out userData, we want to assure the path exists.
    // This better conforms to the Sword++ write methodology: mod.setKey, mod.setEntry
    mutable std::string m_unsnappedKeyText;

    // called whenever position of this key is changed.  Should we move this
    // to a more base class?
    void positionChanged() { if (m_posChangeListener) m_posChangeListener->positionChanged(); }

public:

    class PositionChangeListener {
        TreeKey * m_treeKey = nullptr;
    public:
        virtual ~PositionChangeListener() noexcept;
        virtual void positionChanged() = 0;
        TreeKey *getTreeKey() { return m_treeKey; }
        void setTreeKey(TreeKey *tk) { m_treeKey = tk; }
    } * m_posChangeListener = nullptr;

    void setPositionChangeListener(PositionChangeListener *pcl) { m_posChangeListener = pcl; m_posChangeListener->setTreeKey(this); }

//    TreeKey (const char *ikey = 0);
//    TreeKey (const SWKey * ikey);
//    TreeKey (TreeKey const &k);
    TreeKey();
    ~TreeKey() override;


    virtual std::string const & getLocalName() = 0;
    virtual void setLocalName(std::string const & newName) = 0;
    virtual void setLocalName(std::string && newName) = 0;

    virtual int getLevel() { long bm = getOffset(); int level = 0; do { level++; } while (parent()); setOffset(bm); return level; }

    virtual char const * getUserData(int * size = nullptr) const = 0;
    virtual void setUserData(const char *userData, int size = 0) = 0;

    /** Go to the root node
    */
    virtual void root() = 0;

    /** Go to the parent of the current node
    * @return success or failure
    */
    virtual bool parent() = 0;

    /** Go to the first child of the current node
    * @return success or failure
    */
    virtual bool firstChild() = 0;

    /** Go to the next sibling of the current node
    * @return success or failure
    */
    virtual bool nextSibling() = 0;

    /** Go to the previous sibling of the current node
    * @return success or failure
    */
    virtual bool previousSibling() = 0;

    /** Does the current node have children?
    * @return whether or not it does
    */
    virtual bool hasChildren() = 0;

    virtual void append() = 0;
    virtual void appendChild() = 0;
    virtual void insertBefore() = 0;

    virtual void remove() = 0;


    virtual void setOffset(unsigned long offset) = 0;
    virtual unsigned long getOffset() const = 0;

    void setText(std::string newText) override = 0;
    std::string getText() const override = 0;
    void decrement(int steps = 1) override = 0;
    void increment(int steps = 1) override = 0;
    bool isTraversable() const override ;
    long getIndex() const override;
    void setIndex(long iindex) override;

    /** Set the key to this path.  If the path doesn't exist, then
     *    nodes are created as necessary
     * @param keyPath path to set/create; if unsupplied, then use any unsnapped setText value.
     */
    virtual void assureKeyPath(char const * keyPath = nullptr);
    virtual void save() {}

    SWKEY_OPERATORS(TreeKey)

};

} /* namespace swordxx */

#endif /* SWORDXX_TREEKEY_H */
