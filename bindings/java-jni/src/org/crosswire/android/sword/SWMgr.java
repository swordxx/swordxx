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


