package org.crosswire.sword.orb;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.StringWriter;
import javax.servlet.http.HttpSession;
import javax.servlet.http.HttpSessionBindingListener;
import javax.servlet.http.HttpSessionBindingEvent;

class SwordOrb extends Object implements HttpSessionBindingListener {
	static org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(new String[]{}, null);
	String ior = null;

	private SWMgr attach() {
		SWMgr retVal = null;
		try {
			org.omg.CORBA.Object obj = orb.string_to_object(ior);
			retVal = SWMgrHelper.narrow(obj);
		}
		catch(org.omg.CORBA.SystemException e) {
			e.printStackTrace();
		}
		return retVal;
	}

	public SwordOrb() {
	}


//	this doesn't seem to work.  Never seems to get called for me
	public void finalize () throws Throwable {
		// shut down external process
System.err.println("finalizing");
		try {
			getSWMgrInstance().terminate();
		}
		catch (Exception e) {}	// we know this doesn't return property cuz we killed the orb! :)

		// release thread
		waitor.setFinished();
	}

	public void valueBound(HttpSessionBindingEvent httpSessionBindingEvent) {}
	public void valueUnbound(HttpSessionBindingEvent httpSessionBindingEvent) {
		try {
			getSWMgrInstance().terminate();
		}
		catch (Exception e) {}	// we know this doesn't return property cuz we killed the orb! :)
	}

	private void startOrb() {
		try {
			waitor = new Waitor ();
			new Thread (waitor).run ();
			// start external process
			java.lang.Process p = Runtime.getRuntime().exec("./swordorbserver");
			InputStream is = p.getInputStream();
			InputStreamReader isr = new InputStreamReader(is);
			BufferedReader input = new BufferedReader(isr);

			String line;
			line = input.readLine();
//		retVal = p.waitFor();
			ior = line;
		}
		catch (Exception e) {e.printStackTrace();}
	}

	public SWMgr getSWMgrInstance() {
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


	public static SWMgr getSWMgrInstance(HttpSession session) {
		SwordOrb orb = (SwordOrb)session.getAttribute("SwordOrb");
		if (orb == null) {
			orb = new SwordOrb();
			session.setAttribute("SwordOrb", orb);
		}
		return orb.getSWMgrInstance();
	}


	public static void main(String args[]) throws Exception {
		new SwordOrb ();
		Thread.sleep(100);
		/*
		SWMgr mgr = new SwordOrb().getSWMgrInstance();

		System.out.println("PrefixPath: " + mgr.getPrefixPath());
		System.out.println("ConfigPath: " + mgr.getConfigPath());
		ModInfo[] modInfoList = mgr.getModInfoList();
		System.out.println("sequence size: " + modInfoList.length);
		for (int i = 0; i < modInfoList.length; i++) {
			System.out.println(modInfoList[i].name + ": " + modInfoList[i].type + ": " + modInfoList[i].lang);
		}
		*/
	}

public class Waitor implements Runnable {
  protected boolean finished = false;
  public Waitor () {
  }
  public void setFinished () {
    finished = true;
  }
  public void run () {
    while (!finished) {
	    try { Thread.sleep (100); } catch (Exception e) { e.printStackTrace(); }
	 // possibly even:
	 // System.gc();
    }
  }
}
  protected Waitor waitor;

}