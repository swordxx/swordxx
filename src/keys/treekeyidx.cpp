/******************************************************************************
 *  versekey.h - code for class 'versekey'- a standard Biblical verse key
 *
 * $Id: treekeyidx.cpp,v 1.17 2004/04/02 23:54:45 willthimbleby Exp $
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
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


#include <treekeyidx.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

SWORD_NAMESPACE_START

static const char nl = '\n';
static const char *classes[] = {"TreeKeyIdx", "TreeKey", "SWKey", "SWObject", 0};
SWClass TreeKeyIdx::classdef(classes);


TreeKeyIdx::TreeKeyIdx(const TreeKeyIdx &ikey) : currentNode() {
	init();
	path = 0;
	idxfd = 0;
	datfd = 0;
	copyFrom(ikey);
}

TreeKeyIdx::TreeKeyIdx(const char *idxPath, int fileMode) : currentNode() {
	SWBuf buf;

	init();
	path = 0;
	stdstr(&path, idxPath);

#ifndef O_BINARY		// O_BINARY is needed in Borland C++ 4.53
#define O_BINARY 0		// If it hasn't been defined than we probably
#endif				// don't need it.

	if (fileMode == -1) { // try read/write if possible
		fileMode = O_RDWR;
	}
		
	buf.setFormatted("%s.idx", path);
	idxfd = FileMgr::getSystemFileMgr()->open(buf, fileMode|O_BINARY, true);
	buf.setFormatted("%s.dat", path);
	datfd = FileMgr::getSystemFileMgr()->open(buf, fileMode|O_BINARY, true);

	if (datfd <= 0) {
		buf.setFormatted("Error: %d", errno);
		perror(buf);
		error = errno;
	}
	else {
		root();
	}
}


void TreeKeyIdx::init() {
	myclass = &classdef;
}


TreeKeyIdx::~TreeKeyIdx () {
	if (path)
		delete [] path;

	FileMgr::getSystemFileMgr()->close(idxfd);
	FileMgr::getSystemFileMgr()->close(datfd);
}


const char *TreeKeyIdx::getLocalName() {
	return currentNode.name;
}


const char *TreeKeyIdx::getUserData(int *size) {
	if (size)
		*size = (int)currentNode.dsize;
	return currentNode.userData;
}


void TreeKeyIdx::setUserData(const char *userData, int size) {
	if (currentNode.userData)
		delete currentNode.userData;

	if (!size)
		size = strlen(userData) + 1;

	currentNode.userData = new char [ size ];
	memcpy(currentNode.userData, userData, size);
	currentNode.dsize = size;
}

const char *TreeKeyIdx::setLocalName(const char *newName) {
	stdstr(&(currentNode.name), newName);
	return currentNode.name;
}


void TreeKeyIdx::save() {
	saveTreeNode(&currentNode);
}


const char *TreeKeyIdx::getFullName() const {
	TreeNode parent;
	static SWBuf fullPath;
	fullPath = currentNode.name;
	parent.parent = currentNode.parent;
	while (parent.parent > -1) {
		getTreeNodeFromIdxOffset(parent.parent, &parent);
		fullPath = ((SWBuf)parent.name) + (SWBuf) "/" + fullPath;
	}
	return fullPath.c_str();
}


void TreeKeyIdx::root() {
	error = getTreeNodeFromIdxOffset(0, &currentNode);
}


bool TreeKeyIdx::parent() {
	if (currentNode.parent > -1) {
		error = getTreeNodeFromIdxOffset(currentNode.parent, &currentNode);
		return true;
	}
	return false;
}


bool TreeKeyIdx::firstChild() {
	if (currentNode.firstChild > -1) {
		error = getTreeNodeFromIdxOffset(currentNode.firstChild, &currentNode);
		return true;
	}
	return false;
}


bool TreeKeyIdx::nextSibling() {
	if (currentNode.next > -1) {
		error = getTreeNodeFromIdxOffset(currentNode.next, &currentNode);
		return true;
	}
	return false;
}


bool TreeKeyIdx::previousSibling() {
	TreeNode iterator;
	__u32 target = currentNode.offset;
	if (currentNode.parent > -1) {
		getTreeNodeFromIdxOffset(currentNode.parent, &iterator);
		getTreeNodeFromIdxOffset(iterator.firstChild, &iterator);
		if (iterator.offset != target) {
			while ((iterator.next != target) && (iterator.next > -1))
				getTreeNodeFromIdxOffset(iterator.next, &iterator);
			if (iterator.next > -1) {
				error = getTreeNodeFromIdxOffset(iterator.offset, &currentNode);
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
		__u32 idxOffset = lseek(idxfd->getFd(), 0, SEEK_END);
		lastSib.next = idxOffset;
		saveTreeNodeOffsets(&lastSib);
		__u32 parent = currentNode.parent;
		currentNode.clear();
		currentNode.offset = idxOffset;
		currentNode.parent = parent;
	}
}


void TreeKeyIdx::appendChild() {
	if (firstChild()) {
		append();
	}
	else {
		__u32 idxOffset = lseek(idxfd->getFd(), 0, SEEK_END);
		currentNode.firstChild = idxOffset;
		saveTreeNodeOffsets(&currentNode);
		__u32 parent = currentNode.offset;
		currentNode.clear();
		currentNode.offset = idxOffset;
		currentNode.parent = parent;
	}
}


void TreeKeyIdx::insertBefore() {
}


void TreeKeyIdx::remove() {
}


/******************************************************************************
 * TreeKeyIdx::Create	- Creates new key idx/dat files
 *
 * ENT: path	- directory to store module files
 * RET: error status
 */

signed char TreeKeyIdx::create(const char *ipath) {
	char *path = 0;
	char *buf = new char [ strlen (ipath) + 20 ];
	FileDesc *fd, *fd2;

	stdstr(&path, ipath);

	if ((path[strlen(path)-1] == '/') || (path[strlen(path)-1] == '\\'))
		path[strlen(path)-1] = 0;

	sprintf(buf, "%s.dat", path);
	FileMgr::removeFile(buf);
	fd = FileMgr::getSystemFileMgr()->open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();
	FileMgr::getSystemFileMgr()->close(fd);

	sprintf(buf, "%s.idx", path);
	FileMgr::removeFile(buf);
	fd2 = FileMgr::getSystemFileMgr()->open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
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
 * zStr::getidxbufdat	- Gets the index string at the given dat offset
 *				NOTE: buf is calloc'd, or if not null, realloc'd and must
 *					be free'd by calling function
 *
 * ENT:	ioffset	- offset in dat file to lookup
 *		node		- address of pointer to allocate for storage of string
 */

void TreeKeyIdx::getTreeNodeFromDatOffset(long ioffset, TreeNode *node) const {
	char ch;
	__s32  tmp;
	__u16  tmp2;

	if (datfd > 0) {

		lseek(datfd->getFd(), ioffset, SEEK_SET);

		read(datfd->getFd(), &tmp, 4);
		node->parent = swordtoarch32(tmp);

		read(datfd->getFd(), &tmp, 4);
		node->next = swordtoarch32(tmp);

		read(datfd->getFd(), &tmp, 4);
		node->firstChild = swordtoarch32(tmp);

		SWBuf name;
		do {
			read(datfd->getFd(), &ch, 1);
			name += ch;
		} while (ch);

		stdstr(&(node->name), name.c_str());

		read(datfd->getFd(), &tmp2, 2);
		node->dsize = swordtoarch16(tmp2);

		if (node->dsize) {
			if (node->userData)
				delete [] node->userData;
			node->userData = new char [node->dsize];
			read(datfd->getFd(), node->userData, node->dsize);
		}
	}
}


/******************************************************************************
 * zStr::getidxbuf	- Gets the index string at the given idx offset
 *						NOTE: buf is calloc'd, or if not null, realloc'd
 *							and must be freed by calling function
 *
 * ENT:	ioffset	- offset in idx file to lookup
 *		buf		- address of pointer to allocate for storage of string
 */

char TreeKeyIdx::getTreeNodeFromIdxOffset(long ioffset, TreeNode *node) const {
	__u32 offset;
	char error = KEYERR_OUTOFBOUNDS;
	
	if (ioffset < 0) {
		ioffset = 0;
		error = 77;	// out of bounds but still position to 0;
	}

	node->offset = ioffset;
	if (idxfd > 0) {
		if (idxfd->getFd() > 0) {
			lseek(idxfd->getFd(), ioffset, SEEK_SET);
			if (read(idxfd->getFd(), &offset, 4) == 4) {
				offset = swordtoarch32(offset);
				error = (error == 77) ? KEYERR_OUTOFBOUNDS : 0;
				getTreeNodeFromDatOffset(offset, node);
			}
			else {
				lseek(idxfd->getFd(), -4, SEEK_END);
				if (read(idxfd->getFd(), &offset, 4) == 4) {
					offset = swordtoarch32(offset);
					getTreeNodeFromDatOffset(offset, node);
				}
			}
		}
	}
	return error;
}


unsigned long TreeKeyIdx::getOffset() const {
	return currentNode.offset;
}

void TreeKeyIdx::setOffset(unsigned long offset) {
	error = getTreeNodeFromIdxOffset(offset, &currentNode);
}


void TreeKeyIdx::saveTreeNodeOffsets(TreeNode *node) {
	long datOffset = 0;
	__s32 tmp;

	if (idxfd > 0) {
		lseek(idxfd->getFd(), node->offset, SEEK_SET);
		if (read(idxfd->getFd(), &tmp, 4) != 4) {
			datOffset = lseek(datfd->getFd(), 0, SEEK_END);
			tmp = archtosword32(datOffset);
			write(idxfd->getFd(), &tmp, 4);
		}
		else {
			datOffset = swordtoarch32(tmp);
			lseek(datfd->getFd(), datOffset, SEEK_SET);
		}

		tmp = archtosword32(node->parent);
		write(datfd->getFd(), &tmp, 4);

		tmp = archtosword32(node->next);
		write(datfd->getFd(), &tmp, 4);

		tmp = archtosword32(node->firstChild);
		write(datfd->getFd(), &tmp, 4);
	}
}


void TreeKeyIdx::copyFrom(const TreeKeyIdx &ikey) {

	SWKey::copyFrom(ikey);

	currentNode.offset = ikey.currentNode.offset;
	currentNode.parent = ikey.currentNode.parent;
	currentNode.next = ikey.currentNode.next;
	currentNode.firstChild = ikey.currentNode.firstChild;
	stdstr(&(currentNode.name), ikey.currentNode.name);
	currentNode.dsize = ikey.currentNode.dsize;

	if (currentNode.userData)
		delete [] currentNode.userData;
	if (currentNode.dsize) {
		currentNode.userData = new char [ currentNode.dsize ];
		memcpy(currentNode.userData, ikey.currentNode.userData, currentNode.dsize);
	}
	else currentNode.userData = 0;

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
}


void TreeKeyIdx::saveTreeNode(TreeNode *node) {
	long datOffset = 0;
	__s32 tmp;
	if (idxfd > 0) {

		lseek(idxfd->getFd(), node->offset, SEEK_SET);
		datOffset = lseek(datfd->getFd(), 0, SEEK_END);
		tmp = archtosword32(datOffset);
		write(idxfd->getFd(), &tmp, 4);

		saveTreeNodeOffsets(node);

		write(datfd->getFd(), node->name, strlen(node->name));
		char null = 0;
		write(datfd->getFd(), &null, 1);

		__u16 tmp2 = archtosword16(node->dsize);
		write(datfd->getFd(), &tmp2, 2);

		if (node->dsize) {
			write(datfd->getFd(), node->userData, node->dsize);
		}
	}
}


void TreeKeyIdx::setText(const char *ikey) {
	char *buf = 0;
	stdstr(&buf, ikey);
	char *leaf = strtok(buf, "/");
	root();
	while ((leaf) && (!Error())) {
		bool ok, inChild = false;
		for (ok = firstChild(); ok; ok = nextSibling()) {
			inChild = true;
			if (!stricmp(leaf, getLocalName()))
				break;
		}
		leaf = strtok(0, "/");
		if (!ok) {
		    	if (inChild) {	// if we didn't find a matching child node, default to first child
				parent();
				firstChild();
			}
			if (leaf)
				error = KEYERR_OUTOFBOUNDS;
			break;
		}
	}
	delete [] buf;
}



void TreeKeyIdx::copyFrom(const SWKey &ikey) {
	SWKey::copyFrom(ikey);
}

void TreeKeyIdx::setPosition(SW_POSITION p) {
	switch (p) {
	case POS_TOP:
		root();
		break;
	case POS_BOTTOM:
		error = getTreeNodeFromIdxOffset(lseek(idxfd->getFd(), -4, SEEK_END), &currentNode);
		break;
	} 
	Error();	// clear error from normalize
}

const char *TreeKeyIdx::getText() const {
	return getFullName();
}


int TreeKeyIdx::_compare (const TreeKeyIdx & ikey) {
		return (getOffset() - ikey.getOffset());
}


int TreeKeyIdx::compare(const SWKey &ikey) {
	TreeKeyIdx *treeKey = SWDYNAMIC_CAST(TreeKeyIdx, (&ikey));
	if (treeKey)
		return _compare(*treeKey);
	return SWKey::compare(ikey);
}


void TreeKeyIdx::decrement(int steps) {
	error = getTreeNodeFromIdxOffset(currentNode.offset - (4*steps), &currentNode);
}

void TreeKeyIdx::increment(int steps) {
	error = getTreeNodeFromIdxOffset(currentNode.offset + (4*steps), &currentNode);

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



TreeKeyIdx::TreeNode::TreeNode() {

	name       = 0;
	stdstr(&name, "");
	userData   = 0;

	clear();
}


void TreeKeyIdx::TreeNode::clear() {
	offset     = 0;
	parent     = -1;
	next       = -1;
	firstChild = -1;
	dsize      = 0;

	if (name)
		delete [] name;
	name = 0;
	stdstr(&name, "");

	if (userData)
		delete [] userData;
	userData   = 0;
}


TreeKeyIdx::TreeNode::~TreeNode() {
	if (name)
		delete [] name;
	
	if (userData)
		delete [] userData;
}


SWKey *TreeKeyIdx::clone() const
{
	return new TreeKeyIdx(*this);
}

SWORD_NAMESPACE_END
