#include <femain.h>

FEMain::FEMain() {
}

FEMain::~FEMain() {
	list <SWDisplay *>::iterator it;

	for (it = displays.begin(); it != displays.end(); it++)
		delete *it;

}
