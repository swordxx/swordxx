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

class sapphire {
public:
    sapphire(unsigned char *key = 0, unsigned char keysize=0);
    ~sapphire();
    void initialize(unsigned char *key, unsigned char keysize);
    unsigned char encrypt(unsigned char b = 0);
    unsigned char decrypt(unsigned char b);
};
