/******************************************************************************
*  url.h  - code for an URL parser utility class
*
* $Id: url.h,v 1.2 2004/07/08 19:30:42 joachim Exp $
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

#ifndef URL_H
#define URL_H

#include <swbuf.h>
#include <map>

SWORD_NAMESPACE_START

// Class URL
// The URL class provides an interface to work on the data of an URL like "http://www.crosswire.org/index.jsp?page=test&amp;user=nobody"
// 
class URL {
public:
	/** Constructor.
	 * @param url The url string which should be parsed into protocol, hostname, path and paramters
	 */
	URL ( const char* url );
	
	/** Get the protocol.
	* @return The protocol, e.g. "http" for an url like "http://www.crosswire.org/index.jsp?page=help"
	*/
	const char* const  getProtocol() const;
	/** Get the hostname
	* @return The hostname, e.g. "www.crosswire.org" for an url like "http://www.crosswire.org/index.jsp?page=help"
	*/
	const char* const  getHostName() const;
	/** Get the path
	* @return The path, e.g. "/index.jsp" for an url like "http://www.crosswire.org/index.jsp?page=help"
	*/
	const char* const  getPath() const;
	
	/** All available paramters
	* @return The map which contains the parameters and their values
	*/
	const std::map< sword::SWBuf, sword::SWBuf >&  getParameters() const;
	
	/**
	 * Returns the value of an URL parameter. For the URL "http://www.crosswire.org/index.jsp?page=test&amp;user=nobody" the value of the parameter "page" would be "test".
	 * If the parameter is not set an empty string is returned.
	 * @param name The name of the paramter.
	 * @return The value of the given paramter of an empty string if the name could not be found in the list of available paramters
	 */
	const char* const getParamterValue (const char* const name);
		
private:
	/** Parse
	 * Parse the URL into protocol, hostname, path, page and paramters
	 */
	void parse ();
		
	SWBuf m_url;	
	SWBuf m_protocol;
	SWBuf m_hostname;
	SWBuf m_path;
	std::map< SWBuf, SWBuf > m_parameterMap;
};

SWORD_NAMESPACE_END

#endif //URL_H
