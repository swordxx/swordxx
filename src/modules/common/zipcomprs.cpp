/******************************************************************************
 *  swcomprs.cpp   - code for class 'ZipCompress'- a driver class that provides
 *				compression utilities. - using zlib
 */

#include <string.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <zipcomprs.h>
#include <zlib.h>

/******************************************************************************
 * ZipCompress Constructor - Initializes data for instance of ZipCompress
 *
 */

ZipCompress::ZipCompress() : SWCompress()
{
	printf("init compress\n");
}


/******************************************************************************
 * ZipCompress Destructor - Cleans up instance of ZipCompress
 */

ZipCompress::~ZipCompress()
{
}


/******************************************************************************
 * ZipCompress::Encode	- This function "encodes" the input stream into the
 *						output stream.
 *						The GetChars() and SendChars() functions are
 *						used to separate this method from the actual
 *						i/o.
 */

void ZipCompress::Encode(void)
{
/*
ZEXTERN int ZEXPORT compress OF((Bytef *dest,   uLongf *destLen,
                                 const Bytef *source, uLong sourceLen));
     Compresses the source buffer into the destination buffer.  sourceLen is
   the byte length of the source buffer. Upon entry, destLen is the total
   size of the destination buffer, which must be at least 0.1% larger than
   sourceLen plus 12 bytes. Upon exit, destLen is the actual size of the
   compressed buffer.
     This function can be used to compress a whole file at once if the
   input file is mmap'ed.
     compress returns Z_OK if success, Z_MEM_ERROR if there was not
   enough memory, Z_BUF_ERROR if there was not enough room in the output
   buffer.
*/
	direct = 0;	// set direction needed by parent [Get|Send]Chars()

	// get buffer
	char chunk[1024];
	char *buf = (char *)calloc(1, 1024);
	char *chunkbuf = buf;
	int chunklen;
	unsigned long len = 0;
	while((chunklen = GetChars(chunk, 1023))) {
		memcpy(chunkbuf, chunk, chunklen);
		chunkbuf += chunklen;
		len += chunklen;
		if (chunklen < 1023)
			break;
		else	buf = (char *)realloc(buf, len + 1024);
	}


	zlen = (long) (len*1.001)+15;
	char *zbuf = new char[len];
	if (len)
	{
		//printf("Doing compress\n");
		if (compress((Bytef*)zbuf, &zlen, (const Bytef*)buf, len)!=Z_OK)
		{
			printf("ERROR in compression\n");
		}
		else {
			SendChars(zbuf, zlen);
		}
	}
	else
	{
		printf("No buffer to compress\n");
	}
	delete [] zbuf;
	free (buf);
}


/******************************************************************************
 * ZipCompress::Decode	- This function "decodes" the input stream into the
 *						output stream.
 *						The GetChars() and SendChars() functions are
 *						used to separate this method from the actual
 *						i/o.
 */

void ZipCompress::Decode(void)
{
/*
ZEXTERN int ZEXPORT uncompress OF((Bytef *dest,   uLongf *destLen,
                                   const Bytef *source, uLong sourceLen));
     Decompresses the source buffer into the destination buffer.  sourceLen is
   the byte length of the source buffer. Upon entry, destLen is the total
   size of the destination buffer, which must be large enough to hold the
   entire uncompressed data. (The size of the uncompressed data must have
   been saved previously by the compressor and transmitted to the decompressor
   by some mechanism outside the scope of this compression library.)
   Upon exit, destLen is the actual size of the compressed buffer.
     This function can be used to decompress a whole file at once if the
   input file is mmap'ed.

     uncompress returns Z_OK if success, Z_MEM_ERROR if there was not
   enough memory, Z_BUF_ERROR if there was not enough room in the output
   buffer, or Z_DATA_ERROR if the input data was corrupted.
*/

	// get buffer
	char chunk[1024];
	char *zbuf = (char *)calloc(1, 1024);
	char *chunkbuf = zbuf;
	int chunklen;
	unsigned long zlen = 0;
	while((chunklen = GetChars(chunk, 1023))) {
		memcpy(chunkbuf, chunk, chunklen);
		chunkbuf += chunklen;
		zlen += chunklen;
		if (chunklen < 1023)
			break;
		else	zbuf = (char *)realloc(zbuf, zlen + 1024);
	}

	//printf("Decoding complength{%ld} uncomp{%ld}\n", zlen, blen);
	if (zlen) {
		unsigned long blen = zlen*10;	// trust compression is less than 1000%
		char *buf = new char[blen]; 
		//printf("Doing decompress {%s}\n", zbuf);
		uncompress((Bytef*)buf, &blen, (Bytef*)zbuf, zlen);
		SendChars(buf, blen);
		delete [] buf;
	}
	else {
		printf("No buffer to decompress!\n");
	}
	//printf("Finished decoding\n");
	free (buf);
}
