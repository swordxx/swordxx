#include <sysdata.h>
#include <stdio.h>

int main(int argc, char **argv) {
	printf("0x%.4x 0x%.4x\n", 255, __swap16(255));
	printf("0x%.8x 0x%.8x\n", 255, __swap32(255));
	printf("0x%.16x 0x%.16llx\n", 255, __swap64(255));
	return 0;
}
