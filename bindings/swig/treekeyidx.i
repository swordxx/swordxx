%{
#include "treekeyidx.h"
using namespace sword;
%}

class TreeKeyIdx : public TreeKey {
	TreeKeyIdx(const TreeKeyIdx &ikey);
	TreeKeyIdx (const char *idxPath, int fileMode = -1);
	~TreeKeyIdx ();

//	virtual const char *getLocalName();
//	virtual const char *setLocalName(const char *);

//	virtual const char *getUserData(int *size = 0);
//	virtual void setUserData(const char *userData, int size = 0);

//	virtual const char *getFullName() const;

//	virtual void root();
//	virtual bool parent();

//	virtual bool firstChild();
//	virtual bool nextSibling();
//	virtual bool previousSibling();

//	virtual bool hasChildren();

//	virtual void append();
//	virtual void appendChild();
//	virtual void insertBefore();

//	virtual void remove();
	virtual void save();

	virtual void copyFrom(const TreeKeyIdx &ikey);
//	virtual void copyFrom(const SWKey & ikey);

//	virtual SWKey &operator = (const TreeKeyIdx &ikey) { copyFrom(ikey); return *this; }
//	void setOffset(unsigned long offset);
//	unsigned long getOffset() const;

//	SWKEY_OPERATORS

//	virtual void setText(const char *ikey);
//	virtual void setPosition(SW_POSITION p);
//	virtual const char *getText() const;
	virtual int _compare (const TreeKeyIdx & ikey);
//	virtual int compare(const SWKey &ikey);
//	virtual void decrement(int steps = 1);
//	virtual void increment(int steps = 1);
//	virtual char Traversable () { return 1; }

	static signed char create(const char *path);
};
