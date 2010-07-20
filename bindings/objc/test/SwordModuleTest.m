//
//  SwordModuleTest.m
//  ObjCSword
//
//  Created by Manfred Bergmann on 14.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import "SwordModuleTest.h"
#import "ObjCSword/Configuration.h"
#import "ObjCSword/OSXConfiguration.h"
#import "ObjCSword/SwordManager.h"
#import "ObjCSword/SwordBible.h"
#import "ObjCSword/SwordModule+Index.h"

@implementation SwordModuleTest

- (void)setUp {
    [[Configuration config] setClass:[OSXConfiguration class]];
}

- (void)testCreateSearchIndex {
    SwordModule *sm = [[SwordManager defaultManager] moduleWithName:@"GerSch"];
    STAssertNotNil(sm, @"Module is nil");
    
    NSLog(@"creating clucene search index...");
    //[sm createSearchIndex];
    NSLog(@"creating clucene search index...done");
}

- (void)testLoopRenderedVerses {
    SwordBible *bible = (SwordBible *)[[SwordManager defaultManager] moduleWithName:@"GerSch"];
    STAssertNotNil(bible, @"Module is nil");

    NSArray *verses = [bible renderedTextEntriesForRef:@"Gen"];
    STAssertNotNil(verses, @"");
    STAssertTrue([bible numberOfVerseKeysForReference:@"Gen"] == [verses count], @"");    
}

@end
