/******************************************************************************
 *
 * Latin1UTF16 -	SWFilter decendant to convert a Latin-1 character to UTF-16
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <latin1utf16.h>

Latin1UTF16::Latin1UTF16() {
}


char Latin1UTF16::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
    unsigned char *from;
    unsigned short *to;
    int len;
    
    len = strlen(text) + 1;						// shift string to right of buffer
    if (len < maxlen) {
      memmove(&text[maxlen - len], text, len);
      from = (unsigned char*)&text[maxlen - len];
    }
    else	
      from = (unsigned char*)text;
    // -------------------------------
    
    for (to = (unsigned short*)text; *from; from++) {
        switch (*from) {
	case 0x80: // '€'
		*to++ = 0x20AC;
		break;
	case 0x82: // '‚'
		*to++ = 0x201A;
		break;
	case 0x83: // 'ƒ'
		*to++ = 0x0192;
		break;
	case 0x84: // '„'
		*to++ = 0x201E;
		break;
	case 0x85: // '…'
		*to++ = 0x2026;
		break;
	case 0x86: // '†'
		*to++ = 0x2020;
		break;
	case 0x87: // '‡'
		*to++ = 0x2021;
		break;
	case 0x88: // 'ˆ'
		*to++ = 0x02C6;
		break;
	case 0x89: // '‰'
		*to++ = 0x2030;
		break;
	case 0x8A: // 'Š'
		*to++ = 0x0160;
		break;
	case 0x8B: // '‹'
		*to++ = 0x2039;
		break;
	case 0x8C: // 'Œ'
		*to++ = 0x0152;
		break;
	case 0x8E: // 'Ž'
		*to++ = 0x017D;
		break;
	case 0x91: // '‘'
		*to++ = 0x2018;
		break;
	case 0x92: // '’'
		*to++ = 0x2019;
		break;
	case 0x93: // '“'
		*to++ = 0x201C;
		break;
	case 0x94: // '”'
		*to++ = 0x201D;
		break;
	case 0x95: // '•'
		*to++ = 0x2022;
		break;
	case 0x96: // '–'
		*to++ = 0x2013;
		break;
	case 0x97: // '—'
		*to++ = 0x2014;
		break;
	case 0x98: // '˜'
		*to++ = 0x02DC;
		break;
	case 0x99: // '™'
		*to++ = 0x2122;
		break;
	case 0x9A: // 'š'
		*to++ = 0x0161;
		break;
	case 0x9B: // '›'
		*to++ = 0x203A;
		break;
	case 0x9C: // 'œ'
		*to++ = 0x0153;
		break;
	case 0x9E: // 'ž'
		*to++ = 0x017E;
		break;
	case 0x9F: // 'Ÿ'
		*to++ = 0x0178;
		break;
        default:
               *to++ = (unsigned short)*from;
        }
    }
    *to = 0;
    return 0;
}
