/*****************************************************************
 * outline.c
 *
 * Copyright 1999, Clark Cooper
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the license contained in the
 * COPYING file that comes with the expat distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Read an XML document from standard input and print an element
 * outline on standard output.
 */


#include <stdio.h>
#include <expat.h>
#include "versekey2.h"
//#include <map>

using namespace sword;

#define BUFFSIZE        8192
#define MAXOB 92
#define TESTAMENT_HEADING 255
#define NOTINREFSYS -1


char Buff[BUFFSIZE];

int Depth;
VerseKey2 vk;
const char *abbrevs[MAXOB+1];
int totalchaps = 0;

//struct bkref newbks[96], *newcps;
sword::bkref newbks[MAXOB+1], *newcps;

void init() {
	for (int i=0; i <= MAXOB; i++) {
		//printf("%s\n", vk.getPrefAbbrev(i));
		abbrevs[i] = vk.getPrefAbbrev(i);
		newbks[i].offset = 0;
		newbks[i].maxnext = 0;
	}
}

void outputbks() {
	for (int i = 0; i <= MAXOB; i++) {
		printf("book %d, offset %d, maxchapters %d\n", i, newbks[i].offset, newbks[i].maxnext);
	}
}

void outputcps() {
	for (int i = 0; i <= MAXOB; i++) {
		printf("book %d: (%d:%d){", i, newbks[i].offset, newbks[i].maxnext);
		if (newbks[i].offset != NOTINREFSYS && newbks[i].maxnext != TESTAMENT_HEADING)
			for (int j = 0; j <= newbks[i].maxnext; j++)
			{
				printf("{%d,%d}, ", newcps[j+newbks[i].offset].offset, newcps[j+newbks[i].offset].maxnext);
				if (j && j%5==0)
					printf("\n");
			}
		printf("}\n\n", i);
	}
}


void makebksoffsets() {
	int lastgood = 0, lastnext = 0;
	for (int i = 1; i <= MAXOB; i++) {
		if (newbks[i].maxnext == 0)
			newbks[i].offset = NOTINREFSYS; 
		else if (newbks[i-1].maxnext == TESTAMENT_HEADING) {
			newbks[i].offset = lastgood + 1;
			lastgood = newbks[i].offset;
			lastnext = newbks[i].maxnext;
			totalchaps = newbks[i].offset + newbks[i].maxnext + 1;
		}
		else
		{
			newbks[i].offset = lastgood + lastnext + 1;
			lastgood = newbks[i].offset;
			if (newbks[i].maxnext == TESTAMENT_HEADING)
				lastnext = 0;
			else
			{
				totalchaps = newbks[i].offset + newbks[i].maxnext + 1;
				lastnext = newbks[i].maxnext;
			}
		}
	}
}

void makecpsoffsets() {
	int lastone;
	newcps[0].offset = 1;
	for (int i = 1; i <= MAXOB; i++) {
		if (newbks[i].offset != NOTINREFSYS) {
			int chapoff = newbks[i].offset;
			if ((i>1) && newbks[i-1].maxnext == TESTAMENT_HEADING) {
				//printf("setting %d with %d\n", chapoff-1, chapoff-2);
				newcps[chapoff-1].offset = newcps[chapoff-2].offset + 
					newcps[chapoff-2].maxnext + 2;
			}
			if (newbks[i].maxnext != TESTAMENT_HEADING) {
				for (int j=0; j <= newbks[i].maxnext; j++)
				{
					newcps[chapoff+j].offset = newcps[chapoff+j-1].offset + 
						newcps[chapoff+j-1].maxnext + 1;
					//printf("setting %d with %d\n", chapoff+j, chapoff+j-1);
				}
			}
		}
	}
}

void outputfiles(const char *fname) {
	SWBuf bksbuf = fname; 
	bksbuf += ".bks";
	SWBuf cpsbuf = fname;
	cpsbuf += ".cps";
	FILE *bksfile = fopen(bksbuf.c_str(), "w");
	if (!bksfile)
	{
		fprintf(stderr, "Couldn't open file to parse %s\n", bksbuf.c_str());
		return;
	}
	int numbkswrite = fwrite(newbks, sizeof(sword::bkref), MAXOB+1, bksfile);
	if (numbkswrite != MAXOB+1)
	{
		printf("only wrote %d of %d entries\n", numbkswrite, MAXOB+1);
	}
	fclose(bksfile);
	FILE *cpsfile = fopen(cpsbuf.c_str(), "w");
	if (!cpsfile)
	{
		fprintf(stderr, "Couldn't open file to parse %s\n", cpsbuf.c_str());
		return;
	}
	int numcpswrite = fwrite(newcps, sizeof(sword::bkref), totalchaps, cpsfile);
	if (numcpswrite != totalchaps)
	{
		printf("only wrote %d of %d entries\n", numcpswrite, totalchaps);
	}
	fclose(cpsfile);
}

static void
bksstart(void *data, const char *el, const char **attr)
{
  int book=0, chapter=0, verse=0;
  char id[128];

  //for (i = 0; i < Depth; i++)
  //  printf("%d%d", i, i);

  //printf("%s", el);
  if (Depth) {
   for (int i = 0; attr[i]; i += 2) {
    //printf(" %s='%s'", attr[i], attr[i + 1]);
	sscanf(attr[i + 1], "%[^.].%d.%d", &id, &chapter, &verse);
	if (!strncmp("GrEsth", attr[i + 1], 6)) {
		book = 87;
	}
	else if (!strncmp("PssSol", attr[i + 1], 6)) {
		book = 82;
	}
	else if (!strncmp("Ps151", attr[i + 1], 5)) {
		book = 92;
	}
	else for (int j=0; j <= MAXOB; j++) {
		if (!strncmp(abbrevs[j], attr[i + 1], strlen(abbrevs[j])))
		{
			book = j;
			break;
		}
	}
	//printf("%s(%d:%d:%d:%s)\n", attr[i + 1], book, chapter, verse, id);
	if (newbks[book].maxnext < chapter)
		newbks[book].maxnext = chapter;
   }
  //printf("\n");
  }

  Depth++;
}

static void
cpsstart(void *data, const char *el, const char **attr)
{
  int book=0, chapter=0, verse=0;
  char id[128];

  //for (i = 0; i < Depth; i++)
  //  printf("%d%d", i, i);

  //printf("%s", el);
  if (Depth) {
   for (int i = 0; attr[i]; i += 2) {
    //printf(" %s='%s'", attr[i], attr[i + 1]);
	sscanf(attr[i + 1], "%[^.].%d.%d", &id, &chapter, &verse);
	if (!strncmp("GrEsth", attr[i + 1], 6)) {
		book = 87;
	}
	else if (!strncmp("PssSol", attr[i + 1], 6)) {
		book = 82;
	}
	else if (!strncmp("Ps151", attr[i + 1], 5)) {
		book = 92;
	}
	else for (int j=0; j <= MAXOB; j++) {
		if (!strncmp(abbrevs[j], attr[i + 1], strlen(abbrevs[j])))
		{
			book = j;
			break;
		}
	}
	//printf("%s(%d:%d:%d:%s)\n", attr[i + 1], book, chapter, verse, id);
	if (newcps[newbks[book].offset + chapter].maxnext < verse)
		newcps[newbks[book].offset + chapter].maxnext = verse;
   }
  //printf("\n");
  }

  Depth++;
}


static void
end(void *data, const char *el)
{
  Depth--;
}

void runparser(FILE *parsefile, XML_Parser p) {
  for (;;) {
    int done;
    int len;

    len = fread(Buff, 1, BUFFSIZE, parsefile);
    if (ferror(parsefile)) {
      fprintf(stderr, "Read error\n");
      exit(-1);
    }
    done = feof(parsefile);

    if (XML_Parse(p, Buff, len, done) == XML_STATUS_ERROR) {
      fprintf(stderr, "Parse error at line %d:\n%s\n",
              XML_GetCurrentLineNumber(p),
              XML_ErrorString(XML_GetErrorCode(p)));
      exit(-1);
    }

    if (done)
      break;
  }
}

int
main(int argc, char *argv[])
{
	init();
  XML_Parser p = XML_ParserCreate(NULL);
  if (! p) {
    fprintf(stderr, "Couldn't allocate memory for parser\n");
    exit(-1);
  }
  
  FILE *parsefile = fopen(argv[1], "r");
  if (!parsefile)
  {
    fprintf(stderr, "Couldn't open file to parse %s\n", argv[1]);
    exit(-1);
  }

  XML_SetElementHandler(p, bksstart, end);
  printf("first run through\n");
  runparser(parsefile, p);
  newbks[0].maxnext = TESTAMENT_HEADING;
  newbks[40].maxnext = TESTAMENT_HEADING;
  newbks[68].maxnext = TESTAMENT_HEADING;
  newbks[86].maxnext = TESTAMENT_HEADING;
  makebksoffsets();
  outputbks();
  rewind(parsefile);
  XML_ParserFree(p);
  XML_Parser q = XML_ParserCreate(NULL);
  if (! q) {
    fprintf(stderr, "Couldn't allocate memory for parser\n");
    exit(-1);
  }
  printf("totalchaps is %d\n", totalchaps);
  newcps = new sword::bkref[totalchaps]; 
  for (int i=0; i<totalchaps; i++) {
	  newcps[i].offset = 0;
	  newcps[i].maxnext = 0;
  }
	  
  XML_SetElementHandler(q, cpsstart, end);
  printf("second run through\n");
  runparser(parsefile, q);
  fclose(parsefile);
  XML_ParserFree(q);
  makecpsoffsets();
  outputcps();
  outputfiles(argv[2]);
  delete [] newcps;
  return 0;
}
