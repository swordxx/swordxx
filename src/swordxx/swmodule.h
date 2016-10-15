/******************************************************************************
 *
 *  swmodule.h -    code for base class 'module'.  Module is the basis for
 *              all types of modules (e.g. texts, commentaries, maps,
 *              lexicons, etc.)
 *
 * $Id$
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
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

#ifndef    _WIN32_WCE
#include <iostream>
#endif
#include <list>
#include "defs.h"
#include "keys/listkey.h"
#include "swconfig.h"



namespace swordxx {

class SWOptionFilter;
class SWFilter;

#define SEARCHFLAG_MATCHWHOLEENTRY 4096

#define SWMODULE_OPERATORS \
    operator std::string() { return renderText(); } \
    operator SWKey &() { return *getKey(); } \
    operator SWKey *() { return getKey(); } \
    SWModule &operator <<(const char *inbuf) { setEntry(inbuf); return *this; } \
    SWModule &operator <<(const SWKey *sourceKey) { linkEntry(sourceKey); return *this; } \
    SWModule &operator -=(int steps) { decrement(steps); return *this; } \
    SWModule &operator +=(int steps) { increment(steps); return *this; } \
    SWModule &operator ++(int) { return *this += 1; } \
    SWModule &operator --(int) { return *this -= 1; } \
    SWModule & operator=(decltype(Position::Top)) \
    { positionToTop(); return *this; } \
    SWModule & operator=(decltype(Position::Bottom)) \
    { positionToBottom(); return *this; } \


typedef std::list < SWFilter * >FilterList;
typedef std::list < SWOptionFilter * >OptionFilterList;
typedef std::map < std::string, std::string, std::less < std::string > > AttributeValue;
typedef std::map < std::string, AttributeValue, std::less < std::string > > AttributeList;
typedef std::map < std::string, AttributeList, std::less < std::string > > AttributeTypeList;

#define SWTextDirection char
#define SWTextMarkup char

/**
 * The class SWModule is the base class for all modules used in Sword++.
 * It provides functions to look up a text passage, to search in the module,
 * to switch on/off the state of optional things like Strong's numbers or
 * footnotes.
 *
 * SWModule has also functions to write to the data files.
 */

class SWDLLEXPORT SWModule {

private: /* Fields: */

    /** filters to be executed to remove all markup (for searches) */
    FilterList stripFilters;

    /** filters to be executed immediately upon fileread */
    FilterList rawFilters;

    /** filters to be executed to format for display */
    FilterList renderFilters;

    /** filters to be executed to change markup to user prefs */
    OptionFilterList optionFilters;

    /** filters to be executed to decode text for display */
    FilterList encodingFilters;

    ConfigEntMap ownConfig;

    char * modname = nullptr;
    char * moddesc = nullptr;
    char * modtype = nullptr;
    char * modlang = nullptr;

protected:

    ConfigEntMap * config{&ownConfig};
    mutable AttributeTypeList entryAttributes;
    mutable bool procEntAttr = true;

    mutable char error = 0;
    bool skipConsecutiveLinks = true;

    /** the current key */
    SWKey *key;

    ListKey listKey;

    char direction;
    char markup;
    TextEncoding encoding;

    mutable std::string entryBuf;

    mutable int entrySize = -1;
    mutable long entryIndex;     // internal common storage for index

    static void prepText(std::string &buf);


public:

    /** SWModule c-tor
     *
     * @param key_ the default key.
     * @param imodname Internal name for module; see also getName()
     * @param imoddesc Name to display to user for module; see also getDescription()
     * @param imodtype Type of module (e.g. Biblical Text, Commentary, etc.); see also getType()
     * @param encoding Encoding of the module (e.g. UTF-8)
     * @param dir Direction of text flow (e.g. Right to Left for Hebrew)
     * @param markup Source Markup of the module (e.g. OSIS)
     * @param modlang Language of the module (e.g. en)
     */
    SWModule(SWKey * key_,
             char const * imodname = nullptr,
             char const * imoddesc = nullptr,
             char const * imodtype = nullptr,
             TextEncoding encoding = ENC_UNKNOWN,
             SWTextDirection dir = DIRECTION_LTR,
             SWTextMarkup markup = FMT_UNKNOWN,
             char const * modlang = nullptr);

    /** SWModule d-tor
     */
    virtual ~SWModule();

    virtual void flush() {}

    /** Gets and clears error status
     *
     * @return error status
     */
    char popError();

    /**
     * @return  True if this module is encoded in Unicode, otherwise returns false.
     */
    bool isUnicode() const
    { return (encoding == ENC_UTF8 || encoding == ENC_SCSU); }

    // These methods are useful for modules that come from a standard Sword++ install (most do).
    // SWMgr will call setConfig.  The user may use getConfig and getConfigEntry (if they
    // are not comfortable with, or don't wish to use  stl maps).
    void setConfig(ConfigEntMap * conf) { config = conf; }
    const ConfigEntMap &getConfig() const { return *config; }

    /**
     * Gets a configuration property about a module.  These entries are primarily
     * pulled from the module's .conf file, but also includes some virtual entries
     * such as:
     *     PrefixPath - the absolute filesystem path to the Sword++ module repository
     *    location where this module is located.
     *    AbsoluteDataPath - the full path to the root folder where the module
     *    data is stored.
     */
    const char *getConfigEntry(const char *key) const;

    /**
     * Returns bibliographic data for a module in the requested format
     *
     * @param bibFormat format of the bibliographic data
     * @return bibliographic data in the requested format as a string (BibTeX by default)
     */
    std::string getBibliography(unsigned char bibFormat = BIB_BIBTEX) const;

    /**
     * @return The size of the text entry for the module's current key position.
     */
    int getEntrySize() const { return entrySize; }

    /**
     * Sets a key to this module for position to a particular record
     *
     * @param ikey key with which to set this module
     * @return error status
     */
    char setKey(const SWKey *ikey);

    /**
     * Sets a key to this module for position to a particular record
     * @param ikey The SWKey which should be used as new key.
     * @return Error status
     */
    char setKey(const SWKey &ikey) { return setKey(&ikey); }

    /** Gets the current module key
     * @return the current key of this module
     */
    SWKey *getKey() const { return key; }

    /**
     * gets the key text for the module.
     * do we really need this?
     */

    virtual const char *getKeyText() const { return getKey()->getText(); }


    virtual long getIndex() const { return entryIndex; }
    virtual void setIndex(long iindex) { entryIndex = iindex; }


    /** Gets module name
     *
     * @return pointer to modname
     */
    const char *getName() const { return modname; }

    /** Gets module description
     *
     * @return pointer to moddesc
     */
    const char *getDescription() const { return moddesc; }

    /** Gets module type
     *
     * @return pointer to modtype
     */
    const char *getType() const { return modtype; }

    /** Sets module type
     *
     * @param imodtype Value which to set modtype; [0]-only get
     * @return pointer to modtype
     */
    void setType(const char *imodtype) { stdstr(&modtype, imodtype); }

    /** Sets/gets module direction
     *
     * @param newdir Value which to set direction; [-1]-only get
     * @return new direction
     */
    inline char getDirection() const noexcept { return direction; }

    /** \returns the module encoding. */
    TextEncoding getEncoding() const { return encoding; }

    /** Sets/gets module markup
     *
     * @param markup Value which to set markup; [-1]-only get
     * @return Markup
     */
    char getMarkup() const { return markup; }

    /** Sets/gets module language
     *
     * @param imodlang Value which to set modlang; [0]-only get
     * @return pointer to modlang
     */
    const char *getLanguage() const { return modlang; }


    // search interface -------------------------------------------------

    /**
    * This is the default callback function for searching.
    * This function is a placeholder and does nothing.
    * You can define your own function for search progress
    * evaluation, and pass it over to Search().
    */
    static void nullPercent(char percent, void *userData);

    /** Searches a module for a string
     *
     * @param istr string for which to search
     * @param searchType type of search to perform
     *            >=0 - regex
     *            -1  - phrase
     *            -2  - multiword
     *            -3  - entryAttrib (eg. Word//Strongs/G1234/)
     *            -4  - Lucene
     * @param flags options flags for search
     * @param scope Key containing the scope. VerseKey or ListKey are useful here.
     * @param justCheckIfSupported if set, don't search,
     * only tell if this function supports requested search.
     * @param percent Callback function to get the current search status in %.
     * @param percentUserData User data that is given to the callback function as parameter.
     *
     * @return ListKey set to verses that contain istr
     */
    ListKey & search(char const * istr,
                     int searchType = 0,
                     int flags = 0,
                     SWKey * scope = nullptr,
                     bool * justCheckIfSupported = nullptr,
                     void (* percent)(char, void *) = &nullPercent,
                     void * percentUserData = nullptr);

    /** Allocates a key of specific type for module
     * The different reimplementations of SWModule (e.g. SWText) support SWKey implementations,
     * which support special.  This functions returns a SWKey object which works with the current
     * implementation of SWModule. For example for the SWText class it returns a VerseKey object.
     * @see VerseKey, ListKey, SWText, SWLD, SWCom
     * @return pointer to allocated key. Caller is responsible for deleting the object
     */
    virtual SWKey *createKey() const = 0;

    std::string const & getRawEntry() const { return getRawEntryBuf(); }

    // write interface ----------------------------
    /** Is the module writable? :)
     * @return yes or no
     */
    virtual bool isWritable() const { return false; }

    /** Modify the current module entry text - only if module isWritable()
     */
    virtual void setEntry(const char *inbuf, long len= -1) {
        (void) inbuf;
        (void) len;
    }

    /** Link the current module entry to another module entry - only if
     *    module isWritable()
     */
    virtual void linkEntry(SWKey const * sourceKey) { (void) sourceKey; }

    /** Delete current module entry - only if module isWritable()
     */
    virtual void deleteEntry() {}

    // end write interface ------------------------

    /** Decrements module key a number of entries
     * @param steps Number of entries to jump backward
     */
    virtual void decrement(int steps = 1);

    /** Increments module key a number of entries
     * @param steps Number of entries to jump forward
     */
    virtual void increment(int steps = 1);

    virtual void positionToTop();
    virtual void positionToBottom();

    /** OptionFilterBuffer a text buffer
     * @param filters the FilterList of filters to iterate
     * @param buf the buffer to filter
     * @param key key location from where this buffer was extracted
     */
    virtual void filterBuffer(OptionFilterList const & filters,
                              std::string & buf,
                              SWKey const * key) const;

    /** FilterBuffer a text buffer
     * @param filters the FilterList of filters to iterate
     * @param buf the buffer to filter
     * @param key key location from where this buffer was extracted
     */
    virtual void filterBuffer(FilterList const & filters,
                              std::string & buf,
                              SWKey const * key) const;

    /** Adds a RenderFilter to this module's renderFilters queue.
     *    Render Filters are called when the module is asked to produce
     *    renderable text.
     * @param newfilter the filter to add
     * @return *this
     */
    virtual SWModule &addRenderFilter(SWFilter *newFilter) {
        renderFilters.push_back(newFilter);
        return *this;
    }

    /** Retrieves a container of render filters associated with this
     *    module.
     * @return container of render filters
     */
    virtual const FilterList &getRenderFilters() const {
        return renderFilters;
    }

    /** Removes a RenderFilter from this module's renderFilters queue
     * @param oldfilter the filter to remove
     * @return *this
     */
    virtual SWModule &removeRenderFilter(SWFilter *oldFilter) {
        renderFilters.remove(oldFilter);
        return *this;
    }

    /** Replaces a RenderFilter in this module's renderfilters queue
     * @param oldfilter the filter to remove
     * @param newfilter the filter to add in its place
     * @return *this
     */
    virtual SWModule &replaceRenderFilter(SWFilter *oldFilter, SWFilter *newFilter) {
        for (auto & filter : renderFilters)
            if (filter == oldFilter)
                filter = newFilter;
        return *this;
    }

    /** RenderFilter run a buf through this module's Render Filters
     * @param buf the buffer to filter
     * @param key key location from where this buffer was extracted
     */
    virtual void renderFilter(std::string &buf, const SWKey *key) const {
        filterBuffer(renderFilters, buf, key);
    }

    /** Adds an EncodingFilter to this module's @see encodingFilters queue.
     *    Encoding Filters are called immediately when the module is read
     *    from data source, to assure we have desired internal data stream
     *    (e.g. UTF-8 for text modules)
     * @param newfilter the filter to add
     * @return *this
     */
    virtual SWModule &addEncodingFilter(SWFilter *newFilter) {
        encodingFilters.push_back(newFilter);
        return *this;
    }

    /** Removes an EncodingFilter from this module's encodingFilters queue
     * @param oldfilter the filter to remove
     * @return *this
     */
    virtual SWModule &removeEncodingFilter(SWFilter *oldFilter) {
        encodingFilters.remove(oldFilter);
        return *this;
    }

    /** Replaces an EncodingFilter in this module's encodingfilters queue
     * @param oldfilter the filter to remove
     * @param newfilter the filter to add in its place
     * @return *this
     */
    virtual SWModule &replaceEncodingFilter(SWFilter *oldFilter, SWFilter *newFilter) {
        for (auto & filter : encodingFilters)
            if (filter == oldFilter)
                filter = newFilter;
        return *this;
    }

    /** encodingFilter run a buf through this module's Encoding Filters
     * @param buf the buffer to filter
     * @param key key location from where this buffer was extracted
     */
    virtual void encodingFilter(std::string &buf, const SWKey *key) const {
        filterBuffer(encodingFilters, buf, key);
    }

    /** Adds a StripFilter to this module's stripFilters queue.
     *    Strip filters are called when a module is asked to render
     *    an entry without any markup (like when searching).
     * @param newfilter the filter to add
     * @return *this
     */
    virtual SWModule &addStripFilter(SWFilter *newFilter) {
        stripFilters.push_back(newFilter);
        return *this;
    }

    /** Adds a RawFilter to this module's rawFilters queue
     * @param newFilter the filter to add
     * @return *this
     */
    virtual SWModule &addRawFilter(SWFilter *newfilter) {
        rawFilters.push_back(newfilter);
        return *this;
    }

    /** StripFilter run a buf through this module's Strip Filters
     * @param buf the buffer to filter
     * @param key key location from where this buffer was extracted
     */
    virtual void stripFilter(std::string &buf, const SWKey *key) const {
        filterBuffer(stripFilters, buf, key);
    }


    /** RawFilter a text buffer
     * @param buf the buffer to filter
     * @param key key location from where this buffer was extracted
     */
    virtual void rawFilter(std::string &buf, const SWKey *key) const {
        filterBuffer(rawFilters, buf, key);
    }

    /** Adds an OptionFilter to this module's optionFilters queue.
     *    Option Filters are used to turn options in the text on
     *    or off, or so some other state (e.g. Strong's Number,
     *    Footnotes, Cross References, etc.)
     * @param newfilter the filter to add
     * @return *this
     */
    virtual SWModule &addOptionFilter(SWOptionFilter *newFilter) {
        optionFilters.push_back(newFilter);
        return *this;
    }

    /** OptionFilter a text buffer
     * @param buf the buffer to filter
     * @param key key location from where this buffer was extracted
     */
    virtual void optionFilter(std::string &buf, const SWKey *key) const {
        filterBuffer(optionFilters, buf, key);
    }

    /** Produces plain text, without markup, of the current module entry,
     *    or supplied text
     *
     * @param buf buf to massage instead of current module entry;
     *    if buf is 0, the current text will be used
     * @param len max len to process
     * @return result buffer
     */
    virtual std::string stripText(char const * buf = nullptr, int len = -1);

    /** Produces renderable text of the current module entry or supplied text
     *
     * @param buf buffer to massage instead of current module entry;
     *    if buf is 0, the current module position text will be used
     * @param len max len to process
     * @param render for internal use
     * @return result buffer
     */
    std::string renderText(const char *buf = nullptr,
                     int len = -1,
                     bool render = true) const;

    /** Produces any header data which might be useful which is associated with the
     *    processing done with this filter.  A typical example is a suggested
     *    CSS style block for classed containers.
     */
    virtual const char *getRenderHeader() const;

    /** Produces plain text, without markup, of the module entry at the supplied key
     * @param tmpKey desired module entry
     * @return result buffer
     */
    virtual std::string stripText(const SWKey *tmpKey);

    /** Produces renderable text of the module entry at the supplied key
     * @param tmpKey key to use to grab text
     * @return this module's text at specified key location massaged by Render filters
     */
    std::string renderText(const SWKey *tmpKey);

    /** Whether or not to only hit one entry when iterating encounters
     *    consecutive links when iterating
     * @param val = true means only include entry once in iteration
     */
    virtual void setSkipConsecutiveLinks(bool val) { skipConsecutiveLinks = val; }

    /** Whether or not to only hit one entry when iterating encounters
     *    consecutive links when iterating
     */
    virtual bool isSkipConsecutiveLinks() { return skipConsecutiveLinks; }

    virtual bool isLinked(const SWKey *, const SWKey *) const { return false; }
    virtual bool hasEntry(const SWKey *) const { return false; }

    /** Entry Attributes are special data pertaining to the current entry.
     *    To see what Entry Attributes exists for a specific entry of a module,
     *    the example examples/cmdline/lookup.cpp is a good utility which
     *    displays this information.  It is also useful as an example of how
     *    to access such.
     */
    virtual AttributeTypeList &getEntryAttributes() const { return entryAttributes; }

    /** Processing Entry Attributes can be expensive.  This method allows
     * turning the processing off if they are not desired.  Some internal
     * engine processing turns them off (like searching) temporarily for
     * optimization.
     */
    virtual void setProcessEntryAttributes(bool val) const { procEntAttr = val; }

    /** Whether or not we're processing Entry Attributes
     */
    virtual bool isProcessEntryAttributes() const { return procEntAttr; }


    // Searching:

    /**
     * ask the object to build any indecies it wants for optimal searching
     */
    signed char createSearchFramework(
            void (* percent)(char, void *) = &nullPercent,
            void * percentUserData = nullptr);

    void deleteSearchFramework();

    /**
     * Check if the search is optimally supported (e.g. if index files are
     * presnt and working)
     * This function checks whether the search framework may work in the
     * best way.
     * @return true if the the search is optimally supported, false if
     * it's not working in the best way.
     */
    bool isSearchOptimallySupported(const char *istr, int searchType, int flags, SWKey *scope);

    // OPERATORS -----------------------------------------------------------------
    SWMODULE_OPERATORS

protected: /* Methods: */


    /** This function is reimplemented by the different kinds
     * of module objects
     * @return the raw module text of the current entry
     */
    virtual std::string &getRawEntryBuf() const = 0;

};

} /* namespace swordxx */
#endif
