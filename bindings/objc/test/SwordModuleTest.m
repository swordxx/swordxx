//
//  SwordModuleTest.m
//  ObjCSword
//
//  Created by Manfred Bergmann on 14.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import <ObjCSword/ObjCSword.h>
#import "SwordModuleTest.h"

@implementation SwordModuleTest

- (void)setUp {
    [Configuration configWithImpl:[[[OSXConfiguration alloc] init] autorelease]];
    mgr = [SwordManager managerWithPath:[[Configuration config] defaultModulePath]];
    mod = [mgr moduleWithName:@"GerNeUe"];
}

- (void)testLoopRenderedVerses {
    SwordBible *bible = (SwordBible *)[mgr moduleWithName:@"GerSch"];
    STAssertNotNil(bible, @"Module is nil");

    NSArray *verses = [bible renderedTextEntriesForRef:@"Gen"];
    STAssertNotNil(verses, @"");
    STAssertTrue([bible numberOfVerseKeysForReference:@"Gen"] == [verses count], @"");    
}

- (void)testRenderedVerseText {
    SwordBible *bible = (SwordBible *)[mgr moduleWithName:@"GerSch"];
    STAssertNotNil(bible, @"Module is nil");
    
    SwordModuleTextEntry *text = [bible textEntryForKeyString:@"gen1.1" textType:TextTypeRendered];
    STAssertNotNil(text, @"");
    STAssertTrue([[text text] length] > 0, @"");
}

- (void)testCloneModule {
    SwordBible *bible = (SwordBible *)[mgr moduleWithName:@"GerSch"];
    STAssertNotNil(bible, @"Module is nil");

    /*
    SwordBible *cloned = (SwordBible *)[bible clone];
    STAssertNotNil(cloned, @"Module is nil");    
     */
}

- (void)testLoopWithModulePos {
    SwordListKey *lk = [SwordListKey listKeyWithRef:@"gen" v11n:[mod versification]];
    [lk setPersist:YES];
    [mod setSwordKey:lk];
    NSString *ref = nil;
    NSString *rendered = nil;
    while(![mod error]) {
        ref = [lk keyText];
        rendered = [mod renderedText];
        [mod incKeyPosition];
    }
}

- (void)testLoopWithModulePosNoPersist {
    SwordListKey *lk = [SwordListKey listKeyWithRef:@"gen" v11n:[mod versification]];    
    [lk setPersist:NO];
    [lk setPosition:SWPOS_TOP];
    NSString *ref = nil;
    NSString *rendered = nil;
    while(![lk error]) {
        ref = [lk keyText];
        [mod setSwordKey:lk];
        rendered = [mod renderedText];
        //NSLog(@"%@:%@", ref, rendered);
        [lk increment];
    }
}

- (void)testLoopWithModulePosWithHeadings {
    SwordListKey *lk = [SwordListKey listKeyWithRef:@"gen" headings:YES v11n:[mod versification]];
    [lk setPersist:YES];
    [mod setSwordKey:lk];
    NSString *ref = nil;
    NSString *rendered = nil;
    while(![mod error]) {
        ref = [lk keyText];
        rendered = [mod renderedText];
        [mod incKeyPosition];
    }
}

- (void)testLoopWithModulePosWithDiverseReference {
    SwordListKey *lk = [SwordListKey listKeyWithRef:@"gen 1:1;4:5-8" v11n:[mod versification]];
    [lk setPersist:YES];
    [mod setSwordKey:lk];
    NSString *ref = nil;
    NSString *rendered = nil;
    while(![mod error]) {
        ref = [lk keyText];
        rendered = [mod renderedText];
        NSLog(@"%@:%@", ref, rendered);
        [mod incKeyPosition];
    }
}

- (void)testLoopWithModulePosNoPersistWithDiverseReference {
    SwordListKey *lk = [SwordListKey listKeyWithRef:@"gen 1:1;4:5-8" v11n:[mod versification]];
    [lk setPersist:NO];
    [lk setPosition:SWPOS_TOP];
    NSString *ref = nil;
    NSString *rendered = nil;
    while(![lk error]) {
        ref = [lk keyText];
        [mod setSwordKey:lk];
        rendered = [mod renderedText];
        NSLog(@"%@:%@", ref, rendered);
        [lk increment];
    }
}

- (void)testLoopWithModulePosWithDiverseReferenceAndContext {
    int context = 1;
    SwordVerseKey *vk = [SwordVerseKey verseKeyWithVersification:[mod versification]];
    [vk setPersist:YES];
    SwordListKey *lk = [SwordListKey listKeyWithRef:@"gen 1:1;4:5;8:4;10:2-5" v11n:[mod versification]];
    [lk setPersist:YES];
    [mod setSwordKey:lk];
    NSString *ref = nil;
    NSString *rendered = nil;
    while(![mod error]) {
        if(context > 0) {
            [vk setKeyText:[lk keyText]];
            long lowVerse = [vk verse] - context;
            long highVerse = lowVerse + (context * 2);
            [vk setVerse:lowVerse];
            [mod setSwordKey:vk];
            for(;lowVerse <= highVerse;lowVerse++) {
                ref = [vk keyText];
                rendered = [mod renderedText];                
                NSLog(@"%@:%@", ref, rendered);
                [mod incKeyPosition];
            }
            // set back list key
            [mod setSwordKey:lk];
            [mod incKeyPosition];
        } else {
            ref = [lk keyText];
            rendered = [mod renderedText];
            NSLog(@"%@:%@", ref, rendered);
            [mod incKeyPosition];            
        }
    }
}

- (void)testStrippedTextForRef {
    SwordVerseKey *vk = [SwordVerseKey verseKeyWithRef:@"1Mo 1:2"];
    NSLog(@"start position: %@", [vk keyText]);
    [vk decrement];
    NSLog(@"decrement position: %@", [vk keyText]);
    [vk setVerse:[vk verse] + 3];
    NSLog(@"verse + 3: %@", [vk keyText]);
}

@end
