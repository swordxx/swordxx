/**
 * Title:
 * Description:
 * Copyright:    Copyright (c) 2001 CrossWire Bible Society under the terms of the GNU GPL
 * Company:
 * @author Troy A. Griffitts
 * @version 1.0
 */

#include <swinputmeth.h>

SWInputMethod::SWInputMethod() {
   state = 0;
}

void SWInputMethod::setState(int state) {
   this->state = state;
}

int SWInputMethod::getState() {
   return state;
}

void SWInputMethod::clearState() {
   state = 0;
}
