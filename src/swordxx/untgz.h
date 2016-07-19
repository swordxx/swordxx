#ifndef UNTGZ_H
#define UNTGZ_H

#include <zlib.h>

#ifdef __cplusplus
extern "C" {
#endif

int untargz(int fd, const char *dest) __attribute__ ((visibility("internal")));

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif
