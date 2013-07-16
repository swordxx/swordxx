//
// Created by mbergmann on 18.12.12.
//
// To change the template use AppCode | Preferences | File Templates.
//


#import "SwordFilter.h"
#import "osishtmlhref.h"
#import "osisplain.h"
#import "osisxhtml.h"
#import "thmlhtmlhref.h"
#import "thmlplain.h"
#import "gbfhtmlhref.h"
#import "gbfplain.h"
#import "teihtmlhref.h"
#import "teixhtml.h"
#import "teiplain.h"

@implementation SwordFilter {
    sword::SWFilter *swFilter;
}

- (id)initWithSWFilter:(sword::SWFilter *)swFilter1 {
    self = [super init];
    if (self) {
        swFilter = swFilter1;
    }

    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (sword::SWFilter *)swFilter {
    return swFilter;
}

@end

@implementation SwordOsisHtmlRefFilter
+ (SwordOsisHtmlRefFilter *)filter {
    return [[[SwordOsisHtmlRefFilter alloc] init] autorelease];
}

- (id)init {
    return [super initWithSWFilter:new sword::OSISHTMLHREF()];
}
@end

@implementation SwordOsisXHtmlFilter
+ (SwordOsisXHtmlFilter *)filter {
    return [[[SwordOsisXHtmlFilter alloc] init] autorelease];
}

- (id)init {
    return [super initWithSWFilter:new sword::OSISXHTML()];
}
@end

@implementation SwordOsisPlainFilter
+ (SwordOsisPlainFilter *)filter {
    return [[[SwordOsisPlainFilter alloc] init] autorelease];
}

- (id)init {
    return [super initWithSWFilter:new sword::OSISPlain()];
}
@end

@implementation SwordThmlHtmlFilter
+ (SwordThmlHtmlFilter *)filter {
    return [[[SwordThmlHtmlFilter alloc] init] autorelease];
}

- (id)init {
    return [super initWithSWFilter:new sword::ThMLHTMLHREF()];
}
@end

@implementation SwordThmlPlainFilter
+ (SwordThmlPlainFilter *)filter {
    return [[[SwordThmlPlainFilter alloc] init] autorelease];
}

- (id)init {
    return [super initWithSWFilter:new sword::ThMLPlain()];
}
@end

@implementation SwordGbfHtmlFilter
+ (SwordGbfHtmlFilter *)filter {
    return [[[SwordGbfHtmlFilter alloc] init] autorelease];
}

- (id)init {
    return [super initWithSWFilter:new sword::ThMLHTMLHREF()];
}
@end

@implementation SwordGbfPlainFilter
+ (SwordGbfPlainFilter *)filter {
    return [[[SwordGbfPlainFilter alloc] init] autorelease];
}

- (id)init {
    return [super initWithSWFilter:new sword::ThMLPlain()];
}
@end

@implementation SwordTeiHtmlFilter
+ (SwordTeiHtmlFilter *)filter {
    return [[[SwordTeiHtmlFilter alloc] init] autorelease];
}

- (id)init {
    return [super initWithSWFilter:new sword::TEIHTMLHREF()];
}
@end

@implementation SwordTeiXHtmlFilter
+ (SwordTeiXHtmlFilter *)filter {
    return [[[SwordTeiXHtmlFilter alloc] init] autorelease];
}

- (id)init {
    return [super initWithSWFilter:new sword::TEIXHTML()];
}
@end

@implementation SwordTeiPlainFilter
+ (SwordTeiPlainFilter *)filter {
    return [[[SwordTeiPlainFilter alloc] init] autorelease];
}

- (id)init {
    return [super initWithSWFilter:new sword::TEIPlain()];
}
@end

