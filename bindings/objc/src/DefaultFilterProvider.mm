//
// Created by mbergmann on 18.12.12.
//
//


#import "DefaultFilterProvider.h"

#include "gbfplain.h"
#include "thmlplain.h"
#include "osisplain.h"
#include "teiplain.h"
#include "markupfiltmgr.h"
#include "osishtmlhref.h"
#include "thmlhtmlhref.h"
#include "gbfhtmlhref.h"
#include "teihtmlhref.h"
#include "plainhtml.h"


@implementation DefaultFilterProvider

- (sword::SWFilter *)newOsisRenderFilter {
    return new sword::OSISHTMLHREF();
}

- (sword::SWFilter *)newOsisPlainFilter {
    return new sword::OSISPlain();
}

- (sword::SWFilter *)newGbfRenderFilter {
    return new sword::GBFHTMLHREF();
}

- (sword::SWFilter *)newGbfPlainFilter {
    return new sword::GBFPlain();
}

- (sword::SWFilter *)newThmlRenderFilter {
    return new sword::ThMLHTMLHREF();
}

- (sword::SWFilter *)newThmlPlainFilter {
    return new sword::ThMLPlain();
}

- (sword::SWFilter *)newTeiRenderFilter {
    return new sword::TEIHTMLHREF();
}

- (sword::SWFilter *)newTeiPlainFilter {
    return new sword::TEIPlain();
}

- (sword::SWFilter *)newHtmlPlainFilter {
    return new sword::PLAINHTML();
}

@end