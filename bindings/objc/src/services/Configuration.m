//
//  Configuration.m
//  ObjCSword
//
//  Created by Manfred Bergmann on 13.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import "Configuration.h"

@implementation Configuration

+ (Configuration *)config {
    static Configuration *instance;
    if(instance == nil) {
        instance = [[Configuration alloc] init];
    }
    return instance;
}

- (id)init {
    return [super init];
}

- (void)finalize {
    [super finalize];
}

- (void)dealloc {
    [super dealloc];
}

- (void)setClass:(Class<Configuration>)classImpl {
    object_setClass(self, classImpl);
}

#pragma mark Configuration implementation

- (NSString *)osVersion {return @"";}
- (NSString *)bundleVersion {return @"";}
- (NSString *)defaultModulePath {return @"";}
- (NSString *)defaultAppSupportPath {return @"";}
- (NSString *)tempFolder {return @"";}
- (NSString *)logFile {return @"";}

@end
