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

#ifndef SWIO_H
#define SWIO_H

#include <stdio.h>
#include <unistd.h>

namespace swordxx {

void swread(int fd, void * buf, size_t nbytes);
void swwrite(int fd, const void *buf, size_t nbyte);
void swfgets(char *str, int n, FILE *stream);

} /* namespace swordxx */
#endif
