//
//  SwordBibleBook.h
//  MacSword2
//
//  Created by Manfred Bergmann on 18.02.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifdef __cplusplus
#include <versificationmgr.h>
#include <versekey.h>
#include <localemgr.h>
class sword::VersificationMgr::Book;
#endif

@interface SwordBibleBook : NSObject {
#ifdef __cplusplus
    sword::VersificationMgr::Book *swBook;
#endif
    
    NSString *localizedName;
    int number;
    char numberInTestament;
    char testament;
    NSArray *chapters;
}

@property (readwrite) int number;
@property (readwrite) char numberInTestament;
@property (readwrite) char testament;
@property (retain, readwrite) NSString *localizedName;
@property (retain, readwrite) NSArray *chapters;

#ifdef __cplusplus
- (id)initWithBook:(sword::VersificationMgr::Book *)aBook;
- (sword::VersificationMgr::Book *)book;
#endif

- (NSString *)name;
- (NSString *)osisName;
- (int)numberOfChapters;
- (int)numberOfVersesForChapter:(int)chapter;
/**
 get book index for verseKey
 that is: book number + testament * 100
 */
- (int)generatedIndex;

@end
