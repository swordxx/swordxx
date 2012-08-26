//
//  SwordInstallManager.mm
//  Eloquent
//
//  Created by Manfred Bergmann on 13.08.07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "SwordInstallSourceManager.h"
#import "SwordInstallSource.h"
#import "SwordManager.h"
#import "ObjCSword_Prefix.pch"

#ifdef __cplusplus
typedef std::map<sword::SWBuf, sword::InstallSource *> InstallSourceMap;
typedef sword::multimapwithdefault<sword::SWBuf, sword::SWBuf, std::less <sword::SWBuf> > ConfigEntMap;
#endif

#define INSTALLSOURCE_SECTION_TYPE_FTP  "FTPSource"
#define INSTALLSOURCE_SECTION_TYPE_HTTP	"HTTPSource"

@implementation SwordInstallSourceManager

@dynamic configPath;

// ------------------- getter / setter -------------------
- (NSString *)configPath {
    return configPath;
}

- (void)setConfigPath:(NSString *)value {
    DLog(@"");
    
    if(configPath != value) {
        [configPath release];
        configPath = [value copy];
        
        if(value == nil) {
            return;
        }
        
        
        // check for existence
        NSFileManager *fm = [NSFileManager defaultManager];
        BOOL isDir;
        if(([fm fileExistsAtPath:configPath] == NO) && createPath == YES) {
            // create path
            [fm createDirectoryAtPath:configPath withIntermediateDirectories:NO attributes:nil error:NULL];
        }
        
        if(([fm fileExistsAtPath:configPath isDirectory:&isDir] == YES) && (isDir)) {
            // set configFilePath
            [self setConfigFilePath:[configPath stringByAppendingPathComponent:@"InstallMgr.conf"]];
            
            // check config
            if([fm fileExistsAtPath:self.configFilePath] == NO) {
                // create config entry
                sword::SWConfig config([self.configFilePath cStringUsingEncoding:NSUTF8StringEncoding]);
                config["General"]["PassiveFTP"] = "true";
                config.Save();

                // create default Install source
                SwordInstallSource *is = [[[SwordInstallSource alloc] initWithType:INSTALLSOURCE_TYPE_FTP] autorelease];
                [is setCaption:@"CrossWire"];
                [is setSource:@"ftp.crosswire.org"];
                [is setDirectory:@"/pub/sword/raw"];
                                    
                // addInstallSource will reinitialize
                [self addInstallSource:is withReinitialize:NO];
                
            }
            // init installMgr
            [self reinitialize];                

            // sync with master install source list
            //[self refreshMasterRemoteInstallSourceList];

        } else {
            ALog(@"Config path does not exist: %@", configPath);
        }
    }
}

// -------------------- methods --------------------

// initialization
+ (SwordInstallSourceManager *)defaultController {
    static SwordInstallSourceManager *singleton = nil;
    if(singleton == nil) {
        singleton = [[SwordInstallSourceManager alloc] init];
    }
    
    return singleton;
}

+ (SwordInstallSourceManager *)defaultControllerWithPath:(NSString *)aPath {
    SwordInstallSourceManager *m = [SwordInstallSourceManager defaultController];
    [m setConfigPath:aPath];
    return m;
}

+ (SwordInstallSourceManager *)controllerWithPath:(NSString *)aPath {
    return [[[SwordInstallSourceManager alloc] initWithPath:aPath createPath:YES] autorelease];
}

/**
base path of the module installation
 */
- (id)init {
    self = [super init];
    if(self) {
        createPath = NO;
        [self setConfigPath:nil];
        [self setConfigFilePath:nil];
        [self setInstallSources:[NSMutableDictionary dictionary]];
        [self setInstallSourceList:[NSMutableArray array]];
    }
    
    return self;
}

/**
 initialize with given path
 */
- (id)initWithPath:(NSString *)aPath createPath:(BOOL)create {
    self = [self init];
    if(self) {
        createPath = create;
        [self setConfigPath:aPath];
    }
    
    return self;
}

/** re-init after adding or removing new modules */
- (void)reinitialize {
    sword::SWConfig config([self.configFilePath UTF8String]);
    config.Load();

    // init installMgr
    BOOL disclaimerConfirmed = NO;
    if(swInstallMgr != nil) {
        disclaimerConfirmed = [self userDisclaimerConfirmed];
    }
    swInstallMgr = new sword::InstallMgr([configPath UTF8String]);
    if(swInstallMgr == nil) {
        ALog(@"Could not initialize InstallMgr!");
    } else {
        [self setUserDisclaimerConfirmed:disclaimerConfirmed];
        
        // empty all lists
        [self.installSources removeAllObjects];
        [self.installSourceList removeAllObjects];
        
        // init install sources
        for(InstallSourceMap::iterator it = swInstallMgr->sources.begin(); it != swInstallMgr->sources.end(); it++) {
            sword::InstallSource *sis = it->second;
            SwordInstallSource *is = [[[SwordInstallSource alloc] initWithSource:sis] autorelease];
            
            [self.installSources setObject:is forKey:[is caption]];
            // also add to list
            [self.installSourceList addObject:is];
        }
    }
}

- (void)finalize {
    if(swInstallMgr != nil) {
        delete swInstallMgr;
    }

    [super finalize];
}

- (void)dealloc {
    if(swInstallMgr != nil) {
        delete swInstallMgr;
    }
    
    [self setConfigPath:nil];
    [self setInstallSources:nil];
    [self setInstallSourceList:nil];
    [self setConfigFilePath:nil];
    
    [super dealloc];
}

- (void)addInstallSource:(SwordInstallSource *)is {
    [self addInstallSource:is withReinitialize:YES];
}

// add/remove install sources
- (void)addInstallSource:(SwordInstallSource *)is withReinitialize:(BOOL)reinit {
    
    // save at once
    sword::SWConfig config([self.configFilePath cStringUsingEncoding:NSUTF8StringEncoding]);
	if([[is type] isEqualToString:INSTALLSOURCE_TYPE_FTP]) {
		config["Sources"].insert(ConfigEntMap::value_type(INSTALLSOURCE_SECTION_TYPE_FTP, [[is configEntry] UTF8String]));
	} else {
		config["Sources"].insert(ConfigEntMap::value_type(INSTALLSOURCE_SECTION_TYPE_HTTP, [[is configEntry] UTF8String]));
	}
    config.Save();
    
	if(reinit)
		[self reinitialize];
}

- (void)removeInstallSource:(SwordInstallSource *)is {
    [self removeInstallSource:is withReinitialize:NO];
}

- (void)removeInstallSource:(SwordInstallSource *)is withReinitialize:(BOOL)reinit {
    
    // remove source
    [self.installSources removeObjectForKey:[is caption]];
    [self.installSourceList removeObject:is];
    
    // save at once
    sword::SWConfig config([self.configFilePath cStringUsingEncoding:NSUTF8StringEncoding]);
    config["Sources"].erase(INSTALLSOURCE_SECTION_TYPE_HTTP);
    config["Sources"].erase(INSTALLSOURCE_SECTION_TYPE_FTP);
    
    // build up new
    for(SwordInstallSource *sis in self.installSources) {
		if([[sis type] isEqualToString:INSTALLSOURCE_TYPE_FTP]) {
			config["Sources"].insert(ConfigEntMap::value_type(INSTALLSOURCE_SECTION_TYPE_FTP, [[sis configEntry] UTF8String]));
		} else {
			config["Sources"].insert(ConfigEntMap::value_type(INSTALLSOURCE_SECTION_TYPE_HTTP, [[sis configEntry] UTF8String]));
		}
    }
    config.Save();
    
	if(reinit)
		[self reinitialize];
}

- (void)updateInstallSource:(SwordInstallSource *)is {
    // first remove, then add again
    [self removeInstallSource:is withReinitialize:NO];
    [self addInstallSource:is];
}

// installation/unInstallation
- (int)installModule:(SwordModule *)aModule fromSource:(SwordInstallSource *)is withManager:(SwordManager *)manager {
    
    int stat;
    if([[is source] isEqualToString:@"localhost"]) {
        stat = swInstallMgr->installModule([manager swManager], [[is directory] UTF8String], [[aModule name] UTF8String]);
    } else {
        stat = swInstallMgr->installModule([manager swManager], 0, [[aModule name] UTF8String], [is installSource]);
    }
    
    return stat;
}

- (int)refreshMasterRemoteInstallSourceList {
    int stat = swInstallMgr->refreshRemoteSourceConfiguration();
    if(stat) {
        ALog(@"Unable to refresh with master install source!");
    }
    
    return stat;
}

/**
 uninstalls a module from a SwordManager
 */
- (int)uninstallModule:(SwordModule *)aModule fromManager:(SwordManager *)swManager {
    int stat = swInstallMgr->removeModule([swManager swManager], [[aModule name] UTF8String]);
    
    return stat;
}

// list modules in sources
- (NSArray *)listModulesForSource:(SwordInstallSource *)is {
    return [is listModules];
}

/** refresh modules of this source 
 refreshing the install source is necessary before installation of 
 */
- (int)refreshInstallSource:(SwordInstallSource *)is {
    int ret = 1;
    
    if(is == nil) {
        ALog(@"Install source is nil");
    } else {
        if([[is source] isEqualToString:@"localhost"] == NO) {
            ret = swInstallMgr->refreshRemoteSource([is installSource]);
        }
    }
    
    return ret;
}

/**
 returns an array of Modules with status set, nil on error
 */
- (NSArray *)moduleStatusInInstallSource:(SwordInstallSource *)is baseManager:(SwordManager *)baseMgr {
    
    NSArray *ret = nil;
    
    // get modules map
    NSMutableArray *ar = [NSMutableArray array];
    std::map<sword::SWModule *, int> modStats = swInstallMgr->getModuleStatus(*[baseMgr swManager], *[[is swordManager] swManager]);
    sword::SWModule *module;
	int status;
	for(std::map<sword::SWModule *, int>::iterator it = modStats.begin(); it != modStats.end(); it++) {
		module = it->first;
		status = it->second;
        
        SwordModule *mod = [[[SwordModule alloc] initWithSWModule:module] autorelease];
        [mod setStatus:status];
        [ar addObject:mod];
	}
    
    if(ar) {
        ret = [NSArray arrayWithArray:ar];
    }
    
    return ret;
}

- (BOOL)userDisclaimerConfirmed {
    return swInstallMgr->isUserDisclaimerConfirmed();
}

- (void)setUserDisclaimerConfirmed:(BOOL)flag {
    swInstallMgr->setUserDisclaimerConfirmed(flag);
}

/** low level access */
- (sword::InstallMgr *)installMgr {
    return swInstallMgr;
}

@end
