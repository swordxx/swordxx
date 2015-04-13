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

@interface SwordInstallSourceManagerTest : XCTestCase

@end

@implementation SwordInstallSourceManagerTest

NSString *testConfigPath = @"/tmp/testmodinst";

- (void)setUp {
    NSFileManager *fm = [NSFileManager defaultManager];
    [fm removeItemAtPath:testConfigPath error:NULL];

    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)testObjectCreate {
    SwordInstallSourceManager *mgr = [[SwordInstallSourceManager alloc] initWithPath:@"" createPath:YES];
    XCTAssertTrue(mgr != nil, @"");
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
