package org.crosswire.sword.orb;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.StringWriter;
import javax.servlet.http.HttpSession;
import javax.servlet.http.HttpSessionBindingListener;
import javax.servlet.http.HttpSessionBindingEvent;

public class SwordOrb extends Object implements HttpSessionBindingListener {
	public static final String BIBLES = "Biblical Texts";
	public static final String COMMENTARIES = "Commentaries";
	public static final String LEXDICTS = "Lexicons / Dictionaries";
	public static final String GENBOOKS = "Generic Books";
	public static final String DAILYDEVOS = "Daily Devotional";
	static org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(new String[]{}, null);
	String ior = null;
	String localeName = null;

	private SWMgr attach() {
		SWMgr retVal = null;
		try {
System.out.println("attaching...");
			org.omg.CORBA.Object obj = orb.string_to_object(ior);
			retVal = SWMgrHelper.narrow(obj);
System.out.println("calling testConnection");
			retVal.testConnection();
System.out.println("testConnection successful");
		}
		catch(Exception e) {
//			e.printStackTrace();
			retVal = null;
System.out.println("failed in attach");
		}
		return retVal;
	}

	public SwordOrb() {
	}


//	this doesn't seem to work.  Never seems to get called for me
	public void finalize () throws Throwable {
		// shut down external process
		try {
			getSWMgrInstance().terminate();
		}
		catch (Exception e) {}	// we know this doesn't return property cuz we killed the orb! :)

	}


	public void valueBound(HttpSessionBindingEvent httpSessionBindingEvent) {}

	public void valueUnbound(HttpSessionBindingEvent httpSessionBindingEvent) {
		try {
//			throw new Exception("value unbound; showing stacktrace");
			getSWMgrInstance().terminate();
		}
		catch (Exception e) {}	// we know this doesn't return properly cuz we killed the orb! :)
//		catch (Exception e) {e.printStackTrace();}	// we know this doesn't return properly cuz we killed the orb! :)
	}


	private void startOrb() {
		try {
			// start external process
			java.lang.Process p = Runtime.getRuntime().exec("swordorbserver");
			InputStream is = p.getInputStream();
			InputStreamReader isr = new InputStreamReader(is);
			BufferedReader input = new BufferedReader(isr);

			String line;
			line = input.readLine();
//		retVal = p.waitFor();
			ior = line;
System.out.println("Launched ORB, IOR: " + ior);
		}
		catch (Exception e) {e.printStackTrace();}
	}


	public SWMgr getSWMgrInstance() {
		SWMgr retVal = null;
		try {
System.out.println("trying to attach to running ORB");
			retVal = attach();
		}
		catch(org.omg.CORBA.SystemException e) {
//			e.printStackTrace();
			retVal = null;
		}
		if (retVal == null) {
			try {
System.out.println("no ORB running; trying to launch");
				startOrb();
System.out.println("trying to attach to newly launched ORB");
				retVal = attach();
				if (retVal != null) {
					if (localeName != null) {
						retVal.setDefaultLocale(localeName);
					}
				}
			}
			catch(org.omg.CORBA.SystemException e) {
				e.printStackTrace();
			}
		}
		return retVal;
	}

	public static void setSessionLocale(String localeName, HttpSession session) {
		session.setAttribute("SwordOrbLocale", localeName);
		SWMgr mgr = getSWMgrInstance(session);
		if (mgr != null) {
			mgr.setDefaultLocale(localeName);
		}
	}

	public static SwordOrb getSessionOrb(HttpSession session) {
		SwordOrb orb = (SwordOrb)session.getAttribute("SwordOrb");
		if (orb == null) {
System.out.println("No ORB found in session; constructing a new instance");
			orb = new SwordOrb();

			String locName = (String)session.getAttribute("SwordOrbLocale");
			if (locName != null)
				orb.localeName = locName;

			session.setAttribute("SwordOrb", orb);
		}
		else {
System.out.println("ORB found in session");
		}
		return orb;
	}

	public static SWMgr getSWMgrInstance(HttpSession session) {
		SwordOrb orb = getSessionOrb(session);
		SWMgr mgr = orb.getSWMgrInstance();
		return mgr;
	}


	public static void main(String args[]) throws Exception {
		SWMgr mgr = new SwordOrb().getSWMgrInstance();

		System.out.println("PrefixPath: " + mgr.getPrefixPath());
		System.out.println("ConfigPath: " + mgr.getConfigPath());
		ModInfo[] modInfoList = mgr.getModInfoList();
		System.out.println("sequence size: " + modInfoList.length);
		SWModule module;
/*
		for (int i = 0; i < modInfoList.length; i++) {
			System.out.println(modInfoList[i].name + ": " + modInfoList[i].category + ": " + modInfoList[i].language);
			module = mgr.getModuleByName(modInfoList[i].name);
			module.setKeyText("jas1:19");
			System.out.println(module.getRenderText());
		}
*/
		module = mgr.getModuleByName("WHNU");
		module.setKeyText("rev.22.21");
		System.out.println(new String(module.getRawEntry().getBytes("iso8859-1"), "UTF-8"));
/*
		boolean lucene = module.hasSearchFramework();
		SearchHit[] searchResults = module.search("God love world", (lucene)?SearchType.LUCENE:SearchType.MULTIWORD, 0, "");
		for (int i = 0; i < searchResults.length; i++)
			System.out.println(searchResults[i].key);

*/
	}
}
