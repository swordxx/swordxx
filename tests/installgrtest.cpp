#include <swinstallmgr.h>
#include <iostream>

using namespace std;
using namespace sword;

int main(int argc, char **argv) {
	SWInstallMgr inst("ftp://ftp.crosswire.org/pub/sword/raw");
	inst.Refresh();

}

