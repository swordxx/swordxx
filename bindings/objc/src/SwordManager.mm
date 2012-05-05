/*	SwordManager.mm - Sword API wrapper for Modules.

    Copyright 2008 Manfred Bergmann
    Based on code by Will Thimbleby

	This program is free software; you can redistribute it and/or modify it under the terms of the
	GNU General Public License as published by the Free Software Foundation version 2.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
	even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	General Public License for more details. (http://www.gnu.org/licenses/gpl.html)
*/

#import "SwordManager.h"
#import "Configuration.h"
#include <string>
#include <list>

#include "gbfplain.h"
#include "thmlplain.h"
#include "osisplain.h"
#import "Notifications.h"
#import "SwordBook.h"
#import "SwordModule.h"
#import "SwordBible.h"
#import "SwordCommentary.h"
#import "SwordDictionary.h"
#import "SwordListKey.h"
#import "SwordVerseKey.h"
#import "ObjCSword_Prefix.pch"

using std::string;
using std::list;

@interface SwordManager (PrivateAPI)

- (void)refreshModules;
- (void)addFiltersToModule:(sword::SWModule *)mod;

@end

@implementation SwordManager (PrivateAPI)

- (void)refreshModules {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    
    // loop over modules
    sword::SWModule *mod;
	for(sword::ModMap::iterator it = swManager->Modules.begin(); it != swManager->Modules.end(); it++) {
		mod = it->second;
        
        if(mod) {
            // temporary instance
            SwordModule *swMod = [SwordModule moduleForSWModule:mod];
            NSString *type = [swMod typeString];

            ModuleType aType = [SwordModule moduleTypeForModuleTypeString:type];
            SwordModule *sm = [SwordModule moduleForType:aType swModule:mod swordManager:self];
            [dict setObject:sm forKey:[[sm name] lowercaseString]];

            [self addFiltersToModule:mod];
        }
	}
    
    // set modules
    self.modules = dict;
}

- (void)addFiltersToModule:(sword::SWModule *)mod {
    // prepare display filters
    switch(mod->Markup()) {
        case sword::FMT_GBF:
            if(!gbfFilter) {
                gbfFilter = new sword::GBFHTMLHREF();
            }
            if(!gbfStripFilter) {
                gbfStripFilter = new sword::GBFPlain();
            }
            mod->AddRenderFilter(gbfFilter);
            mod->AddStripFilter(gbfStripFilter);
            break;
        case sword::FMT_THML:
            if(!thmlFilter) {
                thmlFilter = new sword::ThMLHTMLHREF();
            }
            if(!thmlStripFilter) {
                thmlStripFilter = new sword::ThMLPlain();
            }
            mod->AddRenderFilter(thmlFilter);
            mod->AddStripFilter(thmlStripFilter);
            break;
        case sword::FMT_OSIS:
            if(!osisFilter) {
                osisFilter = new sword::OSISHTMLHREF();
            }
            if(!osisStripFilter) {
                osisStripFilter = new sword::OSISPlain();
            }
            mod->AddRenderFilter(osisFilter);
            mod->AddStripFilter(osisStripFilter);
            break;
        case sword::FMT_TEI:
            if(!teiFilter) {
                teiFilter = new sword::TEIHTMLHREF();
            }
            mod->AddRenderFilter(teiFilter);
            break;
        case sword::FMT_PLAIN:
        default:
            if(!plainFilter) {
                plainFilter = new sword::PLAINHTML();
            }
            mod->AddRenderFilter(plainFilter);
            break;
    }    
}

@end

@implementation SwordManager

@synthesize modules;
@synthesize modulesPath;
@synthesize managerLock;
@synthesize temporaryManager;

# pragma mark - class methods

+ (NSDictionary *)linkDataForLinkURL:(NSURL *)aURL {
    NSMutableDictionary *ret = [NSMutableDictionary dictionary];
    
    NSString *scheme = [aURL scheme];
    if([scheme isEqualToString:@"sword"]) {
        // in this case host is the module and path the reference
        [ret setObject:[aURL host] forKey:ATTRTYPE_MODULE];
        [ret setObject:[[[aURL path] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding] 
                        stringByReplacingOccurrencesOfString:@"/" withString:@""]
                forKey:ATTRTYPE_VALUE];
        [ret setObject:@"scriptRef" forKey:ATTRTYPE_TYPE];
        [ret setObject:@"showRef" forKey:ATTRTYPE_ACTION];
    } else if([scheme isEqualToString:@"applewebdata"]) {
        // in this case
        NSString *path = [aURL path];
        NSString *query = [aURL query];
        if([[path lastPathComponent] isEqualToString:@"passagestudy.jsp"]) {
            NSArray *data = [query componentsSeparatedByString:@"&"];
            NSString *type = @"x";
            NSString *module = @"";
            NSString *passage = @"";
            NSString *value = @"1";
            NSString *action = @"";
            for(NSString *entry in data) {
                if([entry hasPrefix:@"type="]) {
                    type = [[entry componentsSeparatedByString:@"="] objectAtIndex:1];
                } else if([entry hasPrefix:@"module="]) {
                    module = [[entry componentsSeparatedByString:@"="] objectAtIndex:1];
                } else if([entry hasPrefix:@"passage="]) {
                    passage = [[entry componentsSeparatedByString:@"="] objectAtIndex:1];
                } else if([entry hasPrefix:@"action="]) {
                    action = [[entry componentsSeparatedByString:@"="] objectAtIndex:1];                    
                } else if([entry hasPrefix:@"value="]) {
                    value = [[entry componentsSeparatedByString:@"="] objectAtIndex:1];                    
                } else {
                    ALog(@"Unknown parameter: %@", entry);
                }
            }
            
            [ret setObject:module forKey:ATTRTYPE_MODULE];
            [ret setObject:passage forKey:ATTRTYPE_PASSAGE];
            [ret setObject:value forKey:ATTRTYPE_VALUE];
            [ret setObject:action forKey:ATTRTYPE_ACTION];
            [ret setObject:type forKey:ATTRTYPE_TYPE];
        }
    }
    
    return ret;
}

+ (NSArray *)moduleTypes {
    return [NSArray arrayWithObjects:
            SWMOD_TYPES_BIBLES, 
            SWMOD_TYPES_COMMENTARIES,
            SWMOD_TYPES_DICTIONARIES,
            SWMOD_TYPES_GENBOOKS, nil];
}

+ (SwordManager *)managerWithPath:(NSString *)path {
    SwordManager *manager = [[[SwordManager alloc] initWithPath:path] autorelease];
    return manager;
}

+ (SwordManager *)defaultManager {
    static SwordManager *instance;
    if(instance == nil) {
        // use default path
        instance = [[SwordManager alloc] initWithPath:[[Configuration config] defaultModulePath]];
    }
    
	return instance;
}

- (id)initWithPath:(NSString *)path {

	if((self = [super init])) {
        // this is our main swManager
        temporaryManager = NO;
        
        self.modulesPath = path;

		self.modules = [NSDictionary dictionary];
		self.managerLock = (NSLock *)[[NSRecursiveLock alloc] init];

        [self reInit];
        
        sword::StringList options = swManager->getGlobalOptions();
        sword::StringList::iterator	it;
        for(it = options.begin(); it != options.end(); it++) {
            [self setGlobalOption:[NSString stringWithCString:it->c_str() encoding:NSUTF8StringEncoding] value:SW_OFF];
        }        
    }	
	
	return self;
}

- (id)initWithSWMgr:(sword::SWMgr *)aSWMgr {
    self = [super init];
    if(self) {
        swManager = aSWMgr;
        // this is a temporary swManager
        temporaryManager = YES;
        
		self.modules = [NSDictionary dictionary];
        self.managerLock = (NSLock *)[[NSRecursiveLock alloc] init];
        
		[self refreshModules];
    }
    
    return self;
}

- (void)finalize {
    if(!temporaryManager) {
        delete swManager;
    }
    
	[super finalize];
}

- (void)dealloc {
    if(!temporaryManager) {
        delete swManager;
    }
    [self setModules:nil];
    [self setModulesPath:nil];
    [self setManagerLock:nil];
    
    [super dealloc];
}

- (void)reInit {
	[managerLock lock];
    if(modulesPath && [modulesPath length] > 0) {
        
        // modulePath is the main sw manager
        swManager = new sword::SWMgr([modulesPath UTF8String], true, new sword::EncodingFilterMgr(sword::ENC_UTF8));

        if(!swManager) {
            ALog(@"Cannot create SWMgr instance for default module path!");
        } else {
            NSFileManager *fm = [NSFileManager defaultManager];
            NSArray *subDirs = [fm contentsOfDirectoryAtPath:modulesPath error:NULL];
            // for all sub directories add module
            BOOL directory;
            NSString *fullSubDir = nil;
            NSString *subDir = nil;
            for(subDir in subDirs) {
                // as long as it's not hidden
                if(![subDir hasPrefix:@"."] && 
                   ![subDir isEqualToString:@"InstallMgr"] && 
                   ![subDir isEqualToString:@"mods.d"] &&
                   ![subDir isEqualToString:@"modules"]) {
                    fullSubDir = [modulesPath stringByAppendingPathComponent:subDir];
                    fullSubDir = [fullSubDir stringByStandardizingPath];
                    
                    //if its a directory
                    if([fm fileExistsAtPath:fullSubDir isDirectory:&directory]) {
                        if(directory) {
                            DLog(@"Augmenting folder: %@", fullSubDir);
                            swManager->augmentModules([fullSubDir UTF8String]);
                            DLog(@"Augmenting folder done");
                        }
                    }
                }
            }
            
            // clear some data
            [self refreshModules];
            
            SendNotifyModulesChanged(nil);
        }
    }
	[managerLock unlock];    
}

- (void)addPath:(NSString *)path {
    
	[managerLock lock];
	if(swManager == nil) {
		swManager = new sword::SWMgr([path UTF8String], true, new sword::EncodingFilterMgr(sword::ENC_UTF8));
    } else {
		swManager->augmentModules([path UTF8String]);
    }
	
	[self refreshModules];
	[managerLock unlock];
    
    SendNotifyModulesChanged(nil);
}

- (SwordModule *)moduleWithName:(NSString *)name {
    
	SwordModule	*ret = [modules objectForKey:[name lowercaseString]];
    if(ret == nil) {
        sword::SWModule *mod = [self getSWModuleWithName:name];
        if(mod == NULL) {
            ALog(@"No module by that name: %@!", name);
        } else {
            // temporary instance
            SwordModule *swMod = [SwordModule moduleForSWModule:mod];
            NSString *type = [swMod typeString];
            
            ModuleType aType = [SwordModule moduleTypeForModuleTypeString:type];
            ret = [SwordModule moduleForType:aType swModule:mod swordManager:self];

            if(ret != nil) {
                NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithDictionary:modules];
                [dict setObject:ret forKey:[name lowercaseString]];
                self.modules = dict;                
            }
        }        
    }
    
	return ret;
}

- (void)setCipherKey:(NSString *)key forModuleNamed:(NSString *)name {
	[managerLock lock];	
	swManager->setCipherKey([name UTF8String], [key UTF8String]);
	[managerLock unlock];
}

#pragma mark - module access

- (void)setGlobalOption:(NSString *)option value:(NSString *)value {
	[managerLock lock];
    swManager->setGlobalOption([option UTF8String], [value UTF8String]);
	[managerLock unlock];
}

- (BOOL)globalOption:(NSString *)option {
    return [[NSString stringWithUTF8String:swManager->getGlobalOption([option UTF8String])] isEqualToString:SW_ON];
}

- (NSArray *)listModules {
    return [modules allValues];
}
- (NSArray *)moduleNames {
    return [modules allKeys];
}

- (NSArray *)sortedModuleNames {
    return [[self moduleNames] sortedArrayUsingSelector:@selector(compare:)];
}

- (NSArray *)modulesForFeature:(NSString *)feature {
    NSMutableArray *ret = [NSMutableArray array];
    for(SwordModule *mod in [modules allValues]) {
        if([mod hasFeature:feature]) {
            [ret addObject:mod];
        }
    }
	
    // sort
    NSArray *sortDescriptors = [NSArray arrayWithObject:[[NSSortDescriptor alloc] initWithKey:@"name" ascending:YES]];
    [ret sortUsingDescriptors:sortDescriptors];

	return [NSArray arrayWithArray:ret];
}

- (NSArray *)modulesForType:(ModuleType)type {
    NSMutableArray *ret = [NSMutableArray array];
    for(SwordModule *mod in [modules allValues]) {
        if([mod type] == type || type == All) {
            [ret addObject:mod];
        }
    }
    
    // sort
    NSArray *sortDescritors = [NSArray arrayWithObject:[[NSSortDescriptor alloc] initWithKey:@"name" ascending:YES]]; 
    [ret sortUsingDescriptors:sortDescritors];
    
	return [NSArray arrayWithArray:ret];
}

- (NSArray *)modulesForCategory:(ModuleCategory)cat {
    NSMutableArray *ret = [NSMutableArray array];
    for(SwordModule *mod in [modules allValues]) {
        if([mod category] == cat) {
            [ret addObject:mod];
        }
    }
    
    // sort
    NSArray *sortDescritors = [NSArray arrayWithObject:[[NSSortDescriptor alloc] initWithKey:@"name" ascending:YES]]; 
    [ret sortUsingDescriptors:sortDescritors];
    
	return [NSArray arrayWithArray:ret];    
}

#pragma mark - lowlevel methods

- (sword::SWMgr *)swManager {
    return swManager;
}

- (sword::SWModule *)getSWModuleWithName:(NSString *)moduleName {
	sword::SWModule *module = NULL;

	[managerLock lock];
	module = swManager->Modules[[moduleName UTF8String]];	
	[managerLock unlock];
    
	return module;
}

@end
