/******************************************************************************
 *
 *  swfiltermgr.h -    definition of class SWFilterMgr used as an interface to
 *            manage filters on a module
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

#ifndef SWORDXX_SWFILTERMGR_H
#define SWORDXX_SWFILTERMGR_H

#include "defs.h"
#include "swconfig.h"


namespace swordxx {

class SWModule;
class SWMgr;

/** Class to manage different kinds of filters.
*/
class SWDLLEXPORT SWFilterMgr {

public: /* Methods: */

    virtual ~SWFilterMgr() noexcept;

    virtual void addGlobalOptions(SWModule & module,
                                  ConfigEntMap const & section,
                                  ConfigEntMap::const_iterator start,
                                  ConfigEntMap::const_iterator end);

    virtual void addLocalOptions(SWModule & module,
                                 ConfigEntMap const & section,
                                 ConfigEntMap::const_iterator start,
                                 ConfigEntMap::const_iterator end);

    /**
      Adds the encoding filters which are defined in "section" to the SWModule
      object "module".
      \param module To this module the encoding filter(s) are added.
      \param section We use this section to get a list of filters we should
                     apply to the module.
    */
    virtual void addEncodingFilters(SWModule & module,
                                    ConfigEntMap const & section);


    /**
      Adds the render filters which are defined in "section" to the SWModule
      object "module".
      \param module To this module the render filter(s) are added.
      \param section We use this section to get a list of filters we should
                     apply to the module.
    */
    virtual void addRenderFilters(SWModule & module,
                                  ConfigEntMap const & section);


    /**
      Adds the strip filters which are defined in "section" to the SWModule
      object "module".
      \param module To this module the strip filter(s) are added.
      \param section We use this section to get a list of filters we should
                     apply to the module.
    */
    virtual void addStripFilters(SWModule & module,
                                 ConfigEntMap const & section);


    /**
      Adds the raw filters which are defined in "section" to the SWModule object
      "module".
      \param module To this module the raw filter(s) are added.
      \param section We use this section to get a list of filters we should
                     apply to the module.
    */
    virtual void addRawFilters(SWModule & module,
                               ConfigEntMap const & section);

};

} /* namespace swordxx */

#endif /* SWORDXX_SWFILTERMGR_H */
