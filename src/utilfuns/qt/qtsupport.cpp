#include <swbuf.h>
#include <swkey.h>
#include <QString>

SWORD_NAMESPACE_START


SWBuf::SWBuf(const QString &str) {
	init(0);
	set(str.toUtf8().constData());
}

SWBuf::operator QString () const {
	return QString::fromUtf8(c_str());
}

SWKey::SWKey(const QString &str) {
	init();
	index     = 0;
	persist   = 0;
	keytext   = 0;
	rangeText = 0;
	error     = 0;
	userData  = 0;
	stdstr(&keytext, str.toUtf8().constData());
}


SWORD_NAMESPACE_END
