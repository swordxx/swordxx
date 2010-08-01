//
//  SetClassTest.h
//  ObjCSword
//
//  Created by Manfred Bergmann on 12.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@protocol TestLocal
- (void)sayHello;
@end

@interface Test : NSObject <TestLocal> {
}

- (void)sayHello;
- (void)setClass:(Class<TestLocal>)classImpl;

@end
