/******************************************************************************
 *
 * Placeholder for the REAL SAPPHIRE II Stream Cipher Files
 *
 * The SAPPHIRE II package may be obtained from within the United States or
 * Canada from:
 *
 *   http://cryptography.org/cgi-bin/crypto.cgi/libraries/sapphire.zip
 *
 *
 * These files must be placed accordingly:
 *
 *   .../sword/include/sapphire.h
 *   .../sword/src/modules/common/sapphire.cpp
 *
 * If you are outside of these countries, you will have to obtain this package
 * elsewhere.
 *
 */


#ifdef UNIX
#include <memory.h>
#include <unistd.h>
#else
#include <mem.h>
#endif
#include "sapphire.h"

unsigned char sapphire::keyrand(int limit,
                                unsigned char *user_key,
                                unsigned char keysize,
                                unsigned char *rsum,
                                unsigned *keypos)
    {
    return keysize;
    }

void sapphire::initialize(unsigned char *key, unsigned char keysize)
    {
    }

void sapphire::hash_init(void)
    {
    }

sapphire::sapphire(unsigned char *key, unsigned char keysize)
    {
    }

void sapphire::burn(void)
    {
    }

sapphire::~sapphire()
    {
    }

unsigned char sapphire::encrypt(unsigned char b)
    {
    return b;
    }

unsigned char sapphire::decrypt(unsigned char b)
    {
    return b;
    }

void sapphire::hash_final(unsigned char *hash,      // Destination
                          unsigned char hashlength) // Size of hash.
    {
    }

