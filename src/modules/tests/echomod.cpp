/******************************************************************************
 *  echomod.cpp - code for class 'echomod'- a dummy test text module that just
 *		  echos back the key
 */

#include <echomod.h>


SWORD_NAMESPACE_START

EchoMod::EchoMod() : SWText("echomod", "Echos back key")
{
}


EchoMod::~EchoMod() {
}


SWBuf &EchoMod::getRawEntryBuf()
{
	static SWBuf retVal;
	retVal = *key;
	return retVal;
}

SWORD_NAMESPACE_END
