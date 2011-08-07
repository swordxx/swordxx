//
//  SwordModuleTest.h
//  ObjCSword
//
//  Created by Manfred Bergmann on 14.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>

@class SwordModule, SwordManager;

@interface SwordModuleTest : SenTestCase {
    SwordManager *mgr;
    SwordModule *mod;
}

@end
