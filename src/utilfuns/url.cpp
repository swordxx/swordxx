/******************************************************************************
*  url.cpp  - code for an URL parser utility class
*
* $Id: url.cpp,v 1.3 2004/07/15 13:51:54 joachim Exp $
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

#include <url.h>
#include <swlog.h>

SWORD_NAMESPACE_START

/**
 * Constructors/Destructors
 */
URL::URL(const char* url)  
	: 	m_url(""),
		m_protocol(""),
		m_hostname(""),
		m_path("")
{
	if (url && strlen(url)) {
		m_url = url;
		parse();
	}
}

const char* const URL::getProtocol() const {
	return m_protocol.c_str();
}

const char* const URL::getHostName () const {
	return m_hostname.c_str();
}

const char* const URL::getPath() const {
	return m_path.c_str();
}

std::map< SWBuf, SWBuf > URL::getParameters()  const {
	return m_parameterMap;
}

/**
 * Returns the value of an URL parameter. For the URL "http://www.crosswire.org/index.jsp?page=test&amp;user=nobody" the value of the parameter "page" would be "test".
 * If the parameter is not set an empty string is returned.
 */
const char* const URL::getParamterValue(const char* const name) {
	return m_parameterMap[ SWBuf(name) ].c_str();
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
	 const char* urlPtr = m_url.c_str();
	 int pos = 0;
	 	 
	 m_protocol = "";
	 m_hostname = "";
	 m_path = "";
	 m_parameterMap.clear();
	 
	 // 2. Get the protocol, which is from the begining to the first ://
	 char* end = strchr( urlPtr, ':' );
	 if (end) { //protocol was found
	 	m_protocol.append(urlPtr, end-urlPtr);
	 	pos = end-urlPtr;
		
		for (char* c = end; (*c == ':') || (*c == '/') ; ++c, ++pos) {} //find the end of the protocol separator (e.g. "://")
	 }
	
	 //3.Get the hostname part. This is the part from pos up to the first slash
	 end = strchr(urlPtr+pos, '/');
	 if (end) {
		m_hostname.append(urlPtr+pos, (end-urlPtr)-pos);
	 	
		pos = end - urlPtr;
	 }
	 
	 //4. Get the path, e.g. /dir/script.jsp, this is the part up to the first question mark, if it exisrs. Otherwise up to the end.
	 char* start = end;
	 end = strchr(start, '?');
	 if (end) {
	 	m_path.append(start, end-start);
		
		pos = end-urlPtr;
	 }
	 else {
	 	m_path.append(start);
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
				m_parameterMap[ paramName ] = paramValue;	
			}
		}
		else {
			break; //no valid parameter in the url
		}
		
		start = end+1;
		end = strstr(start, "&amp;") ? strstr(start, "&amp;")+5 : (strstr(start, "&") ? strstr(start, "&")+1 : 0); //try to find a new paramter part
	}
}

SWORD_NAMESPACE_END
