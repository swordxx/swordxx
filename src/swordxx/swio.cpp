/******************************************************************************
 *
 * Copyright 2016
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#include "swio.h"
#include "swlog.h"

namespace swordxx {

void swread(int fd, void * buf, size_t nbytes) {
    ssize_t stat = read(fd, buf, nbytes);
    if (stat == -1)
        SWLog::getSystemLog()->logError("Error using \"read\" on a file.");
}

void swwrite(int fd, const void *buf, size_t nbyte) {
    ssize_t stat = write(fd, buf, nbyte);
    if (stat == -1)
        SWLog::getSystemLog()->logError("Error using \"write\" on a file.");
}

void swfgets(char *str, int n, FILE *stream) {
    char * stat = fgets(str, n, stream);
    if (stat == nullptr)
        SWLog::getSystemLog()->logError("Error using \"fgets\" on a file.");
}


} /* namespace swordxx */
