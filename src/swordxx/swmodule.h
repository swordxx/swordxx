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
#include <map>
#include "defs.h"
#include "keys/listkey.h"
#include "swconfig.h"



namespace swordxx {

class SWOptionFilter;
class SWFilter;

#define SEARCHFLAG_MATCHWHOLEENTRY 4096

using AttributeValue = std::map<std::string, std::string>;

/**
 * The class SWModule is the base class for all modules used in Sword++.
 * It provides functions to look up a text passage, to search in the module,
 * to switch on/off the state of optional things like Strong's numbers or
 * footnotes.
 *
 * SWModule has also functions to write to the data files.
 */
class SWDLLEXPORT SWModule {

private: /* Types: */

    using FilterList = std::list<std::shared_ptr<SWFilter> >;
    using OptionFilterList = std::list<std::shared_ptr<SWOptionFilter> >;
    using AttributeList = std::map<std::string, AttributeValue>;
    using AttributeTypeList = std::map<std::string, AttributeList>;

private: /* Fields: */

    /** filters to be executed to remove all markup (for searches) */
    FilterList stripFilters;

    /** filters to be executed immediately upon file read */
    FilterList rawFilters;

    /** filters to be executed to format for display */
    FilterList renderFilters;

    /** filters to be executed to change markup to user preferences */
    OptionFilterList optionFilters;

    /** filters to be executed to decode text for display */
    FilterList encodingFilters;

    ConfigEntMap ownConfig;

    std::string modname;
    std::string moddesc;
    std::string modtype;
    std::string modlang;

    ConfigEntMap * config{&ownConfig};
    mutable AttributeTypeList entryAttributes;
    mutable bool procEntAttr = true;

    ListKey listKey;

    SWTextDirection direction;
    SWTextMarkup markup;
    TextEncoding encoding;

protected:

    mutable char error = 0;
    bool skipConsecutiveLinks = true;

    /** the current key */
    SWKey *key;

    mutable std::string entryBuf;

    mutable int entrySize = -1;
    mutable long entryIndex;     // internal common storage for index

    static void prepText(std::string &buf);

public:

    /**
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
    std::string getBibliography(BibliographyFormat format = BIB_BIBTEX) const;

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

    /** \returns the current key of this module. */
    SWKey *getKey() const { return key; }

    /**
     * gets the key text for the module.
     * do we really need this?
     */
    virtual const char *getKeyText() const { return getKey()->getText(); }

    virtual long getIndex() const { return entryIndex; }
    virtual void setIndex(long iindex) { entryIndex = iindex; }

    std::string const & getName() const { return modname; }
    std::string const & getDescription() const { return moddesc; }
    std::string const & getType() const { return modtype; }

    void setType(char const * imodtype)
    { modtype = (imodtype ? imodtype : ""); }

    inline SWTextDirection getDirection() const noexcept { return direction; }

    TextEncoding getEncoding() const { return encoding; }

    SWTextMarkup getMarkup() const { return markup; }

    std::string const & getLanguage() const { return modlang; }


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
     * @param percent Callback function to get the current search status in %.
     * @param percentUserData User data that is given to the callback function as parameter.
     *
     * @return ListKey set to verses that contain istr
     */
    ListKey & search(char const * istr,
                     int searchType = 0,
                     int flags = 0,
                     SWKey * scope = nullptr,
                     void (* percent)(char, void *) = &nullPercent,
                     void * percentUserData = nullptr);

    /** Allocates a key of specific type for module
     * The different reimplementations of SWModule (e.g. SWText) support SWKey implementations,
     * which support special.  This functions returns a SWKey object which works with the current
     * implementation of SWModule. For example for the SWText class it returns a VerseKey object.
     * @see VerseKey, ListKey, SWText, SWLD, SWCom
     * @return pointer to allocated key.
     */
    virtual std::unique_ptr<SWKey> createKey() const = 0;

    std::string const & getRawEntry() const { return getRawEntryBuf(); }

    // write interface ----------------------------

    virtual bool isWritable() const { return false; }

    /** Modify the current module entry text - only if module isWritable() */
    virtual void setEntry(const char *inbuf, long len= -1) {
        (void) inbuf;
        (void) len;
    }

    /** Link the current module entry to another module entry - only if
     *    module isWritable()
     */
    virtual void linkEntry(SWKey const & sourceKey) { (void) sourceKey; }

    /** Delete current module entry - only if module isWritable() */
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
    void filterBuffer(OptionFilterList const & filters,
                      std::string & buf,
                      SWKey const * key) const;

    /** FilterBuffer a text buffer
     * @param filters the FilterList of filters to iterate
     * @param buf the buffer to filter
     * @param key key location from where this buffer was extracted
     */
    void filterBuffer(FilterList const & filters,
                      std::string & buf,
                      SWKey const * key) const;

    /** Adds a RenderFilter to this module's renderFilters queue.
     *    Render Filters are called when the module is asked to produce
     *    renderable text.
     * @param newfilter the filter to add
     * @return *this
     */
    SWModule & addRenderFilter(std::shared_ptr<SWFilter> newFilter) {
        renderFilters.emplace_back(std::move(newFilter));
        return *this;
    }

    /** Retrieves a container of render filters associated with this
     *    module.
     * @return container of render filters
     */
    FilterList const & getRenderFilters() const { return renderFilters; }

    /** Removes a RenderFilter from this module's renderFilters queue
     * @param oldfilter the filter to remove
     * @return *this
     */
    SWModule & removeRenderFilter(std::shared_ptr<SWFilter> const & oldFilter) {
        renderFilters.remove(oldFilter);
        return *this;
    }

    /** Replaces a RenderFilter in this module's renderfilters queue
     * @param oldfilter the filter to remove
     * @param newfilter the filter to add in its place
     * @return *this
     */
    SWModule & replaceRenderFilter(std::shared_ptr<SWFilter> const & oldFilter,
                                   std::shared_ptr<SWFilter> newFilter)
    {
        for (auto & filter : renderFilters)
            if (filter == oldFilter)
                filter = newFilter;
        return *this;
    }

    /** RenderFilter run a buf through this module's Render Filters
     * @param buf the buffer to filter
     * @param key key location from where this buffer was extracted
     */
    void renderFilter(std::string & buf, SWKey const * key) const {
        filterBuffer(renderFilters, buf, key);
    }

    /** Adds an EncodingFilter to this module's @see encodingFilters queue.
     *    Encoding Filters are called immediately when the module is read
     *    from data source, to assure we have desired internal data stream
     *    (e.g. UTF-8 for text modules)
     * @param newfilter the filter to add
     * @return *this
     */
    SWModule & addEncodingFilter(std::shared_ptr<SWFilter> newFilter) {
        encodingFilters.emplace_back(std::move(newFilter));
        return *this;
    }

    /** Removes an EncodingFilter from this module's encodingFilters queue
     * @param oldfilter the filter to remove
     * @return *this
     */
    SWModule & removeEncodingFilter(std::shared_ptr<SWFilter> const & oldFilter)
    {
        encodingFilters.remove(oldFilter);
        return *this;
    }

    /** Replaces an EncodingFilter in this module's encodingfilters queue
     * @param oldfilter the filter to remove
     * @param newfilter the filter to add in its place
     * @return *this
     */
    SWModule & replaceEncodingFilter(
            std::shared_ptr<SWFilter> const & oldFilter,
            std::shared_ptr<SWFilter> newFilter)
    {
        for (auto & filter : encodingFilters)
            if (filter == oldFilter)
                filter = newFilter;
        return *this;
    }

    /** encodingFilter run a buf through this module's Encoding Filters
     * @param buf the buffer to filter
     * @param key key location from where this buffer was extracted
     */
    void encodingFilter(std::string & buf, SWKey const * key) const {
        filterBuffer(encodingFilters, buf, key);
    }

    /** Adds a StripFilter to this module's stripFilters queue.
     *    Strip filters are called when a module is asked to render
     *    an entry without any markup (like when searching).
     * @param newfilter the filter to add
     * @return *this
     */
    SWModule & addStripFilter(std::shared_ptr<SWFilter> newFilter) {
        stripFilters.emplace_back(std::move(newFilter));
        return *this;
    }

    /** Adds a RawFilter to this module's rawFilters queue
     * @param newFilter the filter to add
     * @return *this
     */
    SWModule & addRawFilter(std::shared_ptr<SWFilter> newfilter) {
        rawFilters.emplace_back(std::move(newfilter));
        return *this;
    }

    /** StripFilter run a buf through this module's Strip Filters
     * @param buf the buffer to filter
     * @param key key location from where this buffer was extracted
     */
    void stripFilter(std::string & buf, SWKey const * key) const {
        filterBuffer(stripFilters, buf, key);
    }


    /** RawFilter a text buffer
     * @param buf the buffer to filter
     * @param key key location from where this buffer was extracted
     */
    void rawFilter(std::string & buf, SWKey const * key) const {
        filterBuffer(rawFilters, buf, key);
    }

    /** Adds an OptionFilter to this module's optionFilters queue.
     *    Option Filters are used to turn options in the text on
     *    or off, or so some other state (e.g. Strong's Number,
     *    Footnotes, Cross References, etc.)
     * @param newfilter the filter to add
     * @return *this
     */
    SWModule & addOptionFilter(std::shared_ptr<SWOptionFilter> newFilter) {
        optionFilters.push_back(std::move(newFilter));
        return *this;
    }

    /** OptionFilter a text buffer
     * @param buf the buffer to filter
     * @param key key location from where this buffer was extracted
     */
    void optionFilter(std::string & buf, SWKey const * key) const {
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
    std::string stripText(char const * buf = nullptr, int len = -1);

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
    char const * getRenderHeader() const;

    /** Produces plain text, without markup, of the module entry at the supplied key
     * @param tmpKey desired module entry
     * @return result buffer
     */
    std::string stripText(SWKey const * tmpKey);

    /** Produces renderable text of the module entry at the supplied key
     * @param tmpKey key to use to grab text
     * @return this module's text at specified key location massaged by Render filters
     */
    std::string renderText(SWKey const * tmpKey);

    /** Whether or not to only hit one entry when iterating encounters
     *    consecutive links when iterating
     * @param val = true means only include entry once in iteration
     */
    void setSkipConsecutiveLinks(bool val) { skipConsecutiveLinks = val; }

    /** Whether or not to only hit one entry when iterating encounters
     *    consecutive links when iterating
     */
    bool isSkipConsecutiveLinks() { return skipConsecutiveLinks; }

    virtual bool isLinked(const SWKey *, const SWKey *) const { return false; }
    virtual bool hasEntry(const SWKey *) const { return false; }

    /** Entry Attributes are special data pertaining to the current entry.
     *    To see what Entry Attributes exists for a specific entry of a module,
     *    the example examples/cmdline/lookup.cpp is a good utility which
     *    displays this information.  It is also useful as an example of how
     *    to access such.
     */
    AttributeTypeList & getEntryAttributes() const { return entryAttributes; }

    /** Processing Entry Attributes can be expensive.  This method allows
     * turning the processing off if they are not desired.  Some internal
     * engine processing turns them off (like searching) temporarily for
     * optimization.
     */
    void setProcessEntryAttributes(bool val) const { procEntAttr = val; }

    /** Whether or not we're processing Entry Attributes */
    bool isProcessEntryAttributes() const { return procEntAttr; }


    // Searching:

    /** Ask the object to build any indecies it wants for optimal searching */
    signed char createSearchFramework(
            void (* percent)(char, void *) = &nullPercent,
            void * percentUserData = nullptr);

    void deleteSearchFramework();

    bool hasSearchIndex();

protected: /* Methods: */


    /** This function is reimplemented by the different kinds
     * of module objects
     * @return the raw module text of the current entry
     */
    virtual std::string &getRawEntryBuf() const = 0;

private: /* Methods: */

    std::string searchIndexPath();

};

} /* namespace swordxx */
#endif
