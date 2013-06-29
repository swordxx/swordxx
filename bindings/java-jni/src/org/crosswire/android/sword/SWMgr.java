/******************************************************************************
 *
 *  SWMgr.java -	
 *
 * $Id$
 *
 * Copyright 2009-2013 CrossWire Bible Society (http://www.crosswire.org)
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

package org.crosswire.android.sword;

public class SWMgr {
	
	static {
		System.loadLibrary("sword");
	}

	public static class ModInfo {
		public String name;
		public String description;
		public String category;
		public String language;
		public String version;
		public String delta;
	}


	public native String version();
	public native void reInit();

	public native ModInfo[]   getModInfoList();
	public native SWModule    getModuleByName(String name);
	public native String      getPrefixPath();
	public native String      getConfigPath();
	public native void        setGlobalOption(String option, String value);
	public native String      getGlobalOption(String option);
	public native String      getGlobalOptionTip(String option);
	public native String      filterText(String filterName, String text);
	public native String[]    getGlobalOptions();
	public native String[]    getGlobalOptionValues(String option);
	public native void        setCipherKey(String modName, String key);
	public native void        setJavascript(boolean val);
	public native String[]    getAvailableLocales();
	public native void        setDefaultLocale(String name);
}


