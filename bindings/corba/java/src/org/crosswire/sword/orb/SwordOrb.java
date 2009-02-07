/*
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
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

package org.crosswire.sword.orb;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.StringWriter;
import javax.servlet.http.HttpSession;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSessionBindingListener;
import javax.servlet.http.HttpSessionBindingEvent;
import java.util.Vector;
import java.util.Hashtable;
import java.util.Date;
import java.util.Properties;

public class SwordOrb extends Object implements HttpSessionBindingListener {
	public static Properties config = null;
	public static String ORBEXE = "swordorbserver";
	public static final int MAX_REMOTE_ADDR_CONNECTIONS = 20;
	public static final int MAX_ACCESS_COUNT_PER_INTERVAL = 10;
	public static final long MAX_ACCESS_COUNT_INTERVAL = 10 * 1000;	// milliseconds
	public static final long BLACKLIST_DURATION = 10 * 60 * 1000;	// milliseconds
	public static final String BIBLES = "Biblical Texts";
	public static final String COMMENTARIES = "Commentaries";
	public static final String LEXDICTS = "Lexicons / Dictionaries";
	public static final String GENBOOKS = "Generic Books";
	public static final String DAILYDEVOS = "Daily Devotional";

	static java.util.Properties p = new java.util.Properties();
	static {
		p.setProperty("com.sun.CORBA.codeset.charsets", "0x05010001, 0x00010109");    // UTF-8, UTF-16
		p.setProperty("com.sun.CORBA.codeset.wcharsets", "0x00010109, 0x05010001");    // UTF-16, UTF-8
	}

	static org.omg.CORBA.ORB orb = org.omg.CORBA_2_3.ORB.init(new String[]{}, p);
	static Hashtable clients = new Hashtable();
	String ior = null;
	String remoteAddr = null;
	String localeName = null;
	long   lastAccessed = 0;
	int    intervalCount = 0;
	long   intervalStamp = 0;
	long   blacklistTill = 0;

	private SWMgr attach() {

		// assert IOR has been set
		if (ior == null)
			return null;

		SWMgr retVal = null;
		try {
System.out.println("attaching...");
			org.omg.CORBA.Object obj = orb.string_to_object(ior);
			retVal = SWMgrHelper.narrow(obj);
System.out.println("calling testConnection");
			retVal.testConnection();
System.out.println("testConnection successful");
		}
		catch(Throwable e) {
//			e.printStackTrace();
			retVal = null;
System.out.println("failed in attach");
		}
		return retVal;
	}

	public SwordOrb(String remoteAddr) {
		this.remoteAddr = remoteAddr;
	}


//	this doesn't seem to work.  Never seems to get called for me
	public void finalize () throws Throwable {
		// shut down external process
		try {
System.out.println("calling finalize.");
			getSWMgrInstance().terminate();
		}
		catch (Exception e) {}	// we know this doesn't return property cuz we killed the orb! :)

	}


	public void valueBound(HttpSessionBindingEvent httpSessionBindingEvent) {}

	public void valueUnbound(HttpSessionBindingEvent httpSessionBindingEvent) {
		try {
//			throw new Exception("value unbound; showing stacktrace");
			Vector orbs = (Vector)clients.get(remoteAddr);
int size = -1;
			if (orbs != null) {
size = orbs.size();
				orbs.remove(this);
			}
System.out.println("calling valueUnbound. size before: " + size + "; size after: "+orbs.size());
			getSWMgrInstance().terminate();
		}
		catch (Exception e) {}	// we know this doesn't return properly cuz we killed the orb! :)
//		catch (Exception e) {e.printStackTrace();}	// we know this doesn't return properly cuz we killed the orb! :)
	}

	private static void loadConfig(HttpServletRequest request) {
		try {
			config = new Properties();
			File propName = new File(request.getSession().getServletContext().getRealPath("/WEB-INF/swordweb.properties"));
			if (propName.exists()) {
				FileInputStream propFile = new FileInputStream(propName);
				config.load(propFile);
				propFile.close();
			}
		}
		catch (Exception e) { e.printStackTrace(); }
		ORBEXE = config.getProperty("orbexe", "swordorbserver");
	}

	private void startOrb() {
		try {
			// start external process
			java.lang.Process p = Runtime.getRuntime().exec(ORBEXE);
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

	void checkAccessAbuse() throws Exception {
		if ((blacklistTill > 0) && (System.currentTimeMillis() < blacklistTill)) {
			throw new Exception("You're an abuser and have been blacklisted till " + new Date(blacklistTill));
		}
		if (++intervalCount > MAX_ACCESS_COUNT_PER_INTERVAL) {
			if (System.currentTimeMillis() < intervalStamp + MAX_ACCESS_COUNT_INTERVAL) {
				// abuser
				blacklistTill = System.currentTimeMillis() + BLACKLIST_DURATION;
			}
			intervalStamp = System.currentTimeMillis();
			intervalCount = 0;
		}
	}

	public SWMgr getSWMgrInstance() throws Exception {
		lastAccessed = System.currentTimeMillis();
		checkAccessAbuse();
		SWMgr retVal = null;
		try {
System.out.println("trying to attach to running ORB");
			retVal = attach();
		}
		catch(Exception e) {
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

	public static void setSessionLocale(String localeName, HttpServletRequest request) throws Exception {
		request.getSession().setAttribute("SwordOrbLocale", localeName);
		SWMgr mgr = getSWMgrInstance(request);
		if (mgr != null) {
			mgr.setDefaultLocale(localeName);
		}
	}

	public static SwordOrb getSessionOrb(HttpServletRequest request) throws Exception {
		if (config == null) loadConfig(request);
		HttpSession session = request.getSession();
		SwordOrb orb = (SwordOrb)session.getAttribute("SwordOrb");
		String remoteAddr = request.getRemoteAddr();
		if (orb == null) {
System.out.println("No ORB found in session; constructing a new instance");

			Vector orbs = (Vector)clients.get(remoteAddr);
			if (orbs == null) {
				orbs = new Vector();
				clients.put(remoteAddr, orbs);
			}
			if (orbs.size() < MAX_REMOTE_ADDR_CONNECTIONS) {
				orb = new SwordOrb(remoteAddr);
				orbs.add(orb);

				String locName = (String)session.getAttribute("SwordOrbLocale");
				if (locName != null)
					orb.localeName = locName;

				session.setAttribute("SwordOrb", orb);
			}
			else throw new Exception("Max Remote Addr Connections from: ["+remoteAddr+"]");
		}
		else {
System.out.println("ORB found in session");
		}
		return orb;
	}

	public static SWMgr getSWMgrInstance(HttpServletRequest request) throws Exception {
		SwordOrb orb = getSessionOrb(request);
		SWMgr mgr = orb.getSWMgrInstance();
		return mgr;
	}


	public static void main(String args[]) throws Exception {
		SWMgr mgr = new SwordOrb("127.0.0.1").getSWMgrInstance();

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
