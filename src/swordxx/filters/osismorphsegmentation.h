/******************************************************************************
 *
 * Copyright 2006-2013 CrossWire Bible Society (http://www.crosswire.org)
 * CrossWire Bible Society
 * P. O. Box 2528
 * Tempe, AZ  85280-2528
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

#ifndef SWORDXX_OSISMORPHSEGMENTATION_H
#define SWORDXX_OSISMORPHSEGMENTATION_H

#include "../offonoptionfilter.h"


namespace swordxx {

/*  This filters toggles splitting of morphemes
 *  (for morpheme segmented Hebrew in the WLC)
 */

class SWDLLEXPORT OSISMorphSegmentation: public OffOnOptionFilter {

public:
    OSISMorphSegmentation();
    ~OSISMorphSegmentation() override;

    char processText(std::string & text,
                         SWKey const * key = nullptr,
                         SWModule const * module = nullptr) override;

};

} /* namespace swordxx */

#endif /* SWORDXX_OSISMORPHSEGMENTATION_H */
