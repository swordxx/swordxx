%{
#include "swbuf.h"
using namespace sword;
%}

class SWBuf {
public:
  	SWBuf(const char *initVal = 0);
	SWBuf(char initVal);
	SWBuf(const SWBuf &other);
	virtual ~SWBuf();

	inline void setFillByte(char ch) { fillByte = ch; }
	inline char getFillByte() { return fillByte; }
	inline const char *c_str() const;
	inline char &charAt(unsigned int pos) { return ((pos <= (unsigned int)(end - buf)) ? buf[pos] : nullStr[0]); }
	inline unsigned int size() const { return length(); }
	inline unsigned int length() const { return end - buf; }

	void set(const char *newVal);
	void set(const SWBuf &newVal);
	void setSize(unsigned int len);
	void append(const char *str, int max = -1);
	inline void append(const SWBuf &str, int max = -1) { append(str.c_str(), max); }
	inline void append(char ch);
	void appendFormatted(const char *format, ...);

	inline char *getRawData();

	/*
	inline operator const char *() const { return c_str(); }
	inline char &operator[](unsigned int pos) { return charAt(pos); }
	inline char &operator[](int pos) { return charAt((unsigned int)pos); }
	inline SWBuf &operator =(const char *newVal) { set(newVal); return *this; }
	inline SWBuf &operator =(const SWBuf &other) { set(other); return *this; }
	inline SWBuf &operator +=(const char *str) { append(str); return *this; }
	inline SWBuf &operator +=(char ch) { append(ch); return *this; }
	inline SWBuf &operator -=(unsigned int len) { setSize(length()-len); return *this; }
	inline SWBuf &operator --(int) { operator -=(1); return *this; }
	inline SWBuf operator +(const SWBuf &other) const {
		SWBuf retVal = buf;
		retVal += other;
		return retVal;
	}
	inline SWBuf operator +(char ch) const { return (*this) + SWBuf(ch); }
	*/

	int compare(const SWBuf &other) const;

	/*
	inline bool operator ==(const SWBuf &other) const;
	inline bool operator !=(const SWBuf &other) const;
	inline bool operator > (const SWBuf &other) const;
	inline bool operator < (const SWBuf &other) const;
	inline bool operator <=(const SWBuf &other) const;
	inline bool operator >=(const SWBuf &other) const;

	inline bool operator ==(const char *other) const;
	inline bool operator !=(const char *other) const;
	inline bool operator > (const char *other) const;
	inline bool operator < (const char *other) const;
	inline bool operator <=(const char *other) const;
	inline bool operator >=(const char *other) const;
	*/
};
