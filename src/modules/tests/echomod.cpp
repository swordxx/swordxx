/******************************************************************************
 *  echomod.cpp - code for class 'echomod'- a dummy test text module that just
 *		  echos back the key
 */

#include <echomod.h>


EchoMod::EchoMod() : SWText("echomod", "Echos back key")
{
}


EchoMod::~EchoMod()
{
    this->SWText::~SWText();
}


EchoMod::operator const char*()
{
	return *key;
}
