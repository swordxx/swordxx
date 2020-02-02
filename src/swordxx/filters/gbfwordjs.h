/***************************************************************************
 *
 * Copyright 2005-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_GBFWORDSJS_H
#define SWORDXX_GBFWORDSJS_H

#include "../offonoptionfilter.h"


namespace swordxx {

class SWMgr;
  /** This Filter shows/hides strong's numbers in a GBF text
  */
class SWDLLEXPORT GBFWordJS: public OffOnOptionFilter {
     SWModule * m_defaultGreekLex;
     SWModule * m_defaultHebLex;
     SWModule * m_defaultGreekParse;
     SWModule * m_defaultHebParse;
     SWMgr * m_mgr;

public:
    GBFWordJS();
    ~GBFWordJS() override;

    char processText(std::string & text,
                         SWKey const * key = nullptr,
                         SWModule const * module = nullptr) override;

    void setDefaultModules(SWModule * defaultGreekLex = nullptr,
                           SWModule * defaultHebLex = nullptr,
                           SWModule * defaultGreekParse = nullptr,
                           SWModule * defaultHebParse = nullptr)
    {
        this->m_defaultGreekLex   = defaultGreekLex;
        this->m_defaultHebLex     = defaultHebLex;
        this->m_defaultGreekParse = defaultGreekParse;
        this->m_defaultHebParse   = defaultHebParse;
    }
    void setMgr(SWMgr *mgr) { this->m_mgr = mgr; }
};

} /* namespace swordxx */

#endif /* SWORDXX_GBFWORDSJS_H */
