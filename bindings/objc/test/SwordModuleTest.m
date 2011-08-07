//
//  SwordModuleTest.m
//  ObjCSword
//
//  Created by Manfred Bergmann on 14.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import "SwordModuleTest.h"
#ifdef TARGET_IPHONE_SIMULATOR
#import "SwordManager.h"
#import "Configuration.h"
#import "OSXConfiguration.h"
//#import "iOSConfiguration.h"
#import "SwordModule.h"
#import "SwordModuleTextEntry.h"
#import "VerseEnumerator.h"
#import "SwordListKey.h"
#import "SwordVerseKey.h"
#else
#import "ObjCSword/ObjCSword.h"
#import "ObjCSword/SwordModule+Index.h"
#endif

@implementation SwordModuleTest

- (void)setUp {
#ifdef TARGET_IPHONE_SIMULATOR
//    [[Configuration config] setClass:[iOSConfiguration class]];
#else
#endif
    [Configuration configWithImpl:[[OSXConfiguration alloc] init]];
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
    [mod setKey:lk];
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
        [mod setKey:lk];
        rendered = [mod renderedText];
        //NSLog(@"%@:%@", ref, rendered);
        [lk increment];
    }
}

- (void)testLoopWithModulePosWithHeadings {
    SwordListKey *lk = [SwordListKey listKeyWithRef:@"gen" headings:YES v11n:[mod versification]];
    [lk setPersist:YES];
    [mod setKey:lk];
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
    [mod setKey:lk];
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
        [mod setKey:lk];
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
    [mod setKey:lk];
    NSString *ref = nil;
    NSString *rendered = nil;
    while(![mod error]) {
        if(context > 0) {
            [vk setKeyText:[lk keyText]];
            long lowVerse = [vk verse] - context;
            long highVerse = lowVerse + (context * 2);
            [vk setVerse:lowVerse];
            [mod setKey:vk];
            for(;lowVerse <= highVerse;lowVerse++) {
                ref = [vk keyText];
                rendered = [mod renderedText];                
                NSLog(@"%@:%@", ref, rendered);
                [mod incKeyPosition];
            }
            // set back list key
            [mod setKey:lk];
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
    NSLog(@"start position: %s", [vk keyText]);
    [vk decrement];
    NSLog(@"decrement position: %s", [vk keyText]);
    [vk setVerse:[vk verse] + 3];
    NSLog(@"verse + 3: %s", [vk keyText]);
}

/*
 #ifdef __cplusplus
 #include <swtext.h>
 #include <versekey.h>
 #include <regex.h>
 #include <swmodule.h>
 class sword::SWModule;
 #include <iostream>
 #include <versekey.h>
 #include <rawtext.h>
 #include <rawcom.h>
 #include <echomod.h>
 #include <stdlib.h>
 using namespace sword;
 #endif
 
 - (void)testHeadings {
 mod = [[SwordManager defaultManager] moduleWithName:@"KJV"];
 STAssertNotNil(mod, @"No Mod");
 
 // enable headings
 [[SwordManager defaultManager] setGlobalOption:SW_OPTION_HEADINGS value:SW_ON];
 [[SwordManager defaultManager] setGlobalOption:SW_OPTION_STRONGS value:SW_ON];
 [[SwordManager defaultManager] setGlobalOption:SW_OPTION_FOOTNOTES value:SW_ON];
 
 SWModule *target;    
 target = [mod swModule];    
 target->setKey("gen 1:1");    
 target->RenderText();		// force an entry lookup to resolve key to something in the index
 
 std::cout << "==Raw=Entry===============\n";
 std::cout << target->getKeyText() << ":\n";
 std::cout << target->getRawEntry();
 std::cout << "\n";
 std::cout << "==Render=Entry============\n";
 std::cout << target->RenderText();
 std::cout << "\n";
 std::cout << "==========================\n";
 std::cout << "Entry Attributes:\n\n";
 AttributeTypeList::iterator i1;
 AttributeList::iterator i2;
 AttributeValue::iterator i3;
 for (i1 = target->getEntryAttributes().begin(); i1 != target->getEntryAttributes().end(); i1++) {
 std::cout << "[ " << i1->first << " ]\n";
 for (i2 = i1->second.begin(); i2 != i1->second.end(); i2++) {
 std::cout << "\t[ " << i2->first << " ]\n";
 for (i3 = i2->second.begin(); i3 != i2->second.end(); i3++) {
 std::cout << "\t\t" << i3->first << " = " << i3->second << "\n";
 }
 }
 }
 std::cout << std::endl;
 }
 */

@end
