#include <swinstallmgr.h>
#include <iostream>

using namespace std;
using namespace sword;

int main(int argc, char **argv) {
	InstallMgr inst("ftp://ftp.crosswire.org/pub/sword/raw");
	inst.Refresh();

}

