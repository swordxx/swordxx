/******************************************************************************
 *
 *  encfiltmgr.h -    definition of class SWEncodingMgr, subclass of
 *            used to transcode all module text to a requested
 *            markup
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_ENCFILTERMGR_H
#define SWORDXX_ENCFILTERMGR_H

#include "swfiltermgr.h"

#include <memory>
#include "defs.h"
#include "swfilter.h"


namespace swordxx {

/** This class is like a normal SWMgr, but you can additionally specify which
    encoding you want to use. */
class SWDLLEXPORT EncodingFilterMgr : public SWFilterMgr {

private: /* Types: */

    struct EncodingFilter;

public: /* Methods: */

    /** \param[in] encoding The desired encoding. */
    EncodingFilterMgr(TextEncoding const encoding = ENC_UTF8);

    void setEncoding(TextEncoding const encoding);

    TextEncoding encoding() const noexcept;

    /**
     * Adds the raw filters which are defined in "section" to the SWModule
       object "module".
     * \param module To this module the raw filter(s) are added.
     * \param section We use this section to get a list of filters we should
                      apply to the module.
     */
    void addRawFilters(SWModule & module,
                       ConfigEntMap const & section) override;

    /**
     * Adds the encoding filters which are defined in "section" to the SWModule
       object "module".
     * \param module To this module the encoding filter(s) are added.
     * \param section We use this section to get a list of filters we should
                      apply to the module.
     */
    void addEncodingFilters(SWModule & module,
                            ConfigEntMap const & section) override;

private: /* Fields: */

    std::shared_ptr<SWFilter> const m_latin1utf8;
    std::shared_ptr<SWFilter> const m_scsuutf8;
    std::shared_ptr<SWFilter> const m_utf16utf8;
    std::shared_ptr<EncodingFilter> m_targetenc;

};

} /* namespace swordxx */

#endif /* SWORDXX_ENCFILTERMGR_H */
