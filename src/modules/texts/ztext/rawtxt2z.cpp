// Compression on variable granularity

#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <string>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <zlib.h>
#include <versekey.h>

int iBufSize, ulBuffNum;
ofstream cfile;
ofstream cfile2;

int ofd[2], oxfd[2], ovxfd[2];
int ifd[2], ixfd[2];
int itestfd[2], itestxfd[2];
unsigned long ulIOff=0, ulCOff=0, ulFOff=0, ulNone=0;
string currbuff="";


int openreadfile(char *buffer, char *path, const char *filename)
{
	int filenum;
	sprintf(buffer, "%s/%s", path, filename);
	cfile << buffer << "\n";
	filenum =  open(buffer, O_RDONLY|O_BINARY);
	if (filenum > 0)
	{
		return filenum;
	}
	else
	{
		cerr << "failed to open file to read\n";
		exit(-1);
	}
}

int openwritefile(char *buffer, char *path, const char *filename)
{
	int filenum;
	sprintf(buffer, "%s/%s", path, filename);
	cfile << buffer << "\n";
	filenum =  open(buffer, O_WRONLY|O_BINARY|O_CREAT|O_TRUNC);
	if (filenum > 0)
	{
		return filenum;
	}
	else
	{
		cerr << "failed to open file to read\n";
		exit(-1);
	}
}

int bytebound(unsigned long offset, VerseKey &thekey)
{
	unsigned long bufferoff;
	cfile << "byteboundtest " << thekey << "\n";
	bufferoff = iBufSize * (ulBuffNum+1);
	if (offset > bufferoff)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int versebound(unsigned long offset, VerseKey &thekey)
{
	cfile << "verseboundtest " << thekey << "\n";
	return 1;
}

int chapterbound(unsigned long offset, VerseKey &thekey)
{
	VerseKey testkey;
	testkey = thekey;
	testkey++;
	//cfile << "chapterboundtest " << testkey;
	if (testkey.Verse()==1 || (!thekey.compare("Revelation of John 22:21")))
	{
		//cfile << " 1\n";
		return 1;
	}
	else
	{
		//cfile << " 0\n";
		return 0;
	}
}

int bookbound(unsigned long offset, VerseKey &thekey)
{
	VerseKey testkey;
	testkey = thekey;
	cfile << "bookboundtest " << testkey << "\n";
	testkey++;
	if (testkey.Chapter()==1 || (!thekey.compare("Revelation of John 22:21")))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


typedef int (*boundfunc)(unsigned long offset, VerseKey &thekey);

int writeblock(int i)
{
	char *destbuff=NULL;
	unsigned long compsize = 0, buffsize=0;

	cfile << "compressing block\n";
	// compress current buffer
	buffsize = currbuff.length();
	write(itestfd[i], currbuff.c_str(), buffsize);
	compsize = (unsigned long) (buffsize*1.01)+20;  // at least 1% bigger than buffer + 12 bytes
	//cfile << "{" << compsize << "}";
	//destbuff = (char *) calloc(compsize + 1, 1);
	destbuff = new char[compsize];
	if (compress((Bytef*)destbuff, &compsize, (const Bytef*)currbuff.c_str(), buffsize)!=Z_OK)
	{
		cerr << "Could not compress buffer: exiting\n";
		delete[] destbuff;
		exit(-1);
	}
	//cout << "Compressed buffer{" << compsize << "}\n" << destbuff << "\n";
	//cout.flush();
	// write to compressed file index
	ulCOff = lseek(ofd[i], 0, SEEK_END);
	write(oxfd[i], &ulCOff, 4);    // offset in compressed file
	write(oxfd[i], &compsize, 4);	 // compressed size
	write(oxfd[i], &buffsize, 4);	 // uncompressed size
	cfile << buffsize << " -> " << compsize << "\n";
	cfile2 << "Compressed{" << compsize << "}\n" << destbuff << "\n";
	cfile2.flush();

	//write compressed buffer to file
	write(ofd[i], destbuff, compsize);

	//free(destbuff);
	delete[] destbuff;

	currbuff = "";
	ulBuffNum++;
	ulIOff = 0;
	return 1;
}



int main(int argc, char **argv)
{
	VerseKey key1, key2, key3;
	int i;
	char xbuff[64];
	unsigned long offset;
	unsigned short size=0;
	unsigned long ulsize=0;
	char *tmpbuf=NULL;
	int iType;
	boundfunc blockbound[4] = {bytebound, versebound, chapterbound, bookbound};
	bool newbook=true, newchapter=true, newtestament = true, newmodule = true, lasttodo=true;

	if ((argc < 2) || (argc > 4)) {
		cerr << "usage: " << argv[0] << " datapath [compression type [buffer size]]\n";
		exit(1);
	}

	if (argc>2)
	{
		iType = atoi(argv[2]);
		if (argc==4)
		{
			iBufSize = atoi(argv[3]);
		}
		else
		{
			iBufSize = 1;
		}
	}
	else
	{
		iType = 2;
		iBufSize = 1;
	}

	cfile.open("raw2z.log", ios::out);
	if (!cfile.is_open())
	{
		cerr << "Failed to open log file\n";
		exit(-1);
	}
	cfile2.open("raw2z.lg2", ios::out);
	if (!cfile2.is_open())
	{
		cerr << "Failed to open log file\n";
		exit(-1);
	}
	cfile << iType << " " << iBufSize << "\n";

	if ((iType<=0) || (iType > 4) || !iBufSize || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help") || !strcmp(argv[1], "/?") || !strcmp(argv[1], "-help"))
	{
		cfile << argv[0] << " - a tool to create compressed Sword modules\n";
		cfile << "version 0.1\n\n";
		cfile << "usage: "<< argv[0] << " datapath [compression type [buffer size]]\n\n";
		cfile << "datapath: the directory in which to find the raw module\n";
		cfile << "compression type: (default 2)\n" << "   1 - bytes\n" << "   2 - verses\n" << "   3 - chapters\n" << "   4 - books\n";
		cfile << "buffer size (default 1): the number of the compression type in each block\n";
		exit(1);
	}

	//zobj = new SWCompress();
	//rawdrv = new RawVerse(argv[1]);

#ifndef O_BINARY
#define O_BINARY 0
#endif
	cfile << "opening files\n";

	tmpbuf = new char [ strlen(argv[1]) + 11 ];

	//original files
	ifd[0] = openreadfile(tmpbuf, argv[1], "ot");
	ixfd[0] = openreadfile(tmpbuf, argv[1], "ot.vss");
	ifd[1] = openreadfile(tmpbuf, argv[1], "nt");
	ixfd[1] = openreadfile(tmpbuf, argv[1], "nt.vss");

switch ( iType) {
	case 1 :
		ofd[0] = openwritefile(tmpbuf, argv[1], "ot.rzz");
		oxfd[0] = openwritefile(tmpbuf, argv[1], "ot.rzs");
		ovxfd[0] = openwritefile(tmpbuf, argv[1], "ot.rzv");
		ofd[1] = openwritefile(tmpbuf, argv[1], "nt.rzz");
		oxfd[1] = openwritefile(tmpbuf, argv[1], "nt.rzs");
		ovxfd[1] = openwritefile(tmpbuf, argv[1], "nt.rzv");
		//boundfunc = bytebound;
		break;
	case 2 :
		ofd[0] = openwritefile(tmpbuf, argv[1], "ot.vzz");
		oxfd[0] = openwritefile(tmpbuf, argv[1], "ot.vzs");
		ovxfd[0] = openwritefile(tmpbuf, argv[1], "ot.vzv");
		ofd[1] = openwritefile(tmpbuf, argv[1], "nt.vzz");
		oxfd[1] = openwritefile(tmpbuf, argv[1], "nt.vzs");
		ovxfd[1] = openwritefile(tmpbuf, argv[1], "nt.vzv");
		break;
	case 3 :
		ofd[0] = openwritefile(tmpbuf, argv[1], "ot.czz");
		oxfd[0] = openwritefile(tmpbuf, argv[1], "ot.czs");
		ovxfd[0] = openwritefile(tmpbuf, argv[1], "ot.czv");
		ofd[1] = openwritefile(tmpbuf, argv[1], "nt.czz");
		oxfd[1] = openwritefile(tmpbuf, argv[1], "nt.czs");
		ovxfd[1] = openwritefile(tmpbuf, argv[1], "nt.czv");
		break;
	case 4 :
		ofd[0] = openwritefile(tmpbuf, argv[1], "ot.bzz");
		oxfd[0] = openwritefile(tmpbuf, argv[1], "ot.bzs");
		ovxfd[0] = openwritefile(tmpbuf, argv[1], "ot.bzv");
		ofd[1] = openwritefile(tmpbuf, argv[1], "nt.bzz");
		oxfd[1] = openwritefile(tmpbuf, argv[1], "nt.bzs");
		ovxfd[1] = openwritefile(tmpbuf, argv[1], "nt.bzv");
		break;
	default:
		cerr << "Unknown compression type\n";
		exit(-1);
}
	itestfd[0] = openwritefile(tmpbuf, argv[1], "ot.tst");
	itestfd[1] = openwritefile(tmpbuf, argv[1], "nt.tst");
	itestxfd[0] = openwritefile(tmpbuf, argv[1], "ot.tdx");
	itestxfd[1] = openwritefile(tmpbuf, argv[1], "nt.tdx");


	delete [] tmpbuf;

	//cfile << "about to start\n";

for ( i=0; i<2; i++)
{
	ulIOff=0, ulBuffNum=0;
	currbuff = "";
	key1 = (i == 1) ? "Matthew 1:1" : "Genesis 1:1";
	key2 = key3 = key1;
	newtestament = true;

	cfile << "key: " << key1 << " Testament {" << key1.Testament()-1 << "}\n";
	//cfile << "Chapter {" << key.Chapter() << "}\n";
	//cfile << "Verse {" << key.Verse() << "}\n";
	//cfile << key.compare("Revelation of John 22:21") << "\n";
	//cfile << key.compare("Genesis 1:1") << "\n";
	do
	{
		//cfile << "ok";
		// read current verse offset
		if (read(ixfd[i], &offset, 4) != 4)
		{
			cfile << "Failed to read input verse offsets?\n";
			break;
		}
		if (read(ixfd[i], &size, 2) != 2)
		{
			cfile << "Failed to read input verse sizes?\n";
			break;
		}
		cfile << "key:" << key1 << " offset:" << offset << " size:" << size << "\n";
		sprintf(xbuff, "key{%s} offset{%ld} size{%d}\n", (const char *)key1, offset, size);
		write(itestxfd[i], &xbuff, strlen(xbuff));
		ulsize = size;
		if (!offset && !size)
		{
			//Check for module header
			if (read(ixfd[i], &ulIOff, 4) != 4)
			{
				cfile << "Failed to read input verse offsets?\n";
				break;
			}
			ulsize = ulIOff;
			ulIOff = 0;
			lseek(ixfd[i], 6, SEEK_SET);
		}

		if (ulsize)
		{
			// read current verse and add to current buffer
			tmpbuf = (char *) calloc(ulsize + 1, 1);
			lseek(ifd[i], offset, SEEK_SET);
			read(ifd[i], tmpbuf, ulsize);
			currbuff += tmpbuf;
			//cfile << currbuff << "\n";

			// write to verse index into compressed
			write(ovxfd[i], &ulBuffNum, 4);    // current buffer number
			write(ovxfd[i], &ulIOff, 4);    // offset within the buffer
			write(ovxfd[i], &size, 2);    // verse size

			ulFOff = lseek(ofd[i], 0, SEEK_CUR) + size;
			if (key1.compare("Revelation of John 22:21")!=-1)
			{
				lasttodo = false;
			}
			if (blockbound[iType-1](ulFOff, key1)/*at block boudary*/)
			{
				writeblock(i);
				/*
				cfile << "compressing block\n";
				// compress current buffer
				buffsize = currbuff.length();
				write(itestfd[i], currbuff.c_str(), buffsize);
				compsize = (unsigned long) (buffsize*1.01)+20;  // at least 1% bigger than buffer + 12 bytes
				//cfile << "{" << compsize << "}";
				//destbuff = (char *) calloc(compsize + 1, 1);
				destbuff = new char[compsize];
				if (compress((Bytef*)destbuff, &compsize, (const Bytef*)currbuff.c_str(), buffsize)!=Z_OK)
				{
					cerr << "Could not compress buffer: exiting\n";
					delete[] destbuff;
					exit(-1);
				}
				//cout << "Compressed buffer{" << compsize << "}\n" << destbuff << "\n";
				//cout.flush();
				// write to compressed file index
				ulCOff = lseek(ofd[i], 0, SEEK_END);
				write(oxfd[i], &ulCOff, 4);    // offset in compressed file
				write(oxfd[i], &compsize, 4);	 // compressed size
				write(oxfd[i], &buffsize, 4);	 // uncompressed size
				cfile << buffsize << " -> " << compsize << "\n";
				cfile2 << "Compressed{" << compsize << "}\n" << destbuff << "\n";
				cfile2.flush();

				//write compressed buffer to file
				write(ofd[i], destbuff, compsize);

				//free(destbuff);
				delete[] destbuff;

				currbuff = "";
				ulBuffNum++;
				ulIOff = 0;
				*/
			}
			else
			{
				ulIOff += ulsize;
			}
			free(tmpbuf);

			if (newmodule)
			{
				newmodule = false;
				cfile << "had a new module " << (const char *) key1 << "{" << offset << "}\n";
				writeblock(i);
			}
			else if (newtestament)
			{
				newtestament = false;
				cfile << "had a new testament " << (const char *) key1 << "{" << offset << "}\n";
			}
			else if (newbook)
			{
				newbook = false;
				cfile << "had a new book " << (const char *) key1 << "{" << offset << "}\n";
			}
			else if (newchapter)
			{
				newchapter = false;
				cfile << "had a new chapter " << (const char *) key1 << "{" << offset << "}\n";
			}
			else
			{
				key1++;
			}

			if (key1.Chapter()!=key2.Chapter() || (key1.Book()!=key2.Book()))
			{
				newchapter = true;
				cfile << "got a new chapter " << (const char *) key1 << "\n";
			}
			if (key1.Book()!=key2.Book())
			{
				newbook = true;
				cfile << "got a new book " << (const char *) key1 << "\n";
			}
			key2 = key1;

		}
		else
		{
			cfile << "empty offset\n";
			// write to verse index into compressed
			write(ovxfd[i], &ulNone, 4);    // current buffer number
			write(ovxfd[i], &size, 2);    // verse size
			write(ovxfd[i], &ulNone, 4);    // offset within the buffer
		}
	}
	while ( (key1.Testament()==i+1) && ((key1.compare("Revelation of John 22:21")==-1) || (lasttodo)));

	close(ifd[i]);
	close(ofd[i]);
	close(ixfd[i]);
	close(oxfd[i]);
	close(ovxfd[i]);
	close(itestfd[i]);
	close(itestxfd[i]);
}
	return 1;
}
