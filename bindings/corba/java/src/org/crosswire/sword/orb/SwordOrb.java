package org.crosswire.sword.orb;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.StringWriter;

class SwordOrb extends Object {
	static org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(new String[]{}, null);
	static SwordOrb runningOrb = null;
	String ior = null;

	private static SWMgr attach() {
		SWMgr retVal = null;
		try {
			if (runningOrb != null) {
				org.omg.CORBA.Object obj = orb.string_to_object(runningOrb.ior);
				retVal = SWMgrHelper.narrow(obj);
			}
		}
		catch(org.omg.CORBA.SystemException e) {
			e.printStackTrace();
		}
		return retVal;
	}

	private SwordOrb(String ior) {
		this.ior = ior;
	}

	protected void finalize() throws Throwable {
System.out.println("finalizing");
		try { if (runningOrb != null) getSWMgrInstance().terminate(); } catch (Exception e) {}	// we know this doesn't return property cuz we killed the orb! :)
	}

	private static void startOrb() {
		try {
			java.lang.Process p = Runtime.getRuntime().exec("./swordorbserver");

			InputStream is = p.getInputStream();
			InputStreamReader isr = new InputStreamReader(is);
			BufferedReader input = new BufferedReader(isr);

			String line;
			line = input.readLine();
//		retVal = p.waitFor();
			runningOrb = new SwordOrb(line);
		}
		catch (Exception e) {e.printStackTrace();}
	}

	public static SWMgr getSWMgrInstance() {
		SWMgr retVal = null;
		try {
			retVal = attach();
			if (retVal == null) {
				startOrb();
				retVal = attach();
			}
		}
		catch(org.omg.CORBA.SystemException e) {
			e.printStackTrace();
		}
		return retVal;
	}

	public static void main(String args[]) {
		SWMgr mgr = SwordOrb.getSWMgrInstance();

			System.out.println("PrefixPath: " + mgr.getPrefixPath());
			System.out.println("ConfigPath: " + mgr.getConfigPath());
			ModInfo[] modInfoList = mgr.getModInfoList();
			System.out.println("sequence size: " + modInfoList.length);
			for (int i = 0; i < modInfoList.length; i++) {
				System.out.println(modInfoList[i].name + ": " + modInfoList[i].type + ": " + modInfoList[i].lang);
			}

	}

}


