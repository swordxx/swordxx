#include <string.h>
#include <utilstr.h>
#include <ctype.h>

/******************************************************************************
 * stdstr - Sets/gets a string
 *
 * ENT:	ipstr	- pointer to a string pointer to set if necessary
 *	istr	- string to set to *ipstr
 *			0 - only get
 *
 * RET:	*ipstr
 */

char *stdstr(char **ipstr, const char *istr) {
	if (istr) {
		if (*ipstr)
			delete [] *ipstr;
		int len = strlen(istr) + 1;
		*ipstr = new char [ len ];
		memcpy(*ipstr, istr, len);
	}
	return *ipstr;
}


/******************************************************************************
 * strstrip - Removes leading and trailing spaces from a string
 *
 * ENT:	istr	- string pointer to strip
 *
 * RET:	*istr
 */

char *strstrip(char *istr) {
	char *tmp = istr;
	char *rtmp;

	int len = strlen(istr);
	if (len < 1)
		return istr;
	rtmp = istr + (len - 1);
	
	while (*rtmp == ' ') *(rtmp--) = 0;
	while (*tmp == ' ') tmp++;
	memmove(istr, tmp, (rtmp - tmp) + 1);
	istr[(rtmp - tmp) + 1] = 0;

	return istr;
}


/******************************************************************************
 * stristr - Scans a string for the occurrence of a given substring, no case
 *
 * ENT:	scans s1 for the first occurrence of the substring s2, ingnoring case
 *
 * RET:	a pointer to the element in s1, where s2 begins (points to s2 in s1).
 *			If s2 does not occur in s1, returns null.
 */

const char *stristr(const char *s1, const char *s2) {
	int tLen = strlen(s2);
	int cLen = strlen(s1);
	char *target = new char [ tLen + 1 ];
	int i, j;
	const char *retVal = 0;

	strcpy(target, s2);
	for (i = 0; i < tLen; i++)
		target[i] = SW_toupper(target[i]);

	for (i = 0; i < (cLen - tLen)+1; i++) {
		if (SW_toupper(s1[i]) == (unsigned char)*target) {
			for (j = 1; j < tLen; j++) {
				if (SW_toupper(s1[i+j]) != (unsigned char)target[j])
					break;
			}
			if (j == tLen) {
				retVal = s1+i;
				break;
			}
		}
	}
	delete [] target;
	return retVal;
}

/******************************************************************************
 * strnicmp - compares the first n bytes of 2 string ignoring case
 *
 * ENT:	compares s1 to s2 comparing the first n byte ingnoring case
 *
 * RET:	same as strcmp
 */

const char strnicmp(const char *s1, const char *s2, int len) {

     int tLen = strlen(s2);
     int cLen = strlen(s1);
     char diff;
     int i;
     for (i = 0; ((i < len) && (i < tLen) && (i < cLen)); i++) {
     	if ((diff = SW_toupper(*s1) - SW_toupper(*s2)))
          	return diff;
	s1++;
	s2++;
     }
     return (i < len) ? cLen - tLen : 0;
}

/******************************************************************************
 * strlenw - Scans a string for trailing 0x0000 and return size in BYTES
 *
 * ENT:	target - string for which to determine size
 *
 * RET:	length in BYTES 
 *			If s2 does not occur in s1, returns null.
 */

unsigned int strlenw(const char *s1) {
	return strlen(s1);
// utf8 says no null in string except terminator, so below code is overkill
/*
	const char *ch = s1;
	if (!*ch)
		ch++;
	while (*ch) {
		ch++;
		if (!*ch)
			ch++;
	}
	return (unsigned int)(ch - s1) - 1;
*/
}


/******************************************************************************
 * toupperstr - converts a string to uppercase string
 *
 * ENT:	target - string to convert
 *
 * RET:	target
 */

char *toupperstr(char *buf) {
	char *ret = buf;
	while (*buf)
		*buf = SW_toupper(*buf++);

	return ret;
}
