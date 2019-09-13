/******************************************************************************
 *
 * Copyright Jaak Ristioja
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

#ifndef SWORDXX_DEBUGONLY_H
#define SWORDXX_DEBUGONLY_H

#ifdef NDEBUG
#define SWORDXX_DEBUG_ONLY(...)
#define SWORDXX_NDEBUG_ONLY(...) __VA_ARGS__
#else
#define SWORDXX_DEBUG_ONLY(...) __VA_ARGS__
#define SWORDXX_NDEBUG_ONLY(...)
#endif

#endif /* SWORDXX_DEBUGONLY_H */
