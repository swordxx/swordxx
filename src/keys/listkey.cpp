/******************************************************************************
 *  listkey.cpp - code for base class 'ListKey'.  ListKey is the basis for all
 *				types of keys that have lists of specified indexes
 *				(e.g. a list of verses, place, etc.)
 */

#include <utilfuns.h>
#include <string.h>
#include <stdlib.h>
#include <swkey.h>
#include <listkey.h>

/******************************************************************************
 * ListKey Constructor - initializes instance of ListKey
 *
 * ENT:	ikey - text key
 */

ListKey::ListKey(const char *ikey): SWKey(ikey) {
	arraymax = 0;
	ClearList();
}


ListKey::ListKey(ListKey const &k) : SWKey(k.keytext) {
	arraymax = k.arraymax;
	arraypos = k.arraypos;
	arraycnt = k.arraycnt;
	array = (arraymax)?(SWKey **)malloc(k.arraymax * sizeof(SWKey *)):0;
	for (int i = 0; i < arraycnt; i++)
		array[i] = k.array[i]->clone();
}

SWKey *ListKey::clone() const
{
	return new ListKey(*this);
}

/******************************************************************************
 * ListKey Destructor - cleans up instance of ListKey
 */

ListKey::~ListKey()
{
	ClearList();
}


/******************************************************************************
 * ListKey::ClearList	- Clears out elements of list
 */

void ListKey::ClearList()
{
	int loop;

	if (arraymax) {
		for (loop = 0; loop < arraycnt; loop++)
			delete array[loop];

		free(array);
		arraymax  = 0;
	}
	arraycnt  = 0;
	arraypos  = 0;
	array     = 0;
}


/******************************************************************************
 * ListKey::operator = Equates this ListKey to another ListKey object
 *
 * ENT:	ikey - other ListKey object
 */

ListKey &ListKey::operator =(const ListKey &ikey)
{
	ClearList();

	arraymax = ikey.arraymax;
	arraypos = ikey.arraypos;
	arraycnt = ikey.arraycnt;
	array = (arraymax)?(SWKey **)malloc(ikey.arraymax * sizeof(SWKey *)):0;
	for (int i = 0; i < arraycnt; i++)
		array[i] = ikey.array[i]->clone();

	SetToElement(0);
	return *this;
}


/******************************************************************************
 * ListKey::operator << - Adds an element to the list
 */

ListKey &ListKey::operator <<(const SWKey &ikey)
{
	if (++arraycnt > arraymax) {
		array = (SWKey **) ((array) ? realloc(array, (arraycnt + 32) * sizeof(SWKey *)) : calloc(arraycnt + 32, sizeof(SWKey *)));
		arraymax = arraycnt + 32;
	}
	array[arraycnt-1] = ikey.clone();
	SetToElement(arraycnt-1);

	return *this;
}



/******************************************************************************
 * ListKey::operator =(POSITION)	- Positions this key
 *
 * ENT:	p	- position
 *
 * RET:	*this
 */

SWKey &ListKey::operator =(POSITION p)
{
	switch (p) {
	case 1:	// GCC won't compile P_TOP
		SetToElement(0);
		break;
	case 2:	// GCC won't compile P_BOTTOM
		SetToElement(arraycnt-1);
		break;
	}
	return *this;
}


/******************************************************************************
 * ListKey::operator += - Increments a number of elements
 */

SWKey &ListKey::operator +=(int increment)
{
	if (increment < 0)
		return operator -=(increment*-1);
	Error();		// clear error
	for(; increment && !Error(); increment--) {
		if (arraypos < arraycnt) {
			(*(array[arraypos]))++;
			if (array[arraypos]->Error()) {
				SetToElement(arraypos+1);
			}
			else *this = (const char *)(*array[arraypos]);
		}
		else error = KEYERR_OUTOFBOUNDS;
	}
	return *this;
}


/******************************************************************************
 * ListKey::operator -= - Decrements a number of elements
 */

SWKey &ListKey::operator -=(int decrement)
{
	if (decrement < 0)
		return operator +=(decrement*-1);

	Error();		// clear error
	for(; decrement && !Error(); decrement--) {
		if (arraypos > -1) {
			(*(array[arraypos]))--;
			if (array[arraypos]->Error()) {
				SetToElement(arraypos-1, BOTTOM);
			}
			else *this = (const char *)(*array[arraypos]);
		}
		else error = KEYERR_OUTOFBOUNDS;
	}
	return *this;
}


/******************************************************************************
 * ListKey::Count	- Returns number of elements in list
 */

int ListKey::Count()
{
	return arraycnt;
}


/******************************************************************************
 * ListKey::SetToElement	- Sets key to element number
 *
 * ENT:	ielement	- element number to set to
 *
 * RET:	error status
 */

char ListKey::SetToElement(int ielement, POSITION pos)
{
	arraypos = ielement;
	if (arraypos >= arraycnt) {
		arraypos = (arraycnt>0)?arraycnt - 1:0;
		error = KEYERR_OUTOFBOUNDS;
	}
	else {
		if (arraypos < 0) {
			arraypos = 0;
			error = KEYERR_OUTOFBOUNDS;
		}
		else {
			error = 0;
		}
	}
	
	if (arraycnt) {
		(*array[arraypos]) = pos;
		*this = (const char *)(*array[arraypos]);
	}
	else *this = "";
	
	return error;
}


/******************************************************************************
 * ListKey::GetElement	- Gets a key element number
 *
 * ENT:	pos	- element number to get (or default current)
 *
 * RET:	Key or null on error
 */

SWKey *ListKey::GetElement(int pos) {
	if (pos >=0) {
		if (pos >=arraycnt)
			error = KEYERR_OUTOFBOUNDS;
	}
	else pos = arraypos;
	return (error) ? 0:array[pos];
}
	

/******************************************************************************
 * ListKey::Remove	- Removes current element from list
 */

void ListKey::Remove() {
	if ((arraypos > -1) && (arraypos < arraycnt)) {
		delete array[arraypos];
		if (arraypos < arraycnt - 1)
			memmove(&array[arraypos], &array[arraypos+1], (arraycnt - arraypos - 1) * sizeof(SWKey *));
		arraycnt--;
		
		SetToElement((arraypos)?arraypos-1:0);
	}
}
