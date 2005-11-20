#include <cipherfil.h>
#include <filemgr.h>
#include <swbuf.h>
#include <iostream>

using namespace sword;

int main(int argc, char **argv) {
	
	if (argc != 3) {
		std::cerr << "usage: " << *argv << " <key> <0-encipher|1-decipher>\n";
		return -1;
	}

	
	int encipher = atoi(argv[2]);

	SWFilter *filter = new CipherFilter(argv[1]);

	SWBuf text;
	char buf[4096];
	std::cin >> buf;
	text = buf;

	filter->processText(text, (SWKey *)encipher);

	std::cout << text;
	

	
	return 0;
}
