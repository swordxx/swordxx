/******************************************************************************
 *
 *  swmodule.h -    code for base class 'module'.  Module is the basis for
 *              all types of modules (e.g. texts, commentaries, maps,
 *              lexicons, etc.)
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

#ifndef SWORDXX_SWMODULE_H
#define SWORDXX_SWMODULE_H

#include <list>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include "defs.h"
#include "swconfig.h"


namespace swordxx {

class SWFilter;
class SWKey;
class SWOptionFilter;

using AttributeValue = std::map<std::string, std::string>;

/**
 * The class SWModule is the base class for all modules used in Sword++.
 * It provides functions to look up a text passage, to switch on/off the state
 * of optional things like Strong's numbers or footnotes.
 *
 * SWModule has also functions to write to the data files.
 */
class SWDLLEXPORT SWModule {

public: /* Types: */

    using FilterList = std::list<std::shared_ptr<SWFilter> >;
    using OptionFilterList = std::list<std::shared_ptr<SWOptionFilter> >;
    using AttributeList = std::map<std::string, AttributeValue>;
    using AttributeTypeList = std::map<std::string, AttributeList>;

private: /* Fields: */

    /** filters to be executed to remove all markup (for searches) */
    FilterList m_stripFilters;

    /** filters to be executed immediately upon file read */
    FilterList m_rawFilters;

    /** filters to be executed to format for display */
    FilterList m_renderFilters;

    /** filters to be executed to change markup to user preferences */
    OptionFilterList m_optionFilters;

    /** filters to be executed to decode text for display */
    FilterList m_encodingFilters;

    std::string m_moduleName;
    std::string m_moduleDescription;
    std::string m_moduleType;
    std::string m_moduleLanguageName;

    ConfigEntMap m_config;
    mutable AttributeTypeList m_entryAttributes;
    mutable bool m_processEntryAttributes = true;

    SWTextDirection m_textDirection;
    SWTextMarkup m_textMarkup;
    TextEncoding m_textEncoding;
    bool m_skipConsecutiveLinks = true;

    std::shared_ptr<SWKey> m_currentKey;

protected:

    mutable char error = 0;

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
    SWModule(std::shared_ptr<SWKey> key_,
             char const * imodname = nullptr,
             char const * imoddesc = nullptr,
             char const * imodtype = nullptr,
             TextEncoding encoding = ENC_UNKNOWN,
             SWTextDirection dir = DIRECTION_LTR,
             SWTextMarkup markup = FMT_UNKNOWN,
             char const * modlang = nullptr);

    SWModule(SWModule &&) = delete;
    SWModule(SWModule const &) = delete;

    virtual ~SWModule();

    SWModule & operator=(SWModule &&) = delete;
    SWModule & operator=(SWModule const &) = delete;

    virtual void flush();

    /** Gets and clears error status
     *
     * @return error status
     */
    char popError();

    /**
     * @return  True if this module is encoded in Unicode, otherwise returns false.
     */
    bool isUnicode() const noexcept;

    // These methods are useful for modules that come from a standard Sword++ install (most do).
    // SWMgr will call setConfig.  The user may use getConfig and getConfigEntry (if they
    // are not comfortable with, or don't wish to use  stl maps).
    void setConfig(ConfigEntMap conf) noexcept {
        static_assert(std::is_nothrow_move_assignable<ConfigEntMap>::value, "");
        m_config = std::move(conf);
    }
    const ConfigEntMap &getConfig() const noexcept { return m_config; }

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
     * Sets a key to this module for position to a particular record
     *
     * @param ikey key with which to set this module
     */
    void setKey(std::shared_ptr<SWKey> ikey) noexcept;

    /** \returns the current key of this module. */
    std::shared_ptr<SWKey> getKey() const noexcept { return m_currentKey; }

    /** \returns the current key of this module. */
    template <typename T>
    std::shared_ptr<T> getKeyAs() const noexcept
    { return std::static_pointer_cast<T>(m_currentKey); }

    char setKey(std::string_view positionFrom);

    /**
     * gets the key text for the module.
     * do we really need this?
     */
    virtual std::string getKeyText() const;

    std::string const & getName() const { return m_moduleName; }
    std::string const & getDescription() const { return m_moduleDescription; }
    std::string const & getType() const { return m_moduleType; }

    void setType(std::string imodtype)
            noexcept(std::is_nothrow_move_assignable<std::string>::value)
    { m_moduleType = std::move(imodtype); }

    inline SWTextDirection getDirection() const noexcept
    { return m_textDirection; }

    TextEncoding getEncoding() const { return m_textEncoding; }

    SWTextMarkup getMarkup() const { return m_textMarkup; }

    std::string const & getLanguage() const { return m_moduleLanguageName; }

    /** Allocates a key of specific type for module
     * The different reimplementations of SWModule (e.g. SWText) support SWKey implementations,
     * which support special.  This functions returns a SWKey object which works with the current
     * implementation of SWModule. For example for the SWText class it returns a VerseKey object.
     * @see VerseKey, ListKey, SWText, SWLD, SWCom
     * @return pointer to allocated key.
     */
    virtual std::shared_ptr<SWKey> createKey() const = 0;

    std::string getRawEntry() const;

    // write interface ----------------------------

    virtual bool isWritable() const noexcept;

    /** Modify the current module entry text - only if module isWritable() */
    virtual void setEntry(std::string_view text);

    /** Link the current module entry to another module entry - only if
     *    module isWritable()
     */
    virtual void linkEntry(SWKey const & sourceKey);

    /** Delete current module entry - only if module isWritable() */
    virtual void deleteEntry();

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

    /** Adds a RenderFilter to this module's renderFilters queue.
     *    Render Filters are called when the module is asked to produce
     *    renderable text.
     * @param newFilter the filter to add
     * @return *this
     */
    SWModule & addRenderFilter(std::shared_ptr<SWFilter> newFilter) {
        m_renderFilters.emplace_back(std::move(newFilter));
        return *this;
    }

    /** Retrieves a container of render filters associated with this
     *    module.
     * @return container of render filters
     */
    FilterList const & getRenderFilters() const { return m_renderFilters; }

    /**
      \returns a reference to the options filters associated with this module.
    */
    OptionFilterList const & getOptionFilters() const
    { return m_optionFilters; }

    /** Removes a RenderFilter from this module's renderFilters queue
     * @param oldFilter the filter to remove
     * @return *this
     */
    SWModule & removeRenderFilter(std::shared_ptr<SWFilter> const & oldFilter) {
        m_renderFilters.remove(oldFilter);
        return *this;
    }

    /** Replaces a RenderFilter in this module's renderfilters queue
     * @param oldFilter the filter to remove
     * @param newFilter the filter to add in its place
     * @return *this
     */
    SWModule & replaceRenderFilter(std::shared_ptr<SWFilter> const & oldFilter,
                                   std::shared_ptr<SWFilter> newFilter)
    {
        for (auto & filter : m_renderFilters)
            if (filter == oldFilter)
                filter = newFilter;
        return *this;
    }

    /** Adds an EncodingFilter to this module's @see encodingFilters queue.
     *    Encoding Filters are called immediately when the module is read
     *    from data source, to assure we have desired internal data stream
     *    (e.g. UTF-8 for text modules)
     * @param newFilter the filter to add
     * @return *this
     */
    SWModule & addEncodingFilter(std::shared_ptr<SWFilter> newFilter) {
        m_encodingFilters.emplace_back(std::move(newFilter));
        return *this;
    }

    /** Removes an EncodingFilter from this module's encodingFilters queue
     * @param oldFilter the filter to remove
     * @return *this
     */
    SWModule & removeEncodingFilter(std::shared_ptr<SWFilter> const & oldFilter)
    {
        m_encodingFilters.remove(oldFilter);
        return *this;
    }

    /** Replaces an EncodingFilter in this module's encodingfilters queue
     * @param oldFilter the filter to remove
     * @param newFilter the filter to add in its place
     * @return *this
     */
    SWModule & replaceEncodingFilter(
            std::shared_ptr<SWFilter> const & oldFilter,
            std::shared_ptr<SWFilter> newFilter)
    {
        for (auto & filter : m_encodingFilters)
            if (filter == oldFilter)
                filter = newFilter;
        return *this;
    }

    /** Adds a StripFilter to this module's stripFilters queue.
     *    Strip filters are called when a module is asked to render
     *    an entry without any markup (like when searching).
     * @param newFilter the filter to add
     * @return *this
     */
    SWModule & addStripFilter(std::shared_ptr<SWFilter> newFilter) {
        m_stripFilters.emplace_back(std::move(newFilter));
        return *this;
    }

    /** Adds a RawFilter to this module's rawFilters queue
     * @param newFilter the filter to add
     * @return *this
     */
    SWModule & addRawFilter(std::shared_ptr<SWFilter> newFilter) {
        m_rawFilters.emplace_back(std::move(newFilter));
        return *this;
    }


    /** RawFilter a text buffer
     * @param buf the buffer to filter
     * @param key_ key location from where this buffer was extracted
     */
    void rawFilter(std::string & buf, SWKey const * key_) const;

    /** Adds an OptionFilter to this module's optionFilters queue.
     *    Option Filters are used to turn options in the text on
     *    or off, or so some other state (e.g. Strong's Number,
     *    Footnotes, Cross References, etc.)
     * @param newFilter the filter to add
     * @return *this
     */
    SWModule & addOptionFilter(std::shared_ptr<SWOptionFilter> newFilter) {
        m_optionFilters.push_back(std::move(newFilter));
        return *this;
    }

    /** Produces plain text, without markup, of the current module entry
     *
     * @return result buffer
     */
    std::string stripText();

    /** Produces plain text, without markup, of the supplied text
     *
     * @param buf the buffer to massage
     * @return result buffer
     */
    std::string stripText(std::string buf);

    /** Produces renderable text of the current module entry
     *
     * @return result buffer
     */
    std::string renderText() const;

    /** Produces renderable text of the supplied text
     *
     * @param buf buffer to massage instead of current module entry;
     * @return result buffer
     */
    std::string renderText(std::string buf) const;

    /** Produces any header data which might be useful which is associated with the
     *    processing done with this filter.  A typical example is a suggested
     *    CSS style block for classed containers.
     */
    char const * getRenderHeader() const;

    /** Whether or not to only hit one entry when iterating encounters
     *    consecutive links when iterating
     * @param val = true means only include entry once in iteration
     */
    void setSkipConsecutiveLinks(bool val) { m_skipConsecutiveLinks = val; }

    /** Whether or not to only hit one entry when iterating encounters
     *    consecutive links when iterating
     */
    bool isSkipConsecutiveLinks() { return m_skipConsecutiveLinks; }

    virtual bool isLinked(SWKey const &, SWKey const &) const;
    virtual bool hasEntry(SWKey const &) const;

    /** Entry Attributes are special data pertaining to the current entry.
     *    To see what Entry Attributes exists for a specific entry of a module,
     *    the example examples/cmdline/lookup.cpp is a good utility which
     *    displays this information.  It is also useful as an example of how
     *    to access such.
     */
    AttributeTypeList & getEntryAttributes() const { return m_entryAttributes; }

    /** Processing Entry Attributes can be expensive.  This method allows
     * turning the processing off if they are not desired.  Some internal
     * engine processing turns them off (like searching) temporarily for
     * optimization.
     */
    void setProcessEntryAttributes(bool val) const
    { m_processEntryAttributes = val; }

    /** Whether or not we're processing Entry Attributes */
    bool isProcessEntryAttributes() const { return m_processEntryAttributes; }

protected: /* Methods: */

    virtual std::string getRawEntryImpl() const = 0;

};

} /* namespace swordxx */

#endif /* SWORDXX_SWMODULE_H */
