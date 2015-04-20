//
//  SwordInstallSourceManagerTest.m
//  ObjCSword
//
//  Created by Manfred Bergmann on 12.04.15.
//
//

#import <XCTest/XCTest.h>
#import "SwordInstallSourceManager.h"
#import "SwordInstallSource.h"
#import "SwordManager.h"

@interface SwordInstallSourceManagerTest : XCTestCase

@end

@implementation SwordInstallSourceManagerTest

NSString *testConfigPath = @"/tmp/testmodinst";
NSString *testModuleManagerPath = @"/tmp/testmodmgr";
#define PROJECT_PATH [@"~/Development/MySources/crosswire/Eloquent_MacOS/trunk/sword_src/sword-trunk/bindings/objc" stringByExpandingTildeInPath]
#define LOCAL_INSTALLSOURCE_PATH [PROJECT_PATH stringByAppendingPathComponent:@"LocalTestInstallSource"]

- (void)setUp {
    NSFileManager *fm = [NSFileManager defaultManager];
    [fm removeItemAtPath:testConfigPath error:NULL];
    [fm removeItemAtPath:testModuleManagerPath error:NULL];

    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)testObjectCreate {
    SwordInstallSourceManager *mgr = [[SwordInstallSourceManager alloc] initWithPath:@"" createPath:YES];
    XCTAssertTrue(mgr != nil, @"");
}

- (void)testLocalInstallSource {
    SwordInstallSource *is = [[SwordInstallSource alloc] init];
    [is setSource:@"localhost"];
    [is setDirectory:LOCAL_INSTALLSOURCE_PATH];
    [is setCaption:@"LocalTest"];

    SwordInstallSourceManager *mgr = [[SwordInstallSourceManager alloc] initWithPath:testConfigPath createPath:YES];
    [mgr initManager];
    [mgr addInstallSource:is];

    NSArray *mods = [mgr listModulesForSource:is];
    XCTAssertTrue([mods count] == 1);
    XCTAssertTrue([[((SwordModule *) mods[0]) name] isEqualToString:@"KJV"]);
}

- (void)testLocalInstallSourceStatusNew {
    SwordInstallSource *is = [[SwordInstallSource alloc] init];
    [is setSource:@"localhost"];
    [is setDirectory:LOCAL_INSTALLSOURCE_PATH];
    [is setCaption:@"LocalTest"];

    SwordInstallSourceManager *mgr = [[SwordInstallSourceManager alloc] initWithPath:testConfigPath createPath:YES];
    [mgr initManager];
    [mgr addInstallSource:is];

    SwordManager *swMgr = [SwordManager managerWithPath:testModuleManagerPath];
    NSArray *stats = [mgr moduleStatusInInstallSource:is baseManager:swMgr];
    for(SwordModule *mod in stats) {
        NSLog(@"mod.name=%@", [mod name]);
        NSLog(@"mod.stat=%i", [mod status]);
    }

    XCTAssertTrue([((SwordModule *) stats[0]).name isEqualToString:@"KJV"]);
    XCTAssertTrue(((SwordModule *) stats[0]).status == ModStatNew);
}

- (void)testLocalInstallSourceStatusSame {
    SwordInstallSource *is = [[SwordInstallSource alloc] init];
    [is setSource:@"localhost"];
    [is setDirectory:LOCAL_INSTALLSOURCE_PATH];
    [is setCaption:@"LocalTest"];

    SwordManager *swMgr = [SwordManager managerWithPath:testModuleManagerPath];
    SwordInstallSourceManager *mgr = [[SwordInstallSourceManager alloc] initWithPath:testConfigPath createPath:YES];
    [mgr initManager];
    [mgr addInstallSource:is];

    [mgr installModule:[mgr listModulesForSource:is][0] fromSource:is withManager:swMgr];

    [swMgr reInit];
    NSArray *stats = [mgr moduleStatusInInstallSource:is baseManager:swMgr];
    for(SwordModule *mod in stats) {
        NSLog(@"mod.name=%@", [mod name]);
        NSLog(@"mod.stat=%i", [mod status]);
    }

    XCTAssertTrue([((SwordModule *) stats[0]).name isEqualToString:@"KJV"]);
    XCTAssertTrue(((SwordModule *) stats[0]).status == ModStatSameVersion);
}

- (void)testInstallModuleFromLocalSource {
    SwordInstallSource *is = [[SwordInstallSource alloc] init];
    [is setSource:@"localhost"];
    [is setDirectory:LOCAL_INSTALLSOURCE_PATH];
    [is setCaption:@"LocalTest"];

    SwordManager *swMgr = [SwordManager managerWithPath:testModuleManagerPath];
    SwordInstallSourceManager *mgr = [[SwordInstallSourceManager alloc] initWithPath:testConfigPath createPath:YES];
    [mgr initManager];
    [mgr addInstallSource:is];

    int stat = [mgr installModule:[mgr listModulesForSource:is][0] fromSource:is withManager:swMgr];
    XCTAssertTrue(stat == 0);
}

- (void)testInitManagerCheckConfigPath {
    // make sure this folder doesn't exist at start
    NSFileManager *fm = [NSFileManager defaultManager];
    BOOL isDir;
    BOOL existsFolder = [fm fileExistsAtPath:testConfigPath isDirectory:&isDir];
    XCTAssertFalse(existsFolder, @"");

    SwordInstallSourceManager *mgr = [[SwordInstallSourceManager alloc] initWithPath:testConfigPath createPath:YES];
    [mgr initManager];

    // make sure the folder was created
    existsFolder = [fm fileExistsAtPath:testConfigPath isDirectory:&isDir];
    BOOL existsInstallMgrConf = [fm fileExistsAtPath:[testConfigPath stringByAppendingPathComponent:@"InstallMgr.conf"]];
    
    XCTAssertTrue(isDir, @"");
    XCTAssertTrue(existsFolder, @"");
    XCTAssertTrue(existsInstallMgrConf, @"");
}

- (void)testHasOneInitialInstallSource {
    SwordInstallSourceManager *mgr = [[SwordInstallSourceManager alloc] initWithPath:testConfigPath createPath:YES];
    [mgr initManager];

    XCTAssertTrue([[mgr installSources] count] == 1, @"");
    XCTAssertTrue([[mgr installSourceList] count] == 1, @"");

    XCTAssertEqual([[mgr installSourceList] firstObject], [mgr installSources][@"CrossWire"]);

    XCTAssertTrue([[[[mgr installSourceList] firstObject] caption] isEqualToString:@"CrossWire"], @"");
    XCTAssertTrue([[[[mgr installSourceList] firstObject] source] isEqualToString:@"ftp.crosswire.org"], @"");
    XCTAssertTrue([[[[mgr installSourceList] firstObject] directory] isEqualToString:@"/pub/sword/raw"], @"");
}

- (void)testAddInstallSource {
    SwordInstallSourceManager *mgr = [[SwordInstallSourceManager alloc] initWithPath:testConfigPath createPath:YES];
    [mgr initManager];

    SwordInstallSource *is = [[SwordInstallSource alloc] initWithType:INSTALLSOURCE_TYPE_FTP];
    [is setCaption:@"test"];
    [is setSource:@"foo.bar.local"];
    [is setDirectory:@"/foobar"];

    [mgr addInstallSource:is];

    XCTAssertTrue([[mgr installSources] count] == 2, @"");
    XCTAssertTrue([[mgr installSourceList] count] == 2, @"");

    XCTAssertTrue([[[mgr installSources][@"test"] caption] isEqualToString:@"test"], @"");
    XCTAssertTrue([[[mgr installSources][@"test"] source] isEqualToString:@"foo.bar.local"], @"");
    XCTAssertTrue([[[mgr installSources][@"test"] directory] isEqualToString:@"/foobar"], @"");
}

- (void)testRemoveInstallSource {
    SwordInstallSourceManager *mgr = [[SwordInstallSourceManager alloc] initWithPath:testConfigPath createPath:YES];
    [mgr initManager];

    // first add
    SwordInstallSource *is = [[SwordInstallSource alloc] initWithType:INSTALLSOURCE_TYPE_FTP];
    [is setCaption:@"test"];
    [is setSource:@"foo.bar.local"];
    [is setDirectory:@"/foobar"];

    [mgr addInstallSource:is];

    XCTAssertTrue([[mgr installSources] count] == 2, @"");
    XCTAssertTrue([[mgr installSourceList] count] == 2, @"");

    XCTAssertTrue([[[mgr installSources][@"test"] caption] isEqualToString:@"test"], @"");
    XCTAssertTrue([[[mgr installSources][@"test"] source] isEqualToString:@"foo.bar.local"], @"");
    XCTAssertTrue([[[mgr installSources][@"test"] directory] isEqualToString:@"/foobar"], @"");

    // then remove
    [mgr removeInstallSource:is];

    XCTAssertTrue([[mgr installSources] count] == 1, @"");
    XCTAssertTrue([[mgr installSourceList] count] == 1, @"");
}

- (void)testUseAsDefaultManager {
    SwordInstallSourceManager *mgr = [[SwordInstallSourceManager alloc] initWithPath:testConfigPath createPath:YES];
    [mgr useAsDefaultManager];

    SwordInstallSourceManager *mgr2 = [SwordInstallSourceManager defaultManager];
    XCTAssertEqual(mgr, mgr2);
}

- (void)testExample {
    XCTAssert(YES, @"Pass");
}

@end
