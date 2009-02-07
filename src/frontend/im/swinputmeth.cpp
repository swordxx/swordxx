/**
 * Title:
 * Description:
 * Copyright:    Copyright (c) 2001 CrossWire Bible Society under the terms of the GNU GPL
 * Company:
 * @author Troy A. Griffitts
 * @version 1.0
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
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
