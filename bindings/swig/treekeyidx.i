%{
#include "treekeyidx.h"
using namespace sword;
%}

class TreeKeyIdx : public TreeKey {
public:
	TreeKeyIdx(const TreeKeyIdx &ikey);
	TreeKeyIdx (const char *idxPath, int fileMode = -1);
	~TreeKeyIdx ();

	virtual void save();

	virtual void copyFrom(const TreeKeyIdx &ikey);

	virtual int _compare (const TreeKeyIdx & ikey);

	static signed char create(const char *path);
};
