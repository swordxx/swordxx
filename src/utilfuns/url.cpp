/******************************************************************************
*  url.cpp  - code for an URL parser utility class
*
* $Id: url.cpp,v 1.6 2004/07/20 09:29:22 joachim Exp $
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

//Sword includes
#include <url.h>
#include <swlog.h>

//system includes
#include <ctype.h>
#include <map>
#include <stdio.h>
#include <iostream.h>

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
		this->url = url;
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
	
	static SWBuf emptyStr("");
	return (it != parameterMap.end()) ? it->second.c_str() : emptyStr.c_str();
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
	 else { //no slash found means there's only the host in the url
	 	return; //WARNING: return already here!
	 }
	 
	 //4. Get the path, e.g. /dir/script.jsp, this is the part up to the first question mark, if it exists. Otherwise up to the end.
	 char* start = end;
	 end = start ? strchr(start, '?') : 0;
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
			
	end = (start && strchr(start, '?')) ? strchr(start, '?')+1 :0;
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
				paramName = decode(paramName.c_str());
				paramValue = decode(paramValue.c_str());
				
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

const char* URL::encode(const char *urlText) {
	static SWBuf url;
	url = urlText;
	
	typedef std::map< unsigned char, SWBuf > DataMap;
    	DataMap m;
	for (unsigned short int c = 32; c <= 255; ++c) { //first set all encoding chars
			if ( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || strchr("-_.!~*'()", c)) {
				continue; //we don't need an encoding for this char
			}

			char s[5];
			SWBuf buf;
			buf.setFormatted("%%-.2X", c);
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

const char* URL::decode(const char *encoded) {
	static SWBuf text;
	text = encoded;	

	SWBuf decoded;	
	const int length = text.length();
	int i = 0;
	
	while (i < length) {
		char a = text[i];
		
		if ( a == '+' ) { //handle special cases
			decoded.append(' ');
		}		
		else if ( (a == '%') && (i+2 < length)) { //decode the %ab  hex encoded char
			const char b = toupper( text[i+1] );
			const char c = toupper( text[i+2] );
			
			if (isxdigit(b) && isxdigit(c)) { //valid %ab part
				unsigned int dec = 16 * ((b >= 'A' && b <= 'F') ? (b - 'A' + 10) : (b - '0')); //dec value of the most left digit (b)
				dec += (c >= 'A' && c <= 'F') ? (c - 'A' + 10) : (c - '0'); //dec value of the right digit (c)
				
				decoded.append((char)dec); //append the decoded char
				
				i += 2; //we jump over the %ab part; we have to leave out three, but the while  loop adds one, too
			}
		}
		else { //just append the char
			decoded.append(a);
		}
		
		i++;
	}
	
	if (decoded.length()) {
		text = decoded;
	}
	return text.c_str();
}

SWORD_NAMESPACE_END
