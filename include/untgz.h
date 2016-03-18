#ifndef UNTGZ_H
#define UNTGZ_H

#include <zlib.h>

#ifdef __cplusplus
extern "C" {
#endif

int untargz(int fd, const char *dest);
int untar(gzFile in, const char *dest);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif
