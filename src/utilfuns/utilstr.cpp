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
		*ipstr = new char [ strlen(istr) + 1 ];
		strcpy(*ipstr, istr);
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

	if (strlen(istr) < 1)
		return istr;
	rtmp = istr + (strlen(istr) - 1);
	
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
		target[i] = toupper(target[i]);

	for (i = 0; i < (cLen - tLen)+1; i++) {
		if (toupper(s1[i]) == (unsigned char)*target) {
			for (j = 1; j < tLen; j++) {
				if (toupper(s1[i+j]) != (unsigned char)target[j])
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

