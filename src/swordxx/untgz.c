/*
 * untgz.c -- Display contents and/or extract file from
 * a gzip'd TAR file
 * written by "Pedro A. Aranda Guti\irrez" <paag@tid.es>
 * adaptation to Unix by Jean-loup Gailly <jloup@gzip.org>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#else
# include <direct.h>
# include <io.h>
#endif

#include <zlib.h>

#ifdef WIN32
# include <windows.h>
#  ifndef F_OK
#    define F_OK (0)
#  endif
#  ifdef _MSC_VER
#    define mkdir(dirname,mode) _mkdir(dirname)
#    define unlink(fn)          _unlink(fn)
#  else
#    define mkdir(dirname,mode) _mkdir(dirname)
#  endif
#else
#  include <utime.h>
#endif


/* Values used in typeflag field.  */

#define REGTYPE     '0'        /* regular file */
#define AREGTYPE '\0'        /* regular file */
#define DIRTYPE  '5'        /* directory */

#define BLOCKSIZE 512

struct tar_header
{                /* byte offset */
  char name[100];        /*   0 */
  char mode[8];            /* 100 */
  char uid[8];            /* 108 */
  char gid[8];            /* 116 */
  char size[12];        /* 124 */
  char mtime[12];        /* 136 */
  char chksum[8];        /* 148 */
  char typeflag;        /* 156 */
  char linkname[100];        /* 157 */
  char magic[6];        /* 257 */
  char version[2];        /* 263 */
  char uname[32];        /* 265 */
  char gname[32];        /* 297 */
  char devmajor[8];        /* 329 */
  char devminor[8];        /* 337 */
  char prefix[155];        /* 345 */
                /* 500 */
};

union tar_buffer {
  char               buffer[BLOCKSIZE];
  struct tar_header  header;
};

/* help functions */

static int getoct(char *p,int width)
{
  int result = 0;
  char c;

  while (width --)
    {
      c = *p++;
      if (c == ' ')
    continue;
      if (c == 0)
    break;
      result = result * 8 + (c - '0');
    }
  return result;
}

/* recursive make directory */
/* abort if you get an ENOENT errno somewhere in the middle */
/* e.g. ignore error "mkdir on existing directory" */
/* */
/* return 1 if OK */
/*        0 on error */

static int makedir (char *newdir)
{
  size_t len = strlen(newdir);
  if (len <= 0u)
      return 0;
  char * buffer = malloc(len + 1u);
  if (!buffer)
      return 0;
  memcpy(buffer, newdir, len + 1u);
  char *p;

  if (buffer[len - 1u] == '/') {
    buffer[len - 1u] = '\0';
  }
  if (mkdir(buffer, 0775) == 0)
    {
      free(buffer);
      return 1;
    }

  p = buffer+1;
  while (1)
    {
      char hold;

      while(*p && *p != '\\' && *p != '/')
    p++;
      hold = *p;
      *p = 0;
      if ((mkdir(buffer, 0775) == -1) && (errno == ENOENT))
    {
      fprintf(stderr,"couldn't create directory %s\n",buffer);
      free(buffer);
      return 0;
    }
      if (hold == 0)
    break;
      *p++ = hold;
    }
  free(buffer);
  return 1;
}

/* Tar file list or extract */

static int untar (gzFile in, const char *dest) {
    union  tar_buffer buffer;
    int    len;
    int    err;
    int    getheader = 1;
    int    remaining = 0;
    FILE   *outfile = NULL;
    char   fname[BLOCKSIZE];
    time_t tartime;

    while (1) {
        len = gzread(in, &buffer, BLOCKSIZE);
        if (len < 0)
            fprintf(stderr, "%s\n", gzerror(in, &err));
        /*
        * Always expect complete blocks to process
        * the tar information.
        */
        if (len != BLOCKSIZE)
            fputs("gzread: incomplete block read\n", stderr);

        /*
        * If we have to get a tar header
        */
        if (getheader == 1) {
            /*
            * if we met the end of the tar
            * or the end-of-tar block,
            * we are done
            */
            if ((len == 0)  || (buffer.header.name[0]== 0)) break;

            tartime = (time_t)getoct(buffer.header.mtime,12);
            strcpy(fname, dest);
            if ((fname[strlen(fname)-1] != '/') && (fname[strlen(fname)-1] != '\\'))
                    strcat(fname, "/");
            strcat(fname, buffer.header.name);

            switch (buffer.header.typeflag) {
            case DIRTYPE:
                makedir(fname);
                break;
            case REGTYPE:
            case AREGTYPE:
                remaining = getoct(buffer.header.size,12);
                if (remaining) {
                    outfile = fopen(fname,"wb");
                    if (outfile == NULL) {
                        // try creating directory
                        char *p = strrchr(fname, '/');
                        if (p != NULL) {
                            *p = '\0';
                            makedir(fname);
                            *p = '/';
                            outfile = fopen(fname,"wb");
                        }
                    }
/*
                    fprintf(stderr,
                        "%s %s\n",
                        (outfile) ? "Extracting" : "Couldn't create",
                        fname);
*/
                }
                else
                    outfile = NULL;
                /*
                * could have no contents
                */
                getheader = (remaining) ? 0 : 1;
                break;
            default:
                break;
            }
        }
        else    {
            unsigned int bytes = (remaining > BLOCKSIZE) ? BLOCKSIZE : remaining;

            if (outfile != NULL) {
                if (fwrite(&buffer,sizeof(char),bytes,outfile) != bytes) {
                    fprintf(stderr,"error writing %s skipping...\n",fname);
                    fclose(outfile);
                    unlink(fname);
                }
            }
            remaining -= bytes;
            if (remaining == 0) {
                getheader = 1;
                if (outfile != NULL) {
#ifdef WIN32
                    HANDLE hFile;
                    FILETIME ftm,ftLocal;
                    SYSTEMTIME st;
                    struct tm localt;

                    fclose(outfile);

                    localt = *localtime(&tartime);

                    hFile = CreateFile(fname, GENERIC_READ | GENERIC_WRITE,
                    0, NULL, OPEN_EXISTING, 0, NULL);

                    st.wYear = (WORD)localt.tm_year+1900;
                    st.wMonth = (WORD)localt.tm_mon;
                    st.wDayOfWeek = (WORD)localt.tm_wday;
                    st.wDay = (WORD)localt.tm_mday;
                    st.wHour = (WORD)localt.tm_hour;
                    st.wMinute = (WORD)localt.tm_min;
                    st.wSecond = (WORD)localt.tm_sec;
                    st.wMilliseconds = 0;
                    SystemTimeToFileTime(&st,&ftLocal);
                    LocalFileTimeToFileTime(&ftLocal,&ftm);
                    SetFileTime(hFile,&ftm,NULL,&ftm);
                    CloseHandle(hFile);

                    outfile = NULL;
#else
                    struct utimbuf settime;

                    settime.actime = settime.modtime = tartime;

                    fclose(outfile);
                    outfile = NULL;
                    utime(fname,&settime);
#endif
                }
            }
        }
    }
    return 0;
}

int untargz(int fd, const char *dest) {
    gzFile    f;

    f = gzdopen(fd, "rb");
    if (f == NULL) {
        fputs("Couldn't gzopen file\n", stderr);
        return 1;
    }

    return untar(f, dest);
}
