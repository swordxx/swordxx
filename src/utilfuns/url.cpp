/******************************************************************************
*  url.cpp  - code for an URL parser utility class
*
* $Id: url.cpp,v 1.4 2004/07/17 12:20:50 scribe Exp $
*
* Copyright 2003 CrossWire Bible Society (http://www.crosswire.org)
*	CrossWire Bible Society
*	P. O. Box 2528
*	Tempe, AZ  85280-2528
*
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

#include <stdio.h>
#include <url.h>
#include <swlog.h>

SWORD_NAMESPACE_START

/**
 * Constructors/Destructors
 */
URL::URL(const char* url)  
	: 	url(""),
		protocol(""),
		hostname(""),
		path("")
{
	if (url && strlen(url)) {
		url = url;
		parse();
	}
}

const char* URL::getProtocol() const {
	return protocol.c_str();
}

const char* URL::getHostName () const {
	return hostname.c_str();
}

const char* URL::getPath() const {
	return path.c_str();
}

const URL::ParameterMap &URL::getParameters() const {
	return parameterMap;
}

/**
 * Returns the value of an URL parameter. For the URL "http://www.crosswire.org/index.jsp?page=test&amp;user=nobody" the value of the parameter "page" would be "test".
 * If the parameter is not set an empty string is returned.
 */
const char* URL::getParamterValue(const char* name) const {
	ParameterMap::const_iterator it = parameterMap.find(name);
	return (it != parameterMap.end()) ? it->second.c_str() : 0;
}


/** Parse the URL.
 * Parse the URL into the protocol, the hostname, the path and the paramters with their values
 * 
 */
void URL::parse () {
	/* format example		protocol://hostname/path/path/path.pl?param1=value1&amp;param2=value2
	 * we include the script name in the path, so the path would be /path/path/path.pl in this example
	 *  &amp; could also be &
	 */
	 
	 //1. Init
	 const char* urlPtr = url.c_str();
	 int pos = 0;
	 	 
	 protocol = "";
	 hostname = "";
	 path = "";
	 parameterMap.clear();
	 
	 // 2. Get the protocol, which is from the begining to the first ://
	 char* end = strchr( urlPtr, ':' );
	 if (end) { //protocol was found
	 	protocol.append(urlPtr, end-urlPtr);
	 	pos = end-urlPtr;
		
		for (char* c = end; (*c == ':') || (*c == '/') ; ++c, ++pos) {} //find the end of the protocol separator (e.g. "://")
	 }
	
	 //3.Get the hostname part. This is the part from pos up to the first slash
	 end = strchr(urlPtr+pos, '/');
	 if (end) {
		hostname.append(urlPtr+pos, (end-urlPtr)-pos);
	 	
		pos = end - urlPtr;
	 }
	 
	 //4. Get the path, e.g. /dir/script.jsp, this is the part up to the first question mark, if it exisrs. Otherwise up to the end.
	 char* start = end;
	 end = strchr(start, '?');
	 if (end) {
	 	path.append(start, end-start);
		
		pos = end-urlPtr;
	 }
	 else {
	 	path.append(start);
		return; //WARNING: return already here
	 }

	//5. Fill the map with the parameters and their values
	SWBuf paramName;
	SWBuf paramValue;
			
	end = strchr(start, '?') ? strchr(start, '?')+1 :0;
	while (end) {
		paramName = "";
		paramValue = "";
		
		//search for the equal sign to find the value part
		const char* valueStart = strchr(end, '=');
		
		if (valueStart) {
			const char* valueEnd = strstr(valueStart, "&amp;") ? strstr(valueStart, "&amp;") : strstr(valueStart, "&"); //try to find a new paramter part
			
			if (valueEnd) {
				paramName.append(end, valueStart-end);
				paramValue.append(valueStart+1, valueEnd-(valueStart+1));
			}
			else { //this is the last paramter of the URL
				paramName.append(end, valueStart-end);
				paramValue.append(valueStart+1);
			}
			
			if (paramName.length() && paramValue.length()) {//insert the param into the map if it's valid
				parameterMap[ paramName ] = paramValue;	
			}
		}
		else {
			break; //no valid parameter in the url
		}
		
		start = end+1;
		end = strstr(start, "&amp;") ? strstr(start, "&amp;")+5 : (strstr(start, "&") ? strstr(start, "&")+1 : 0); //try to find a new paramter part
	}
}

const char *encode(const char *urlText) {
	static SWBuf url;
	url= urlText;
	typedef map<unsigned char,SWBuf> DataMap;
    	DataMap m;
	for (unsigned short int c = 32; c <= 255; ++c) { //first set all encoding chars
			if ( (c>='A' && c<='Z') || (c>='a' && c<='z') || (c>='0' && c<='9') || strchr("-_.!~*'()", c)) {
				continue; //we don't need an encoding for this char
			}

			char s[5];
			sprintf(s, "%-.2X", c); //left-aligned, 2 digits, uppercase hex
			m[c] = SWBuf("%") + s; //encoded char is "% + 2 digit hex code of char"
	}
	//the special encodings for certain chars
	m[' '] = '+';

	SWBuf buf;
	const int length = url.length();
	for (int i = 0; i <= length; i++) { //fill "buf"
		const char& c = url[i];
		buf += (!m[c].length()) ? (SWBuf)c : (SWBuf)m[c];
	}

	url = buf;
	return url.c_str();
}
SWORD_NAMESPACE_END
