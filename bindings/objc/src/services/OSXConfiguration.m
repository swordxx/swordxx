//
//  OSXConfiguration.m
//  ObjCSword
//
//  Created by Manfred Bergmann on 12.06.10.
//  Copyright 2010 Software by MABE. All rights reserved.
//

#import "OSXConfiguration.h"


@implementation OSXConfiguration

- (NSString *)osVersion {
    return [[NSDictionary dictionaryWithContentsOfFile:@"/System/Library/CoreServices/SystemVersion.plist"] objectForKey:@"ProductVersion"];
}

- (NSString *)bundleVersion {
    return (NSString *)CFBundleGetValueForInfoDictionaryKey(CFBundleGetMainBundle(), kCFBundleVersionKey);
}

- (NSString *)defaultModulePath {
    return [@"~/Library/Application Support/Sword" stringByExpandingTildeInPath];
}

- (NSString *)defaultAppSupportPath {
    return [@"~/Library/Application Support/MacSword" stringByExpandingTildeInPath];
}

- (NSString *)tempFolder {
    return [@"~/Library/Caches/ObjCSword" stringByExpandingTildeInPath];
}

- (NSString *)logFile {
    return [@"~/Library/Logs/ObjCSword.log" stringByExpandingTildeInPath];
}

@end
