/*
 * roman.c
 * Copyright 2001 by CrossWire Bible Society
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, 
 * MA  02111-1307, USA.
 * 
 */

#include <string.h>

char isroman (const char* str) {
	char * ch = (char*)str;
		for (; *ch; ch++)
		if (!strchr ("IVXLCDMivxlcdm ", *ch))
			return 0;
	return 1;
}

int from_rom(const char* str) {
	int i, n = strlen(str);
	short * num= calloc(n, sizeof(short));
	for (i = 0; str[i]; i++) {
		switch(str[i]) {
		case 'i':
		case 'I':
			num[i] = 1;
			break;
		case 'v':
		case 'V':
			num[i] = 5;
			break;
		case 'x':
		case 'X':
			num[i] = 10;
			break;
		case 'l':
		case 'L':
			num[i] = 50;
			break;
		case 'c':
		case 'C':
			num[i] = 100;
			break;
		case 'd':
		case 'D':
			num[i] = 500;
			break;
		case 'm':
		case 'M':
			num[i] = 1000;
			break;
		default:
			num[i] = 0;
		}
	}
	for (i = 1; str[i]; i++) {
		if (num[i] > num[i-1])  {
			num[i] -= num[i-1];
			num[i-1] = 0;
		}
	}
	n = 0;
	for (i = 0; str[i]; i++) {
		n += num[i];
	}
	return n;
};