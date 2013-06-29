/******************************************************************************
 *
 *  SWModule.java -	
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

public class SWModule {

	private String name;
	private String description;
	private String category;
	
	// if this is a shell module from a remote source...
	private String remoteSourceName;


	public static final int SEARCHTYPE_REGEX     =  1;
	public static final int SEARCHTYPE_PHRASE    = -1;
	public static final int SEARCHTYPE_MULTIWORD = -2;
	public static final int SEARCHTYPE_ENTRYATTR = -3;
	public static final int SEARCHTYPE_LUCENE    = -4;

	private SWModule() {}	// don't allow allocation, instead use factory method SWMgr.getModuleByName to retrieve an instance

	public static class SearchHit {
		public String modName;
		public String key;
		public long   score;
	}
	
	public static interface SearchProgressReporter {
		public void progressReport(int percent);
	}

	public native void          terminateSearch();
	public native SearchHit[]   search(String expression, int searchType, long flags, String scope, SearchProgressReporter progressReporter);
	public SearchHit[]          search(String expression) { return search(expression, SEARCHTYPE_MULTIWORD, 0, "", null); }
	public native char          error();
	public native long          getEntrySize();
	public native String[]      getEntryAttribute(String level1, String level2, String level3, boolean filtered);
	public native String[]      parseKeyList(String keyText);

	// Special values handled for VerseKey modules: [+-][book|chapter]
	//	(e.g.	"+chapter" will increment the VerseKey 1 chapter)
	public native void          setKeyText(String key);

	public native String        getKeyText();
	public native boolean       hasKeyChildren();

	// This method returns child nodes for a genbook,
	// but has special handling if called on a VerseKey module:
	//  [0..6] [testament, book, chapter, verse, chapterMax, verseMax, bookName]
	public native String[]      getKeyChildren();

	public native String        getKeyParent();
	public String               getName()		{ return name; }
	public String               getDescription()	{ return description; }
	public String               getCategory()	{ return category; }
	public native void          previous();
	public native void          next();
	public native void          begin();
	public native String        getStripText();
	public native String        getRenderText();
	public native String        getRawEntry();
	public native void          setRawEntry(String entryBuffer);
	public native String        getConfigEntry(String key);
	public native void          deleteSearchFramework();
	public native boolean       hasSearchFramework();
}
