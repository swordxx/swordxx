%{
#include "treekey.h"
using namespace sword;
%}

class TreeKey : public SWKey {
protected:
	TreeKey();
	~TreeKey();

public:

	virtual const char *getLocalName() = 0;
	virtual const char *setLocalName(const char *) = 0;

	virtual const char *getUserData(int *size = 0) = 0;
	virtual void setUserData(const char *userData, int size = 0) = 0;

	virtual const char *getFullName() const = 0;

	virtual void root() = 0;
	virtual bool parent() = 0;

	virtual bool firstChild() = 0;
	virtual bool nextSibling() = 0;
	virtual bool previousSibling() = 0;

	virtual bool hasChildren() = 0;

	virtual void append() = 0;
	virtual void appendChild() = 0;
	virtual void insertBefore() = 0;

	virtual void remove() = 0;

	virtual void setOffset(unsigned long offset) = 0;
	virtual unsigned long getOffset() const = 0;

	//virtual void setText(const char *ikey) = 0;
	virtual void setPosition(SW_POSITION p) = 0;
	//virtual const char *getText() const = 0;
	//virtual int compare(const SWKey &ikey) = 0;
	//virtual void decrement(int steps = 1) = 0;
	//virtual void increment(int steps = 1) = 0;
	virtual char Traversable ();
	virtual long Index () const;
	//virtual long Index (long iindex);
};
