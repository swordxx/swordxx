//
// Created by mbergmann on 18.12.12.
//
// To change the template use AppCode | Preferences | File Templates.
//


#import <Foundation/Foundation.h>

#ifdef __cplusplus
#include <swmgr.h>		// C++ Sword API
#endif

@protocol FilterProvider

#ifdef __cplusplus
- (sword::SWFilter *)newOsisRenderFilter;
- (sword::SWFilter *)newOsisPlainFilter;
- (sword::SWFilter *)newGbfRenderFilter;
- (sword::SWFilter *)newGbfPlainFilter;
- (sword::SWFilter *)newThmlRenderFilter;
- (sword::SWFilter *)newThmlPlainFilter;
- (sword::SWFilter *)newTeiRenderFilter;
- (sword::SWFilter *)newTeiPlainFilter;
- (sword::SWFilter *)newHtmlPlainFilter;
#endif

@end

@interface DefaultFilterProvider : NSObject <FilterProvider>

#ifdef __cplusplus
- (sword::SWFilter *)newOsisRenderFilter;
- (sword::SWFilter *)newOsisPlainFilter;
- (sword::SWFilter *)newGbfRenderFilter;
- (sword::SWFilter *)newGbfPlainFilter;
- (sword::SWFilter *)newThmlRenderFilter;
- (sword::SWFilter *)newThmlPlainFilter;
- (sword::SWFilter *)newTeiRenderFilter;
- (sword::SWFilter *)newTeiPlainFilter;
- (sword::SWFilter *)newHtmlPlainFilter;
#endif

@end