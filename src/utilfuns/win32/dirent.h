/*

    Declaration of POSIX directory browsing functions and types for Win32.

    Kevlin Henney (mailto:kevlin@acm.org), March 1997.

    Copyright Kevlin Henney, 1997. All rights reserved.

    Permission to use, copy, modify, and distribute this software and its
    documentation for any purpose is hereby granted without fee, provided
    that this copyright and permissions notice appear in all copies and
    derivatives, and that no charge may be made for the software and its
    documentation except to cover cost of distribution.
    
*/

#ifndef DIRENT_INCLUDED
#define DIRENT_INCLUDED

#include <defs.h>

typedef struct DIR DIR;

struct SWDLLEXPORT dirent
{
    char *d_name;
};

SWDLLEXPORT DIR           *opendir(const char *);
SWDLLEXPORT int           closedir(DIR *);
SWDLLEXPORT struct dirent *readdir(DIR *);
SWDLLEXPORT void          rewinddir(DIR *);

#endif
