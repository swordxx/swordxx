//
//  SwordManagerTest.m
//  ObjCSword
//
//  Created by Manfred Bergmann on 14.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import "SwordManagerTest.h"
#import "ObjCSword/SwordManager.h"
#import "ObjCSword/Configuration.h"
#import "ObjCSword/OSXConfiguration.h"


@implementation SwordManagerTest

- (void)setUp {
    [[Configuration config] setClass:[OSXConfiguration class]];
}

- (void)testSwordManagerInit {
    SwordManager *sm = [SwordManager defaultManager];
    STAssertTrue([[sm modules] count] > 0, @"");
}

@end
