%{
#include "listkey.h"
%}

class ListKey : public SWKey {
public:
	/** initializes instance of ListKey
	*
	* @param ikey text key
	*/
	ListKey (const char *ikey = 0);
	ListKey (ListKey const &k);

	/** Cleans up instance of ListKey
	*/
	virtual ~ ListKey ();

	virtual SWKey *clone () const;

	/** Clears out elements of list
	*/
	virtual void ClearList ();

	/** Returns number of elements in list
	* @return number of elements in list
	*/
	virtual int Count ();

	/** Removes current element from list
	*/
	virtual void Remove ();

	/** Sets key to element number
	*
	* @param ielement element number to set to
	* @return error status
	*/
	//virtual char SetToElement (int ielement, SW_POSITION = TOP);
%extend {
	virtual char SetToElement(int element) {
		self->SetToElement(element, TOP);
	};
}

	/** Gets a key element number
	*
	* @param pos element number to get (or default current)
	* @return Key or null on error
	*/
	virtual SWKey* GetElement (int pos = -1);

	/** Adds an element to the list
	* @param ikey the element to add
	*/
	//ListKey & operator << (const SWKey &ikey);
	virtual void add(const SWKey &ikey);

	/** Equates this ListKey to another ListKey object
	*
	* @param ikey other ListKey object
	*/
	virtual void copyFrom(const ListKey & ikey);
	//virtual void copyFrom(const SWKey & ikey) { SWKey::copyFrom(ikey); }

	/** Positions this key
	*
	* @param p position
	* @return *this
	*/
	virtual void setPosition(SW_POSITION);

	/** Decrements a number of elements
	*/
	virtual void decrement(int step);

	/** Increments a number of elements
	*/
	virtual void increment(int step);

	virtual char Traversable ();
	virtual long Index () const;

	/**
	* Returns the index for the new one given as as parameter.
	* The first parameter is the new index.
	*/
	virtual long Index (long index);

	//SWKEY_OPERATORS
	//ListKey & operator =(const ListKey &key) { copyFrom(key); return *this; }
};
