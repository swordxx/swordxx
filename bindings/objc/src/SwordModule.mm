/*	SwordModule.mm - Sword API wrapper for Modules.

	Copyright 2008 Manfred Bergmann
	Based on code by Will Thimbleby

	This program is free software; you can redistribute it and/or modify it under the terms of the
	GNU General Public License as published by the Free Software Foundation version 2.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
	even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	General Public License for more details. (http://www.gnu.org/licenses/gpl.html)
*/

#import "SwordModule.h"
#import "SwordManager.h"
#import "SwordModuleTextEntry.h"
#import "SwordVerseKey.h"

#import "rtfhtml.h"
#import "swtext.h"
#import "versekey.h"
#import "regex.h"

@interface SwordModule ()
@property(readwrite, retain) NSMutableDictionary *configEntries;
- (void)mainInit;
@end


@implementation SwordModule

// -------------- property implementations -----------------
@synthesize configEntries;
@synthesize type;
@synthesize status;
@synthesize moduleLock;
@synthesize indexLock;
@synthesize swManager;
@synthesize name;

/**
 \brief maps type string to ModuleType enum
 @param[in] typeStr type String as in -moduleType(SwordModule)
 @return type according to ModuleType enum
 */
+ (ModuleType)moduleTypeForModuleTypeString:(NSString *)typeStr {
     ModuleType ret = bible;
    
    if(typeStr == nil) {
        ALog(@"have a nil typeStr!");
        return ret;
    }
    
    if([typeStr isEqualToString:SWMOD_CATEGORY_BIBLES]) {
        ret = bible;
    } else if([typeStr isEqualToString:SWMOD_CATEGORY_COMMENTARIES]) {
        ret = commentary;
    } else if([typeStr isEqualToString:SWMOD_CATEGORY_DICTIONARIES]) {
        ret = dictionary;
    } else if([typeStr isEqualToString:SWMOD_CATEGORY_GENBOOKS]) {
        ret = genbook;
    }
    
    return ret;
}

- (void)mainInit {
    self.type = [SwordModule moduleTypeForModuleTypeString:[self typeString]];
    self.moduleLock = [[NSRecursiveLock alloc] init];
    self.indexLock = [[NSLock alloc] init];
    self.configEntries = [NSMutableDictionary dictionary];
    self.name = [NSString stringWithCString:swModule->Name() encoding:NSUTF8StringEncoding];
}

- (id)initWithName:(NSString *)aName swordManager:(SwordManager *)aManager {
    self = [super init];
	if(self) {
		swModule = [aManager getSWModuleWithName:aName];
        self.swManager = aManager;
        
        [self mainInit];
	}
	
	return self;
}

- (id)initWithSWModule:(sword::SWModule *)aModule {
    return [self initWithSWModule:aModule swordManager:nil];
}

- (id)initWithSWModule:(sword::SWModule *)aModule swordManager:(SwordManager *)aManager {    
    self = [super init];
    if(self) {
        swModule = aModule;
        self.swManager = aManager;
        
        [self mainInit];
    }
    
    return self;
}

- (void)finalize {    
	[super finalize];
}

- (void)dealloc {
    [self setConfigEntries:nil];
    [self setSwManager:nil];
    [self setModuleLock:nil];
    [self setIndexLock:nil];
    [self setName:nil];

    [super dealloc];
}

- (void)aquireModuleLock {
    [moduleLock lock];
}

- (void)releaseModuleLock {
    [moduleLock unlock];
}

#pragma mark - convenience methods

/** abstract method. subclasses should handle implementation. */
- (NSAttributedString *)fullAboutText {
    return [[NSMutableAttributedString alloc] init];
}

- (NSInteger)error {
    return swModule->Error();
}

- (NSString *)descr {
    NSString *str = [NSString stringWithCString:swModule->Description() encoding:NSUTF8StringEncoding];
    if(!str) {
        str = [NSString stringWithCString:swModule->Description() encoding:NSISOLatin1StringEncoding];
    }
    return str;
}

- (NSString *)lang {
    NSString *str = [NSString stringWithCString:swModule->Lang() encoding:NSUTF8StringEncoding];
    if(!str) {
        str = [NSString stringWithCString:swModule->Lang() encoding:NSISOLatin1StringEncoding];
    }
    return str;
}

- (NSString *)typeString {
    NSString *str = [NSString stringWithCString:swModule->Type() encoding:NSUTF8StringEncoding];
    if(!str) {
        str = [NSString stringWithCString:swModule->Type() encoding:NSISOLatin1StringEncoding];
    }
    return str;
}

- (NSString *)cipherKey {
    NSString *cipherKey = [configEntries objectForKey:SWMOD_CONFENTRY_CIPHERKEY];
    if(cipherKey == nil) {
        cipherKey = [self configFileEntryForConfigKey:SWMOD_CONFENTRY_CIPHERKEY];
        if(cipherKey != nil) {
            [configEntries setObject:cipherKey forKey:SWMOD_CONFENTRY_CIPHERKEY];
        }
    }
    
    return cipherKey;
}

- (NSString *)version {
    NSString *version = [configEntries objectForKey:SWMOD_CONFENTRY_VERSION];
    if(version == nil) {
        version = [self configFileEntryForConfigKey:SWMOD_CONFENTRY_VERSION];
        if(version != nil) {
            [configEntries setObject:version forKey:SWMOD_CONFENTRY_VERSION];
        }
    }
    
    return version;
}

- (NSString *)minVersion {
    NSString *minVersion = [configEntries objectForKey:SWMOD_CONFENTRY_MINVERSION];
    if(minVersion == nil) {
        minVersion = [self configFileEntryForConfigKey:SWMOD_CONFENTRY_MINVERSION];
        if(minVersion != nil) {
            [configEntries setObject:minVersion forKey:SWMOD_CONFENTRY_MINVERSION];
        }
    }
    
    return minVersion;
}

/** this might be RTF string  but the return value will be converted to UTF8 */
- (NSString *)aboutText {
    NSMutableString *aboutText = [configEntries objectForKey:SWMOD_CONFENTRY_ABOUT];
    if(aboutText == nil) {
        aboutText = [NSMutableString stringWithString:[self configFileEntryForConfigKey:SWMOD_CONFENTRY_ABOUT]];
        if(aboutText != nil) {
			//search & replace the RTF markup:
			// "\\qc"		- for centering							--->>>  ignore these
			// "\\pard"		- for resetting paragraph attributes	--->>>  ignore these
			// "\\par"		- for paragraph breaks					--->>>  honour these
			// "\\u{num}?"	- for unicode characters				--->>>  honour these
			[aboutText replaceOccurrencesOfString:@"\\qc" withString:@"" options:0 range:NSMakeRange(0, [aboutText length])];
			[aboutText replaceOccurrencesOfString:@"\\pard" withString:@"" options:0 range:NSMakeRange(0, [aboutText length])];
			[aboutText replaceOccurrencesOfString:@"\\par" withString:@"\n" options:0 range:NSMakeRange(0, [aboutText length])];
            
			NSMutableString *retStr = [[@"" mutableCopy] autorelease];
			for(NSUInteger i=0; i<[aboutText length]; i++) {
				unichar c = [aboutText characterAtIndex:i];
                
				if(c == '\\' && ((i+1) < [aboutText length])) {
					unichar d = [aboutText characterAtIndex:(i+1)];
					if (d == 'u') {
						//we have an unicode character!
						@try {
							NSUInteger unicodeChar = 0;
							NSMutableString *unicodeCharString = [[@"" mutableCopy] autorelease];
							int j = 0;
							BOOL negative = NO;
							if ([aboutText characterAtIndex:(i+2)] == '-') {
								//we have a negative unicode char
								negative = YES;
								j++;//skip past the '-'
							}
							while(isdigit([aboutText characterAtIndex:(i+2+j)])) {
								[unicodeCharString appendFormat:@"%C", [aboutText characterAtIndex:(i+2+j)]];
								j++;
							}
							unicodeChar = [unicodeCharString integerValue];
							if (negative) unicodeChar = 65536 - unicodeChar;
							i += j+2;
							[retStr appendFormat:@"%C", unicodeChar];
						}
						@catch (NSException * e) {
							[retStr appendFormat:@"%C", c];
						}
						//end dealing with the unicode character.
					} else {
						[retStr appendFormat:@"%C", c];
					}
				} else {
					[retStr appendFormat:@"%C", c];
				}
			}
			
			aboutText = retStr;
        } else {
            aboutText = [NSMutableString string];
        }
        [configEntries setObject:aboutText forKey:SWMOD_CONFENTRY_ABOUT];
    }
    
    return aboutText;    
}

/** versification scheme in config */
- (NSString *)versification {
    NSString *versification = [configEntries objectForKey:SWMOD_CONFENTRY_VERSIFICATION];
    if(versification == nil) {
        versification = [self configFileEntryForConfigKey:SWMOD_CONFENTRY_VERSIFICATION];
        if(versification != nil) {
            [configEntries setObject:versification forKey:SWMOD_CONFENTRY_VERSIFICATION];
        }
    }
    
    // if still nil, use KJV versification
    if(versification == nil) {
        versification = @"KJV";
        [configEntries setObject:versification forKey:SWMOD_CONFENTRY_VERSIFICATION];
    }
    
    return versification;
}

- (BOOL)isEditable {
    BOOL ret = NO;
    NSString *editable = [configEntries objectForKey:SWMOD_CONFENTRY_EDITABLE];
    if(editable == nil) {
        editable = [self configFileEntryForConfigKey:SWMOD_CONFENTRY_EDITABLE];
        if(editable != nil) {
            [configEntries setObject:editable forKey:SWMOD_CONFENTRY_EDITABLE];
        }
    }
    
    if(editable) {
        if([editable isEqualToString:@"YES"]) {
            ret = YES;
        }
    }
    
    return ret;
}

- (BOOL)isRTL {
    BOOL ret = NO;
    NSString *direction = [configEntries objectForKey:SWMOD_CONFENTRY_DIRECTION];
    if(direction == nil) {
        direction = [self configFileEntryForConfigKey:SWMOD_CONFENTRY_DIRECTION];
        if(direction != nil) {
            [configEntries setObject:direction forKey:SWMOD_CONFENTRY_DIRECTION];
        }
    }
    
    if(direction) {
        if([direction isEqualToString:SW_DIRECTION_RTL]) {
            ret = YES;
        }
    }
    
    return ret;    
}

- (BOOL)isUnicode {    
    return swModule->isUnicode();
}

- (BOOL)isEncrypted {
    BOOL encrypted = YES;
    if([self cipherKey] == nil) {
        encrypted = NO;
    }
    
    return encrypted;
}

/** is module locked/has cipherkey config entry but cipherkey entry is empty */
- (BOOL)isLocked {
    BOOL locked = NO;
    NSString *key = [self cipherKey];
    if(key != nil) {
        // check user defaults, that's where we store the entered keys
        NSDictionary *cipherKeys = [[NSUserDefaults standardUserDefaults] objectForKey:DefaultsModuleCipherKeysKey];
        if([key length] == 0 && [[cipherKeys allKeys] containsObject:[self name]] == NO) {
            locked = YES;
        }
    }
    
    return locked;
}

- (BOOL)unlock:(NSString *)unlockKey {
    
	if (![self isEncrypted]) {
		return NO;
    }
    
    NSMutableDictionary	*cipherKeys = [NSMutableDictionary dictionaryWithDictionary:
                                       [[NSUserDefaults standardUserDefaults] objectForKey:DefaultsModuleCipherKeysKey]];
    [cipherKeys setObject:unlockKey forKey:[self name]];
    [[NSUserDefaults standardUserDefaults] setObject:cipherKeys forKey:DefaultsModuleCipherKeysKey];
    
	[swManager setCipherKey:unlockKey forModuleNamed:[self name]];
    
	return YES;
}

- (id)attributeValueForParsedLinkData:(NSDictionary *)data {
    id ret = nil;
    
    NSString *passage = [data objectForKey:ATTRTYPE_PASSAGE];
    if(passage) {
        passage = [[passage stringByReplacingOccurrencesOfString:@"+" withString:@" "] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    } 
    NSString *attrType = [data objectForKey:ATTRTYPE_TYPE];
    if([attrType isEqualToString:@"n"]) {
        NSString *footnoteText = [self entryAttributeValueFootnoteOfType:attrType 
                                                              indexValue:[data objectForKey:ATTRTYPE_VALUE] 
                                                                  forKey:[SwordKey swordKeyWithRef:passage]];
        ret = [self strippedTextFromString:footnoteText];
    } else if([attrType isEqualToString:@"x"]) {
        NSString *refListString = [self entryAttributeValueFootnoteOfType:attrType
                                                               indexValue:[data objectForKey:ATTRTYPE_VALUE] 
                                                                   forKey:[SwordKey swordKeyWithRef:passage]];
        ret = [self strippedTextEntriesForRef:refListString];
    } else if([attrType isEqualToString:@"scriptRef"] || [attrType isEqualToString:@"scripRef"]) {
        NSString *key = [[[data objectForKey:ATTRTYPE_VALUE] stringByReplacingOccurrencesOfString:@"+" 
                                                                                       withString:@" "] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        ret = [self strippedTextEntriesForRef:key];
    }
    
    return ret;
}

- (NSString *)description {
    return [self name];
}

#pragma mark - SwordModuleAccess

/** 
 number of entries
 abstract method, should be overriden by subclasses
 */
- (long)entryCount {
    return 0;
}

- (NSArray *)strippedTextEntriesForRef:(NSString *)reference {
    NSArray *ret = nil;
    
    SwordModuleTextEntry *entry = [self textEntryForKey:[SwordKey swordKeyWithRef:reference] 
                                               textType:TextTypeStripped];
    if(entry) {
        ret = [NSArray arrayWithObject:entry];
    }
    
    return ret;    
}

- (NSArray *)renderedTextEntriesForRef:(NSString *)reference {
    NSArray *ret = nil;
    
    SwordModuleTextEntry *entry = [self textEntryForKey:[SwordKey swordKeyWithRef:reference] 
                                               textType:TextTypeRendered];
    if(entry) {
        ret = [NSArray arrayWithObject:entry];
    }
    
    return ret;
}

- (void)incKeyPosition {
    swModule->increment(1);
}

- (void)decKeyPosition {
    swModule->decrement(1);
}

/**
 subclasses need to implement this
 */
- (void)writeEntry:(SwordModuleTextEntry *)anEntry {
}

- (void)setKeyString:(NSString *)aKeyString {
    swModule->setKey([aKeyString UTF8String]);
}

- (void)setKey:(SwordKey *)aKey {
    swModule->setKey([aKey swKey]);
}

- (id)createKey {
    sword::SWKey *sk = swModule->CreateKey();
    SwordKey *newKey = [SwordKey swordKeyWithSWKey:sk makeCopy:YES];
    delete sk;

    return newKey;
}

- (id)getKey {
    return [SwordKey swordKeyWithSWKey:swModule->getKey()];
}

- (id)getKeyCopy {
    return [SwordKey swordKeyWithSWKey:swModule->getKey() makeCopy:YES];
}

- (void)setProcessEntryAttributes:(BOOL)flag {
    swModule->processEntryAttributes(flag);
}

- (BOOL)processEntryAttributes {
    return swModule->isProcessEntryAttributes();
}

- (NSString *)renderedText {
    NSString *ret = @"";
    ret = [NSString stringWithUTF8String:swModule->RenderText()];
    if(!ret) {
        ret = [NSString stringWithCString:swModule->RenderText() encoding:NSISOLatin1StringEncoding];
    }
    return ret;
}

- (NSString *)renderedTextFromString:(NSString *)aString {
    NSString *ret = @"";
    ret = [NSString stringWithUTF8String:swModule->RenderText([aString UTF8String])];
    if(!ret) {
        ret = [NSString stringWithCString:swModule->RenderText([aString UTF8String]) encoding:NSISOLatin1StringEncoding];
    }
    return ret;
}

- (NSString *)strippedText {
    NSString *ret = @"";
    ret = [NSString stringWithUTF8String:swModule->StripText()];
    if(!ret) {
        ret = [NSString stringWithCString:swModule->StripText() encoding:NSISOLatin1StringEncoding];
    }
    return ret;
}

- (NSString *)strippedTextFromString:(NSString *)aString {
    NSString *ret = @"";
    ret = [NSString stringWithUTF8String:swModule->RenderText([aString UTF8String])];
    if(!ret) {
        ret = [NSString stringWithCString:swModule->RenderText([aString UTF8String]) encoding:NSISOLatin1StringEncoding];
    }
    return ret;
}

- (NSString *)entryAttributeValuePreverse {
    NSString *ret = @"";
    ret = [NSString stringWithUTF8String:swModule->getEntryAttributes()["Heading"]["Preverse"]["0"].c_str()];
    
    return ret;
}

- (NSString *)entryAttributeValueFootnoteOfType:(NSString *)fnType indexValue:(NSString *)index {
    NSString *ret = @"";    
    if([fnType isEqualToString:@"x"]) {
        ret = [NSString stringWithUTF8String:swModule->getEntryAttributes()["Footnote"][[index UTF8String]]["refList"].c_str()];        
    } else if([fnType isEqualToString:@"n"]) {
        ret = [NSString stringWithUTF8String:swModule->getEntryAttributes()["Footnote"][[index UTF8String]]["body"].c_str()];
    }
    return ret;
}

- (NSArray *)entryAttributeValuesLemma {
    NSMutableArray *array = [NSMutableArray array];

    // parse entry attributes and look for Lemma (String's numbers)
    sword::AttributeTypeList::iterator words;
    sword::AttributeList::iterator word;
    sword::AttributeValue::iterator strongVal;
    words = swModule->getEntryAttributes().find("Word");
    if(words != swModule->getEntryAttributes().end()) {
        for(word = words->second.begin();word != words->second.end(); word++) {
            strongVal = word->second.find("Lemma");
            if(strongVal != word->second.end()) {
                // pass empty "Text" entries
                if(strongVal->second == "G3588") {
                    if (word->second.find("Text") == word->second.end())
                        continue;	// no text? let's skip
                }
                NSMutableString *stringValStr = [NSMutableString stringWithUTF8String:strongVal->second];
                if(stringValStr) {
                    [stringValStr replaceOccurrencesOfString:@"|x-Strongs:" withString:@" " options:0 range:NSMakeRange(0, [stringValStr length])];                    
                    [array addObject:stringValStr];
                }
            }
        }
    }
    return [NSArray arrayWithArray:array];
}

#pragma mark - Locking methods

- (SwordModuleTextEntry *)textEntryForKey:(SwordKey *)aKey textType:(TextPullType)aType {
    SwordModuleTextEntry *ret = nil;
    
    if(aKey) {
        [moduleLock lock];
        [self setKey:aKey];
        if(![self error]) {
            NSString *txt = @"";
            if(aType == TextTypeRendered) {
                txt = [self renderedText];
            } else {
                txt = [self strippedText];
            }
            
            if(txt) {
                ret = [SwordModuleTextEntry textEntryForKey:[aKey keyText] andText:txt];
            } else {
                ALog(@"Nil key");
            }
        }
        [moduleLock unlock];
    }
    
    return ret;
}

// general feature access
- (BOOL)hasFeature:(NSString *)feature {
	BOOL has = NO;
	
	if(swModule->getConfig().has("Feature", [feature UTF8String])) {
		has = YES;
    } else if (swModule->getConfig().has("GlobalOptionFilter", [[NSString stringWithFormat:@"GBF%@", feature] UTF8String])) {
 		has = YES;
    } else if (swModule->getConfig().has("GlobalOptionFilter", [[NSString stringWithFormat:@"ThML%@", feature] UTF8String])) {
 		has = YES;
    } else if (swModule->getConfig().has("GlobalOptionFilter", [[NSString stringWithFormat:@"UTF8%@", feature] UTF8String])) {
 		has = YES;
    } else if (swModule->getConfig().has("GlobalOptionFilter", [[NSString stringWithFormat:@"OSIS%@", feature] UTF8String])) {
 		has = YES;
    } else if (swModule->getConfig().has("GlobalOptionFilter", [feature UTF8String])) {
 		has = YES;
    }
	
	return has;
}

- (NSString *)configFileEntryForConfigKey:(NSString *)entryKey {
	NSString *result = nil;
    
	[moduleLock lock];
    const char *entryStr = swModule->getConfigEntry([entryKey UTF8String]);
	if(entryStr) {
		result = [NSString stringWithUTF8String:entryStr];
        if(!result) {
            result = [NSString stringWithCString:entryStr encoding:NSISOLatin1StringEncoding];
        }
    }
	[moduleLock unlock];
	
	return result;
}

- (NSString *)entryAttributeValuePreverseForKey:(SwordKey *)aKey {
    [moduleLock lock];
    [self setKey:aKey];
    swModule->RenderText(); // force processing of key
    NSString *value = [self entryAttributeValuePreverse];
    [moduleLock unlock];
    return value;
}

- (NSString *)entryAttributeValueFootnoteOfType:(NSString *)fnType indexValue:(NSString *)index forKey:(SwordKey *)aKey {
    [moduleLock lock];
    [self setKey:aKey];
    swModule->RenderText(); // force processing of key
    NSString *value = [self entryAttributeValueFootnoteOfType:fnType indexValue:index];
    [moduleLock unlock];
    return value;
}

- (sword::SWModule *)swModule {
	return swModule;
}

@end
