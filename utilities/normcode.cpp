/*  normcode.cpp
 *  Transcodes Latin-1/UTF-8 to canonically normalized UTF-8/SCSU
 */

#include <iostream>
#include <string>
#include <fstream>


#include "unicode/utypes.h"
#include "unicode/convert.h"
#include "unicode/ustring.h"
#include "unicode/normlzr.h"

#define BUFSIZE 65536

int main (char argc, char** argv) {

  if (argc < 5) {
    cout << "usage: " << argv[0] << " -[8|l] <input file> -[8|s] <outputfile> -[c|d]" << endl;
    cout << "The first argument should be -8 if input is UTF-8 or -l if it is Latin-1/Codepage 1252." << endl;
    cout << "The third argument should be -8 for UTF-8 output or -s for SCSU output." << endl;
    cout << "The fifth arg should be -c if you want to pre-compose or -d if you want to decompose (default)." << endl;
    return -1;
  }

  fstream inf, outf;
  char* buffer = new char[BUFSIZE];
  UnicodeString source, norm;
  UErrorCode err;
  int32_t buf32;

  UnicodeConverter latin("Latin-1", err);
  if (U_FAILURE(err)) cout << "Latin-1 converter error" << endl;
  UnicodeConverter scsu("SCSU", err);
  if (U_FAILURE(err)) cout << "SCSU converter error" << endl;
  UnicodeConverter utf8("UTF-8", err);
  if (U_FAILURE(err)) cout << "UTF-8 converter error" << endl;

  inf.open(argv[2], fstream::in);
  outf.open(argv[4], fstream::out);

  while (!inf.eof()) {
    inf.getline(buffer, BUFSIZE);
    buf32 = inf.gcount();
    if (buf32 == 0) {
      outf << endl;
      continue;
    }
    buffer[buf32-1] = '\n';
    buffer[buf32] = 0;

    if (argv[1][1] == 'l') {
      latin.toUnicodeString(source, buffer, buf32, err);
      if (U_FAILURE(err)) cout << "Latin-1 conversion error" << endl;
    } else {
      utf8.toUnicodeString(source, buffer, buf32, err);
      if (U_FAILURE(err)) cout << "UTF-8 conversion error" << endl;
    }

    if (argc > 5) {
      if (argv[5][1] == 'c') {
	Normalizer::normalize(source, UNORM_NFC, 0, norm, err); //canonical composition
      } else {
	Normalizer::normalize(source, UNORM_NFD, 0, norm, err); //canonical decomposition
      }
    } else {
      Normalizer::normalize(source, UNORM_NFD, 0, norm, err); //canonical decomposition
    }
    
    buf32 = BUFSIZE;
    if (argv[3][1] == 's') {
      scsu.fromUnicodeString((char*)buffer, buf32, norm, err);
      if (U_FAILURE(err)) cout << "SCSU conversion error" << endl;
      //      scsu.toUnicodeString(norm, buffer, buf32, err);
      //      buf32 = BUFSIZE;
      //      utf8.fromUnicodeString((char*)buffer, buf32, norm, err);
    } else {
      utf8.fromUnicodeString((char*)buffer, buf32, norm, err);
      if (U_FAILURE(err)) cout << "UTF-8 conversion error" << endl;
    }

    if (U_SUCCESS(err) && !inf.eof()) {
      buffer[buf32] = 0;
    }
    
    outf.write(buffer, buf32);
  }

  outf.close();
  inf.close();

  delete buffer;

  return 0;
}
