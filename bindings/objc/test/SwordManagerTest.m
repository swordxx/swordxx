//
//  SwordManagerTest.m
//  ObjCSword
//
//  Created by Manfred Bergmann on 14.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import <ObjCSword/ObjCSword.h>
#import "SwordManagerTest.h"

@implementation SwordManagerTest

- (void)setUp {
    [Configuration configWithImpl:[[[OSXConfiguration alloc] init] autorelease]];
}

- (void)testSwordManagerInit {
    NSString *modPath = [[Configuration config] defaultModulePath];
    NSLog(@"%@", modPath);
    
    SwordManager *sm = [SwordManager managerWithPath:modPath]; 
    STAssertTrue([[sm modules] count] > 0, @"");
}

@end
