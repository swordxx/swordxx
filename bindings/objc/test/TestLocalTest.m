//
//  TestLocalTest.m
//  ObjCSword
//
//  Created by Manfred Bergmann on 12.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import "TestLocalTest.h"


@implementation TestLocalTest

- (void)testCreateTest {
    Test *testLocal = [[Test alloc] init];
    [testLocal sayHello];
}

- (void)testOtherClass {
    Test *testLocal = [[Test alloc] init];
    [testLocal sayHello];

    [testLocal setClass:[Test1 class]];
    [testLocal sayHello];
}

@end
