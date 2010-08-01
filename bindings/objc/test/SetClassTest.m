//
//  TestLocal.m
//  ObjCSword
//
//  Created by Manfred Bergmann on 12.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import "SetClassTest.h"

@implementation Test

- (void)sayHello {
    NSLog(@"TestLocal");
}

- (void)setClass:(Class<TestLocal>)classImpl {
    object_setClass(self, classImpl);
}

@end
