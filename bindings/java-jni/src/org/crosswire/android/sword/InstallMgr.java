package org.crosswire.android.sword;

public class InstallMgr {

	public native void reInit();

	public native void             setUserDisclaimerConfirmed();
	public native int              syncConfig();
	public native int              uninstallModule(String modName);
	public native String []        getRemoteSources();
	public native int              refreshRemoteSource(String sourceName);
	public native SWMgr.ModInfo [] getRemoteModInfoList(String sourceName);
	public native int              remoteInstallModule(String sourceName, String modName);
	public native SWModule         getRemoteModuleByName(String source, String name);

}

