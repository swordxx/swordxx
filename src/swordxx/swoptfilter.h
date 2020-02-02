/***************************************************************************
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

#ifndef SWORDXX_SWOPTFILTER_H
#define SWORDXX_SWOPTFILTER_H

#include "swfilter.h"

#include <string>
#include <vector>


namespace swordxx {

  /** Base class for all option filters.
  */
class SWDLLEXPORT SWOptionFilter : public virtual SWFilter {

public: /* Methods: */

    SWOptionFilter(std::string oName,
                   std::string oTip,
                   std::vector<std::string> oValues);
    virtual ~SWOptionFilter();

    /** gets the name of the option of this filter
     * @return option name
     */
    std::string const & getOptionName() const noexcept { return optName; }

    /** gets a short explanation of the option of this filter;
     * it could be presented to the user in frontend programs
     * @return option tip/explanation
     */
    std::string const & getOptionTip() const noexcept { return optTip; }

    /** returns a list of the possible option values
     *
     * @return list of option values
     */
    std::vector<std::string> const & getOptionValues() const noexcept
    { return optValues; }

    /** @return The value of the current option.
    */
    std::string const & getSelectedOptionValue() const noexcept;

    std::size_t getSelectedOptionValueIndex() const noexcept
    { return selectedValueIndex; }

    /** sets the value of the option of this filter,
     * e.g maybe a strong's filter might be set to "On" / "Off" -
     * that would mean to show or not to show the strongs in the text,
     * see also getOptionValues()
     * @param ival the new option value
     */
    void setOptionValue(std::string_view value);

private: /* Fields: */

    std::string optName;
    std::string optTip;
    const std::vector<std::string> optValues;
    std::size_t selectedValueIndex;

};

} /* namespace swordxx */

#endif /* SWORDXX_SWOPTFILTER_H */
