//
//  SwordManagerTest.m
//  ObjCSword
//
//  Created by Manfred Bergmann on 14.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import "SwordManagerTest.h"

#ifdef TARGET_IPHONE_SIMULATOR
#import "Configuration.h"
#import "iOSConfiguration.h"
#import "OSXConfiguration.h"
#import "SwordManager.h"
#else
#import "ObjCSword/SwordManager.h"
#import "ObjCSword/Configuration.h"
#import "ObjCSword/OSXConfiguration.h"
#endif

@implementation SwordManagerTest

- (void)setUp {
#ifdef TARGET_IPHONE_SIMULATOR
    [[Configuration config] setClass:[iOSConfiguration class]];
#else
    [[Configuration config] setClass:[OSXConfiguration class]];
#endif
}

- (void)testSwordManagerInit {
    NSString *modPath = [[Configuration config] defaultModulePath];
    NSLog(@"%@", modPath);
    
    SwordManager *sm = [SwordManager managerWithPath:modPath]; 
    STAssertTrue([[sm modules] count] > 0, @"");
}

@end
