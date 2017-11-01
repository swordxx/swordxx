/******************************************************************************
 *
 *  SWMgr.java -	
 *
 * $Id: SWMgr.java 2833 2013-06-29 06:40:28Z chrislit $
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

	public SWMgr() {
		reInit();
	}
	public SWMgr(boolean init) {
		if (init) reInit();
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

	/**
	 * add a conf snippet to extraConfig.  This is useful for adding a config section
	 * sent from a module unlock key app.
	 * @param blob
	 * @return an array of section names which were contained in the blob
	 */
	public native String[]    addExtraConfig(String blob);
	public native void        setExtraConfigValue(String section, String key, String value);
	public native String[]    getExtraConfigSections();
	public native String[]    getExtraConfigKeys(String section);
	public native String      getExtraConfigValue(String section, String key);

	public static interface BibleSyncListener {
		public void messageReceived(String osisRef);
	}
	public native void        sendBibleSyncMessage(String osisRef);
	public native void        registerBibleSyncListener(BibleSyncListener listener);
	public String             getStorageBasePath() {
		return ".";
/*
		Context context = app.getApplicationContext();
		return context.getFilesDir().getAbsolutePath();
*/
	}
}


