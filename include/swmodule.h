/******************************************************************************
 *  swmodule.h  - code for base class 'module'.  Module is the basis for all
 *		  types of modules (e.g. texts, commentaries, maps, lexicons,
 *		  etc.)
 *
 * $Id: swmodule.h,v 1.28 2001/10/24 19:43:57 chrislit Exp $
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWMODULE_H
#define SWMODULE_H

#include <swdisp.h>
#include <swkey.h>
#include <listkey.h>
#include <swfilter.h>
#include <list>

#include <defs.h>

#define FILTERPAD 72

using namespace std;

typedef list < SWFilter * >FilterList;

enum DIRECTION {DIRECTION_LTR, DIRECTION_RTL, DIRECTION_BIDI};
enum FORMATS {FMT_UNKNOWN, FMT_PLAIN, FMT_THML, FMT_GBF, FMT_HTML, FMT_HTMLHREF, FMT_RTF, FMT_OSIS};
enum ENCODINGS {ENC_UNKNOWN, ENC_LATIN1, ENC_UTF8, ENC_SCSU, ENC_UTF16, ENC_RTF, ENC_HTML};

/**
  * The class SWModule is the base class for all modules used in Sword.
  *
  */
  
class SWDLLEXPORT SWModule {

protected:

  char error;

  /** the current key */
  SWKey *key;

  ListKey listkey;
  char *modname;
  char *moddesc;
  char *modtype;

  char direction;
  char markup;
  char encoding;

  /** this module's display object */
  SWDisplay *disp;

  static SWDisplay rawdisp;
  char *entrybuf;

  /** executed to remove all markup (for searches) */
  FilterList *stripFilters;

  /** executed immediately upon fileread */
  FilterList *rawFilters;

  /** executed to format for display */
  FilterList *renderFilters;

  /** executed to change markup to user prefs */
  FilterList *optionFilters;

  /** executed to decode text for display */
  FilterList *encodingFilters;

  bool render;
  int entrySize;

public:

  /**
  * This is the default callback function for searching.
  * This function is a placeholder and does nothing.
  * You can define your own function for search progress
  * evaluation, and pass it over to @ref Search.
  */
  static void nullPercent (char percent, void *userData);

  /**
  * Set this bool to false to terminate the search which is executed by this module (@ref #Search).
  * This is useful for threaded applications to terminate the search in another thread.
  */
  bool terminateSearch;

  /** Initializes data for instance of SWModule
  *
  * @param imodname Internal name for module; see also @ref Name
  * @param imoddesc Name to display to user for module;
  *  see also @ref Description
  * @param idisp Display object to use for displaying
  *  see also @ref Disp
  * @param imodtype Type of Module
  *  (All modules will be displayed with
  *  others of same type under their modtype heading)
  *  see also @ref Type
  */
  SWModule (const char *imodname = 0, const char *imoddesc = 0, SWDisplay * idisp = 0, char *imodtype = 0, char encoding = ENC_UNKNOWN, char dir = DIRECTION_LTR, char markup = FMT_UNKNOWN);

  /** Cleans up instance of SWModule
  */
  virtual ~ SWModule ();

  /** Gets and clears error status
  *
  * @return error status
  */
  virtual char Error ();

  /**
  * @return  True if this module is encoded in Unicode, otherwise return false.
  */
  virtual const bool isUnicode() const {return (encoding == ENC_UTF8 || encoding == ENC_SCSU);}
  /**
  * @return The size of the current entry.
  */
  virtual const int getEntrySize() const {return entrySize;}

  /** 
  * Sets a key to this module for position to a 
  * particular record or set of records
  *
  * @param ikey key with which to set this module
  * @return error status
  */
  virtual char SetKey (const SWKey *ikey);

  /**
  * Sets the key of this module. Similair to @see SetKey(const SWKey*) .
  * @param ikey The SWKey which should be used as new key.
  * @return Error status
  */
  virtual char SetKey (const SWKey &ikey);

  /** Gets the current module key
  * @return the current key of this module
  */
  virtual SWKey & Key () const {
    return *key;
  }
  
  /** Sets the current key of the module to ikey, and returns
  * the keytext
  *
  * @param ikey new current key for the module
  * @return the keytext of the current module key
  */
  virtual char Key (const SWKey & ikey) {
    return SetKey (ikey);
  }
  
  /** Sets/gets module KeyText
  *
  * @param ikeytext value which to set keytext;
  *  [0] - only get
  * @return pointer to keytext
  */
  virtual const char *KeyText (const char *imodtype = 0);
  
  /** Calls this modules display object and passes itself
  *
  * @return error status
  */
  virtual char Display ();
  
  /** Sets/gets display driver
  *
  * @param idisp value which to set disp;
  *  [0] - only get
  * @return pointer to disp
  */
  virtual SWDisplay *Disp (SWDisplay * idisp = 0);
  
  /** Sets/gets module name
  *
  * @param imodname value which to set modname;
  *  [0] - only get
  * @return pointer to modname
  */
  virtual char *Name (const char *imodname = 0);
  
  /** Sets/gets module description
  *
  * @param imoddesc value which to set moddesc;
  *  [0] - only get
  * @return pointer to moddesc
  */
  virtual char *Description (const char *imoddesc = 0);
  
  /** Sets/gets module type
  *
  * @param imodtype value which to set modtype;
  *  [0] - only get
  * @return pointer to modtype
  */
  virtual char *Type (const char *imodtype = 0);

  /** Sets/gets module direction
  *
  * @param newdir value which to set direction;
  *  [-1] - only get
  * @return char direction
  */
  virtual char Direction(char newdir = -1);

  /** Sets/gets module encoding
  *
  * @param newdir value which to set encoding;
  *  [-1] - only get
  * @return char encoding
  */
  virtual char Encoding(char enc = -1);

  /** Sets/gets module markup
  *
  * @param newdir value which to set markup;
  *  [-1] - only get
  * @return char markup
  */
  virtual char Markup(char enc = -1);

  // search methods

  /** Searches a module for a string
  *
  * @param istr string for which to search
  * @param searchType type of search to perform
  *   >=0 ->regex;  1->phrase; 2->multiword;
  * @param flags options flags for search
  * @param justCheckIfSupported if set, don't search,
  * only tell if this function supports requested search.
  *
  * @return listkey set to verses that contain istr
  */
  virtual ListKey & Search (const char *istr, int searchType = 0, int flags = 0,
                  SWKey * scope = 0,
                  bool * justCheckIfSupported = 0,
                  void (*percent) (char, void *) = &nullPercent,
                  void *percentUserData = 0);
  /**
  *
  */
  virtual char createSearchFramework () {
    return 0;
  }				// special search framework
  
  /**
  *
  */
  virtual bool hasSearchFramework () {
    return false;
  }				// special search framework
  
  /**
  *
  */
  virtual bool isSearchOptimallySupported (const char *istr, int searchType,
					   int flags, SWKey * scope) {
      bool retVal = false;
      Search (istr, searchType, flags, scope, &retVal);
      return retVal;
  }

  /** Allocates a key of specific type for module
  *
  * @return pointer to allocated key
  */
  virtual SWKey *CreateKey ();

  /** Renders and returns the current module text
  * @return the rendered current module text
  */
  virtual operator char *();
  
  /** This function is reimplemented by the different kinds
  * of module objects
  * @return the raw module text of the current entry
  */
  virtual char *getRawEntry () = 0;
  
  /**
  *
  */
  virtual operator SWKey & () {
    return *key;
  }
  
  /**
  *
  */
  virtual operator SWKey *() {
    return key;
  }

  // write interface ----------------------------
  /** Is the module writable? :)
  * @return yes or no
  */
  virtual bool isWritable () {
    return false;
  }
  
  /** Creates a new module
  * @param path The first parameter is path of the new module
  * @return error
  */
  static char createModule (const char *) {
    return -1;
  }
  
  /** Modify the current module entry text
  * - only if module @ref isWritable
  * @return *this
  */
  virtual SWModule & operator << (const char *) {
    return *this;
  }
  
  /** Link the current module entry to another module entry
  * - only if module @ref isWritable
  * @return *this
  */
  virtual SWModule & operator << (const SWKey *) {
    return *this;
  }
  
  /** Delete current module entry - only if module @ref isWritable
  *
  */
  virtual void deleteEntry () {
  }
  
  // end write interface ------------------------

  /** Decrements module key a number of entries
  *
  * @param decrement Number of entries to jump backward
  * @return *this
  */
  virtual SWModule & operator -= (int decrement);
  
  /** Increments module key a number of entries
  *
  * @param increment Number of entries to jump forward
  * @return *this
  */
  virtual SWModule & operator += (int increment);
  
  /** Increments module key by 1 entry
  *
  */
  virtual SWModule & operator++ (int) {
    return *this += 1;
  }
  
  /** Decrements module key by 1 entry
  *
  */
  virtual SWModule & operator-- (int) {
    return *this -= 1;
  }
  
  /** Positions this modules to an entry
  *
  * @param p position (e.g. TOP, BOTTOM)
  * @return *this
  */
  virtual SWModule & operator = (SW_POSITION p);

  /** Adds a RenderFilter to this module's @see renderfilters queue
  * @param newfilter the filter to add
  * @return *this
  */
  virtual SWModule & AddRenderFilter (SWFilter * newfilter) {
    renderFilters->push_back (newfilter);
    return *this;
  }

  /** Removes a RenderFilter from this module's @see renderfilters queue
  * @param oldfilter the filter to remove
  * @return *this
  */
  virtual SWModule & RemoveRenderFilter (SWFilter * oldfilter) {
    renderFilters->remove (oldfilter);
    return *this;
  }

  /** Replaces a RenderFilter in this module's @see renderfilters queue
  * @param oldfilter the filter to remove
  * @param newfilter the filter to add in its place
  * @return *this
  */
  virtual SWModule & ReplaceRenderFilter (SWFilter * oldfilter, SWFilter * newfilter) {
    FilterList::iterator iter;
    for (iter = renderFilters->begin(); iter != renderFilters->end(); iter++)
        if (*iter == oldfilter)
                *iter = newfilter;
    return *this;
  }

  /** RenderFilter a text buffer
  * @param buf the buffer to filter
  * @param size the allocated size of the buffer
  * @param key key location from where this buffer was extracted
  * @return *this
  */
  virtual void renderFilter (char *buf, long size, SWKey *key) {
  	filterBuffer(renderFilters, buf, size, key);
  }

  /** Adds an EncodingFilter to this module's @see encodingfilters queue
  * @param newfilter the filter to add
  * @return *this
  */
  virtual SWModule & AddEncodingFilter (SWFilter * newfilter) {
    encodingFilters->push_back (newfilter);
    return *this;
  }

  /** Removes an EncodingFilter from this module's @see encodingfilters queue
  * @param oldfilter the filter to remove
  * @return *this
  */
  virtual SWModule & RemoveEncodingFilter (SWFilter * oldfilter) {
    encodingFilters->remove (oldfilter);
    return *this;
  }

  /** Replaces an EncodingFilter in this module's @see encodingfilters queue
  * @param oldfilter the filter to remove
  * @param newfilter the filter to add in its place
  * @return *this
  */
  virtual SWModule & ReplaceEncodingFilter (SWFilter * oldfilter, SWFilter * newfilter) {
    FilterList::iterator iter;
    for (iter = encodingFilters->begin(); iter != encodingFilters->end(); iter++)
        if (*iter == oldfilter)
                *iter = newfilter;
    return *this;
  }

  /** encodingFilter a text buffer
  * @param buf the buffer to filter
  * @param size the allocated size of the buffer
  * @param key key location from where this buffer was extracted
  * @return *this
  */
  virtual void encodingFilter (char *buf, long size, SWKey *key) {
  	filterBuffer(encodingFilters, buf, size, key);
  }


  /** Adds a StripFilter to this module's @ref stripfilters queue
  * @param newfilter the filter to add
  * @return *this
  */
  virtual SWModule & AddStripFilter (SWFilter * newfilter) {
    stripFilters->push_back (newfilter);
    return *this;
  }

  /** StripFilter a text buffer
  * @param buf the buffer to filter
  * @param size the allocated size of the buffer
  * @param key key location from where this buffer was extracted
  * @return *this
  */
  virtual void stripFilter (char *buf, long size, SWKey *key) {
  	filterBuffer(stripFilters, buf, size, key);
  }
  
  /** Adds a RawFilter to this module's @ref rawfilters queue
  * @param newfilter the filter to add
  * @return *this
  */
  virtual SWModule & AddRawFilter (SWFilter * newfilter) {
    rawFilters->push_back (newfilter);
    return *this;
  }
  
  /** FilterBuffer a text buffer
  * @param filters the FilterList of filters to iterate
  * @param buf the buffer to filter
  * @param size the allocated size of the buffer
  * @param key key location from where this buffer was extracted
  * @return *this
  */
  virtual void filterBuffer (FilterList *filters, char *buf, long size, SWKey *key) {
	FilterList::iterator it;
	for (it = filters->begin(); it != filters->end(); it++) {
		(*it)->ProcessText(buf, size, key);
	}
  }

  /** RawFilter a text buffer
  * @param buf the buffer to filter
  * @param size the allocated size of the buffer
  * @param key key location from where this buffer was extracted
  * @return *this
  */
  virtual void rawFilter (char *buf, long size, SWKey *key) {
        buf[size] = 0;
  	filterBuffer(rawFilters, buf, size, key);
  }
  
  /** Adds an OptionFilter to this module's @ref optionfilters queue
  * @param newfilter the filter to add
  * @return *this
  */
  virtual SWModule & AddOptionFilter (SWFilter * newfilter) {
    optionFilters->push_back (newfilter);
    return *this;
  }

  /** OptionFilter a text buffer
  * @param buf the buffer to filter
  * @param size the allocated size of the buffer
  * @param key key location from where this buffer was extracted
  * @return *this
  */
  virtual void optionFilter (char *buf, long size, SWKey *key) {
  	filterBuffer(optionFilters, buf, size, key);
  }
  
  /** calls all StripFilters on buffer or current text
  *
  * @param buf buf to massage instead of current module position;
  *   if buf is NULL, the current text will be used
  * @param len max len of buf OR current text -- will be applied anyway
  * @return this module's text at specified key location massaged by Strip filters
  */
  virtual const char *StripText (char *buf = 0, int len = -1);
  
  /** calls all RenderFilters on buffer or current text
  *
  * @param buf buffer to Render instead of current module position;
  *   if buf is NULL, the current text will be used
  * @param len max len of buf OR current text -- will be applied anyway
  * @return this module's text at specified key location massaged by Render filters
  */
  virtual const char *RenderText (char *buf = 0, int len = -1);
  
  /** calls all StripFilters on current text
  *
  * @param tmpKey key to use to grab text
  * @return this module's text at specified key location massaged by Strip filters
  */
  virtual const char *StripText (SWKey * tmpKey);
  
  /** calls all RenderFilters on current text
  *
  * @param tmpKey key to use to grab text
  * @return this module's text at specified key location massaged by Render filters
  */
  virtual const char *RenderText (SWKey * tmpKey);
  
};


#endif
