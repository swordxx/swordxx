/******************************************************************************
 *
 * Placeholder for the REAL SAPPHIRE II Stream Cipher Files
 *
 * The SAPPHIRE II package may be obtained from within the United States or
 * Canada from:
 *
 *	http://cryptography.org/cgi-bin/crypto.cgi/libraries/sapphire.zip
 *
 *
 * These files must be placed accordingly:
 *
 *	.../sword/include/sapphire.h
 *	.../sword/src/modules/common/sapphire.cpp
 *
 * If you are outside of these countries, you will have to obtain this package
 * elsewhere.
 *
 */

#include "sapphire.h"

sapphire::sapphire(unsigned char *key, unsigned char keysize) {}
sapphire::~sapphire() {}

void sapphire::initialize(unsigned char *key, unsigned char keysize) {}
unsigned char sapphire::encrypt(unsigned char b) { return b; }
unsigned char sapphire::decrypt(unsigned char b) { return b; }
