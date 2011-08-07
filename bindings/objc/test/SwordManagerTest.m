//
//  SwordManagerTest.m
//  ObjCSword
//
//  Created by Manfred Bergmann on 14.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import "SwordManagerTest.h"

#import "Configuration.h"
//#import "iOSConfiguration.h"
#import "OSXConfiguration.h"
#import "SwordManager.h"

@implementation SwordManagerTest

- (void)setUp {
#ifdef TARGET_IPHONE_SIMULATOR
//    [[Configuration config] setClass:[iOSConfiguration class]];
#else
#endif
    [Configuration configWithImpl:[[OSXConfiguration alloc] init]];
}

- (void)testSwordManagerInit {
    NSString *modPath = [[Configuration config] defaultModulePath];
    NSLog(@"%@", modPath);
    
    SwordManager *sm = [SwordManager managerWithPath:modPath]; 
    STAssertTrue([[sm modules] count] > 0, @"");
}

@end
