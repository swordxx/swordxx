// Include only if your UNIX compiler does not include stricmp but does include strcasecmp

#include <unixstr.h>

SWORD_NAMESPACE_START
int stricmp(const char *s1, const char *s2) {
	return strcasecmp(s1, s2);
}
SWORD_NAMESPACE_END
