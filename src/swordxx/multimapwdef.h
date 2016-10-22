/******************************************************************************
 *
 *  multimapwdef.h -    Implementation of multimapwithdefault
 *
 * $Id$
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef MULTIMAPWDEF_H
#define MULTIMAPWDEF_H

#include <algorithm>
#include <map>

namespace swordxx {

// multmap that still lets you use [] to reference FIRST
// entry of a key if multiples exist
template <typename Key, typename T, typename ... Args>
class multimapwithdefault: public std::multimap<Key, T, Args...> {

public: /* Methods: */

    T & getWithDefault(Key const & key, T const & defaultValue) {
        auto const it(this->find(key));
        return (it != this->end())
               ? it->second
               : this->emplace(key, defaultValue)->second;
    }

    T & operator[](Key const & key) { return getWithDefault(key, T()); }

    bool has(Key const & key, T const & value) const noexcept {
        auto const end(this->upper_bound(key));
        return std::find_if(this->lower_bound(key),
                            end,
                            [&value](auto const & vp)
                            { return vp.second == value; }) != end;
    }
};

} /* namespace swordxx */
#endif
