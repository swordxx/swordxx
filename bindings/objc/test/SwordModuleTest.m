//
//  SwordModuleTest.m
//  ObjCSword
//
//  Created by Manfred Bergmann on 14.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import "SwordModuleTest.h"
#import <ObjCSword/ObjCSword.h>

@class SwordModule, SwordManager;

@interface SwordModuleTest : XCTestCase {
    SwordManager *mgr;
    SwordModule *mod;
}

@end

@implementation SwordModuleTest

- (void)setUp {
    [Configuration configWithImpl:[[OSXConfiguration alloc] init]];

    [[FilterProviderFactory providerFactory] initWithImpl:[[DefaultFilterProvider alloc] init]];
    
    mgr = [SwordManager managerWithPath:[[Configuration config] defaultModulePath]];
    mod = [mgr moduleWithName:@"GerNeUe"];
}

- (void)testModuleIntroduction {
    SwordBible *bible = (SwordBible *)[mgr moduleWithName:@"KJV"];

    NSString *modIntro = [bible moduleIntroduction];
    NSLog(@"mod intro: %@", modIntro);
    XCTAssertNotNil(modIntro);
    XCTAssertEqual(@"Im Anfang schuf Gott Himmel und Erde.", modIntro);
}

- (void)testFirstBookATIntro {
    SwordBible *bible = (SwordBible *)[mgr moduleWithName:@"KJV"];
    
    SwordBibleBook *book = [bible bookList][0];
    NSString *intro = [bible bookIntroductionFor:book];
    NSLog(@"testament: '%i', book '%@' intro: %@", [book testament], [book name], intro);
    XCTAssertNotNil(intro);
    XCTAssertTrue([intro hasPrefix:@" <!P><br />Das erste Buch der Bibel wird auch Genesis"]);
}

- (void)testFirstBookATIntroGer {
    SwordBible *bible = (SwordBible *)[mgr moduleWithName:@"GerNeUe"];

    SwordBibleBook *book = [bible bookList][0];
    NSString *intro = [bible bookIntroductionFor:book];
    NSLog(@"testament: '%i', book '%@' intro: %@", [book testament], [book name], intro);
    XCTAssertNotNil(intro);
    XCTAssertTrue([intro hasPrefix:@" <!P><br />Das erste Buch der Bibel wird auch Genesis"]);
}

- (void)testFirstBookNTIntro {
    SwordBible *bible = (SwordBible *)[mgr moduleWithName:@"KJV"];

    SwordBibleBook *book = [bible bookWithNamePrefix:@"Mat"];
    NSString *intro = [bible bookIntroductionFor:book];
    NSLog(@"testament: '%i', book '%@' intro: %@", [book testament], [book name], intro);
    XCTAssertNotNil(intro);
    XCTAssertTrue([intro hasPrefix:@" <!P><br />Um die Zeit der Apostelversammlung herum"]);
}

- (void)testFirstBookNTIntroGer {
    SwordBible *bible = (SwordBible *)[mgr moduleWithName:@"GerNeUe"];

    SwordBibleBook *book = [bible bookWithNamePrefix:@"Mat"];
    NSString *intro = [bible bookIntroductionFor:book];
    NSLog(@"testament: '%i', book '%@' intro: %@", [book testament], [book name], intro);
    XCTAssertNotNil(intro);
    XCTAssertTrue([intro hasPrefix:@" <!P><br />Um die Zeit der Apostelversammlung herum"]);
}

- (void)testPreverseHeading {
    SwordBible *bible = (SwordBible *)[mgr moduleWithName:@"GerNeUe"];

    [mgr setGlobalOption:SW_OPTION_HEADINGS value:SW_ON];
    SwordBibleTextEntry *text = (SwordBibleTextEntry *) [bible textEntryForKeyString:@"Numbers 1:47" textType:TextTypeRendered];
    NSLog(@"Preverse text: %@", [text preVerseHeading]);
    XCTAssertTrue([[text preVerseHeading] length] > 0);
    XCTAssertTrue([[text preVerseHeading] isEqualToString:@"<title>Die Sonderstellung der Leviten</title>"]);
    [mgr setGlobalOption:SW_OPTION_HEADINGS value:SW_OFF];
}

- (void)testPreverseHeading2 {
    SwordBible *bible = (SwordBible *)[mgr moduleWithName:@"GerNeUe"];

    [mgr setGlobalOption:SW_OPTION_HEADINGS value:SW_ON];
    SwordBibleTextEntry *text = (SwordBibleTextEntry *) [bible textEntryForKeyString:@"Numbers 4:21" textType:TextTypeRendered];
    NSLog(@"Preverse text: %@", [text preVerseHeading]);
    XCTAssertTrue([[text preVerseHeading] length] > 0);
    XCTAssertTrue([[text preVerseHeading] isEqualToString:@"<title>Die Sonderstellung der Leviten</title>"]);
    [mgr setGlobalOption:SW_OPTION_HEADINGS value:SW_OFF];
}

- (void)testLoopRenderedVerses {
    SwordBible *bible = (SwordBible *)[mgr moduleWithName:@"GerSch"];
    XCTAssertNotNil(bible, @"Module is nil");

    NSArray *verses = [bible renderedTextEntriesForRef:@"Gen"];
    XCTAssertNotNil(verses, @"");
    XCTAssertTrue([bible numberOfVerseKeysForReference:@"Gen"] == [verses count], @"");    
}

- (void)testRenderedVerseText {
    SwordBible *bible = (SwordBible *)[mgr moduleWithName:@"GerSch"];
    XCTAssertNotNil(bible, @"Module is nil");
    
    SwordModuleTextEntry *text = [bible textEntryForKeyString:@"gen1.1" textType:TextTypeRendered];
    XCTAssertNotNil(text, @"");
    XCTAssertTrue([[text text] length] > 0, @"");
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
            [vk setVerse:(int)lowVerse];
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
    XCTAssertTrue([vk chapter] == 1);
    XCTAssertTrue([vk book] == 1);
    XCTAssertTrue([vk verse] == 1);

    [vk setVerse:[vk verse] + 3];
    NSLog(@"verse + 3: %@", [vk keyText]);
    XCTAssertTrue([vk chapter] == 1);
    XCTAssertTrue([vk book] == 1);
    XCTAssertTrue([vk verse] == 4);
}

@end
