/***************************************************************************
 *
 *  swoptfilter.h -    Implenetation of SWOptionFilter
 *
 * $Id$
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWOPTFILTER_H
#define SWOPTFILTER_H

#include "swfilter.h"

#include <list>
#include <string>


namespace swordxx {

  /** Base class for all option filters.
  */
class SWDLLEXPORT SWOptionFilter : public virtual SWFilter {
protected:
    std::string optionValue;
    const char *optName;
    const char *optTip;
    const std::list<std::string> *optValues;
    bool option;
    bool isBooleanVal;
public:

    SWOptionFilter();
    SWOptionFilter(char const * oName,
                   char const * oTip,
                   std::list<std::string> const * oValues);
    virtual ~SWOptionFilter();


    /** many options are simple Off/On boolean type, and frontends may wish to show these
     * with checkmarks or the like to the end user.  This is a convenience method
     * to allow a frontend to check if this filter has only Off/On values
     */
    bool isBoolean() { return isBooleanVal; }

    /** gets the name of the option of this filter
     * @return option name
     */
    virtual const char *getOptionName() { return optName; }

    /** gets a short explanation of the option of this filter;
     * it could be presented to the user in frontend programs
     * @return option tip/explanation
     */
    virtual const char *getOptionTip() { return optTip; }

    /** returns a list of the possible option values
     *
     * @return list of option values
     */
    virtual std::list<std::string> getOptionValues() { return *optValues; }

    /** @return The value of the current option.
    */
    virtual const char *getOptionValue();

    /** sets the value of the option of this filter,
     * e.g maybe a strong's filter mioght be set to "on" / "off" -
     * that would mean to show or not to show the strongs in the text,
     * see also getOptionValues()
     * @param ival the new option value
     */
    virtual void setOptionValue(const char *ival);

};

} /* namespace swordxx */
#endif
