/******************************************************************************
 *
 *  canon_null.h -    Versification data for the null system
 *
 * Copyright 2009-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#ifndef SWORDXX_CANON_NULL_H
#define SWORDXX_CANON_NULL_H

namespace swordxx {

// Versification system: null
// Book order:
// This is not a real versification system. Rather, use these [on]tbooks values for other versifications that are missing entire testaments.

/******************************************************************************
 * [on]tbooks_null - initialize static instance for all canonical
 *         text names and chapmax
 */
struct sbook otbooks_null[] = {
  {"", "", "", 0, nullptr}
};

struct sbook ntbooks_null[] = {
  {"", "", "", 0, nullptr}
};

} /* namespace swordxx */

#endif /* SWORDXX_CANON_NULL_H */
