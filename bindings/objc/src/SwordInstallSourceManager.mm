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


@interface SwordInstallSourceManager ()

@end

@implementation SwordInstallSourceManager

// -------------------- methods --------------------

static SwordInstallSourceManager *singleton = nil;
// initialization
+ (SwordInstallSourceManager *)defaultManager {
    if(singleton == nil) {
        singleton = [[SwordInstallSourceManager alloc] init];
    }
    
    return singleton;
}

/**
base path of the module installation
 */
- (id)init {
    self = [super init];
    if(self) {
        [self setCreateConfigPath:NO];
        [self setConfigPath:nil];
        [self setInstallSources:[NSMutableDictionary dictionary]];
        [self setInstallSourceList:[NSMutableArray array]];
        [self setFtpUser:@"ftp"];
        [self setFtpPassword:@"ObjCSword@crosswire.org"];
    }
    
    return self;
}

/**
 initialize with given path
 */
- (id)initWithPath:(NSString *)aPath createPath:(BOOL)create {
    self = [self init];
    if(self) {
        [self setCreateConfigPath:create];
        [self setConfigPath:aPath];
    }
    
    return self;
}

- (void)useAsDefaultManager {
    singleton = self;
}

- (void)dealloc {
    if(swInstallMgr != nil) {
        delete swInstallMgr;
    }
}

/** init after adding or removing new modules */
- (void)initManager {
    [self setupConfig];

    // safe disclaimer flag
    BOOL disclaimerConfirmed = NO;
    if(swInstallMgr != NULL) {
        disclaimerConfirmed = [self userDisclaimerConfirmed];
    }

    if(swInstallMgr == NULL) {
        ALog(@"Initializing swInstallMgr");
        swInstallMgr = [self newDefaultInstallMgr];
        if(swInstallMgr == nil) {
            ALog(@"Could not initialize InstallMgr!");

        } else {
            [self setUserDisclaimerConfirmed:disclaimerConfirmed];

            if(![self existsDefaultInstallSource]) {
                [self addDefaultInstallSource];
            }

            [self setupInstallSources];
        }

    } else {
        ALog(@"Re-initializing swInstallMgr");
        [self reInitialize];
    }
}

- (void)reInitialize {
    [self readInstallMgrConf];
    [self setupInstallSources];
}

- (sword::InstallMgr *)newDefaultInstallMgr {
    ALog(@"Creating InstallMgr with: %@, %i, %@, %@", [self configPath], 0, [self ftpUser], [self ftpPassword]);
    return new sword::InstallMgr(
            [[self configPath] UTF8String],
            0,
            sword::SWBuf([[self ftpUser] UTF8String]),
            sword::SWBuf([[self ftpPassword] UTF8String]));
}

- (BOOL)existsDefaultInstallSource {
    sword::InstallMgr mgr = sword::InstallMgr([[self configPath] UTF8String]);

    for(InstallSourceMap::iterator it = mgr.sources.begin(); it != mgr.sources.end(); it++) {
        sword::InstallSource *sis = it->second;

        if([[NSString stringWithCString:sis->caption.c_str() encoding:NSUTF8StringEncoding] isEqualToString:@"CrossWire"]) {
            return YES;
        }
    }
    return NO;
}

- (void)addDefaultInstallSource {
    SwordInstallSource *is = [[SwordInstallSource alloc] initWithType:INSTALLSOURCE_TYPE_FTP];
    [is setCaption:@"CrossWire"];
    [is setSource:@"ftp.crosswire.org"];
    [is setDirectory:@"/pub/sword/raw"];

    [self addInstallSource:is];
}

- (void)setupConfig {
    if([self configPath] == nil) {
        ALog(@"No config path configured!");
        return;
    }

    // check for existence
    NSFileManager *fm = [NSFileManager defaultManager];
    BOOL isDir;
    ALog(@"Checking for config path at: %@", [self configPath]);
    if(![fm fileExistsAtPath:[self configPath]] && [self createConfigPath]) {
        ALog(@"Config dir doesn't exist, creating it...");
        [fm createDirectoryAtPath:[self configPath] withIntermediateDirectories:NO attributes:nil error:NULL];
        ALog(@"Config dir doesn't exist, creating it...done");
    }

    if([fm fileExistsAtPath:[self configPath] isDirectory:&isDir] && (isDir)) {
        // check config
        if(![fm fileExistsAtPath:[self createInstallMgrConfPath]]) {
            // create config entry
            sword::SWConfig config([[self createInstallMgrConfPath] UTF8String]);
            config["General"]["PassiveFTP"] = "true";
            config.Save();
        }

    } else {
        ALog(@"Config path does not exist: %@", [self configPath]);
    }
}

- (void)setupInstallSources {
    ALog(@"Cleaning all install sources...");
    [[self installSources] removeAllObjects];
    [[self installSourceList] removeAllObjects];
    ALog(@"Cleaning all install sources...done");

    for(InstallSourceMap::iterator it = swInstallMgr->sources.begin(); it != swInstallMgr->sources.end(); it++) {
        sword::InstallSource *sis = it->second;
        SwordInstallSource *is = [[SwordInstallSource alloc] initWithSource:sis];

        ALog(@"Adding install source: %@", [is caption]);
        [self installSources][[is caption]] = is;
        [[self installSourceList] addObject:is];
    }
}

- (NSString *)createInstallMgrConfPath {
    return [[self configPath] stringByAppendingPathComponent:@"InstallMgr.conf"];
}

- (void)addInstallSource:(SwordInstallSource *)is {
    ALog(@"Adding install source: %@", [is caption]);
    [self installSources][[is caption]] = is;
    [[self installSourceList] addObject:is];

    // modify conf file
    sword::SWConfig config([[self createInstallMgrConfPath] UTF8String]);
	if([[is type] isEqualToString:INSTALLSOURCE_TYPE_FTP]) {
		config["Sources"].insert(ConfigEntMap::value_type(INSTALLSOURCE_SECTION_TYPE_FTP, [[is configEntry] UTF8String]));
	} else {
		config["Sources"].insert(ConfigEntMap::value_type(INSTALLSOURCE_SECTION_TYPE_HTTP, [[is configEntry] UTF8String]));
	}
    config.Save();

    [self readInstallMgrConf];
}

- (void)removeInstallSource:(SwordInstallSource *)is {
    ALog(@"Removing install source: %@", [is caption]);
    [self.installSources removeObjectForKey:[is caption]];
    [self.installSourceList removeObject:is];

    // modify conf file
    sword::SWConfig config([[self createInstallMgrConfPath] UTF8String]);
    config["Sources"].erase(INSTALLSOURCE_SECTION_TYPE_HTTP);
    config["Sources"].erase(INSTALLSOURCE_SECTION_TYPE_FTP);

    // build up new
    for(SwordInstallSource *sis in self.installSourceList) {
		if([[sis type] isEqualToString:INSTALLSOURCE_TYPE_FTP]) {
			config["Sources"].insert(ConfigEntMap::value_type(INSTALLSOURCE_SECTION_TYPE_FTP, [[sis configEntry] UTF8String]));
		} else {
			config["Sources"].insert(ConfigEntMap::value_type(INSTALLSOURCE_SECTION_TYPE_HTTP, [[sis configEntry] UTF8String]));
		}
    }
    config.Save();

    [self readInstallMgrConf];
}

- (void)updateInstallSource:(SwordInstallSource *)is {
    // first remove, then add again
    [self removeInstallSource:is];
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
    } else {
        [self reInitialize];
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
        if(![[is source] isEqualToString:@"localhost"]) {
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
        
        SwordModule *mod = [[SwordModule alloc] initWithSWModule:module];
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

- (void)readInstallMgrConf {
    swInstallMgr->readInstallConf();
}

/** low level access */
- (sword::InstallMgr *)installMgr {
    return swInstallMgr;
}

@end
