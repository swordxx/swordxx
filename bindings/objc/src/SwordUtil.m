//
// Created by mbergmann on 18.12.12.
//
// To change the template use AppCode | Preferences | File Templates.
//


#import <ObjCSword/ObjCSword.h>
#import "SwordUtil.h"


@implementation SwordUtil

+ (NSDictionary *)dictionaryFromUrl:(NSURL *)aURL {
    NSMutableDictionary *ret = [NSMutableDictionary dictionary];

    NSString *scheme = [aURL scheme];
    if([scheme isEqualToString:@"sword"]) {
        // in this case host is the module and path the reference
        [ret setObject:[aURL host] forKey:ATTRTYPE_MODULE];
        [ret setObject:[[[aURL path] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding]
                        stringByReplacingOccurrencesOfString:@"/" withString:@""]
                forKey:ATTRTYPE_VALUE];
        [ret setObject:@"scriptRef" forKey:ATTRTYPE_TYPE];
        [ret setObject:@"showRef" forKey:ATTRTYPE_ACTION];
    } else if([scheme isEqualToString:@"applewebdata"]) {
        // in this case
        NSString *path = [aURL path];
        NSString *query = [aURL query];
        if([[path lastPathComponent] isEqualToString:@"passagestudy.jsp"]) {
            NSArray *data = [query componentsSeparatedByString:@"&"];
            NSString *type = @"x";
            NSString *module = @"";
            NSString *passage = @"";
            NSString *value = @"1";
            NSString *action = @"";
            for(NSString *entry in data) {
                if([entry hasPrefix:@"type="]) {
                    type = [[entry componentsSeparatedByString:@"="] objectAtIndex:1];
                } else if([entry hasPrefix:@"module="]) {
                    module = [[entry componentsSeparatedByString:@"="] objectAtIndex:1];
                } else if([entry hasPrefix:@"passage="]) {
                    passage = [[entry componentsSeparatedByString:@"="] objectAtIndex:1];
                } else if([entry hasPrefix:@"action="]) {
                    action = [[entry componentsSeparatedByString:@"="] objectAtIndex:1];
                } else if([entry hasPrefix:@"value="]) {
                    value = [[entry componentsSeparatedByString:@"="] objectAtIndex:1];
                } else {
                    ALog(@"Unknown parameter: %@", entry);
                }
            }

            [ret setObject:[module stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding] forKey:ATTRTYPE_MODULE];
            [ret setObject:[passage stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding] forKey:ATTRTYPE_PASSAGE];
            [ret setObject:[value stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding] forKey:ATTRTYPE_VALUE];
            [ret setObject:[action stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding] forKey:ATTRTYPE_ACTION];
            [ret setObject:[type stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding] forKey:ATTRTYPE_TYPE];
        }
    }

    return ret;
}

+ (NSArray *)padStrongsNumbers:(NSArray *)unpaddedNumbers {
    NSMutableArray *buf = [NSMutableArray array];
    for(NSString *lemma in unpaddedNumbers) {
        [buf addObjectsFromArray:[self padStrongsNumber:lemma]];
    }
    return [NSArray arrayWithArray:buf];
}

+ (NSArray *)padStrongsNumber:(NSString *)unpaddedNumber {
    NSMutableArray *buf = [NSMutableArray array];
    // Hebrew
    NSString *prefix = nil;
    if([unpaddedNumber hasPrefix:@"H"]) {
        prefix = @"H";
    }
    if([unpaddedNumber hasPrefix:@"G"]) {
        prefix = @"G";
    }
    
    if(prefix != nil) {
        // lemma may contain more codes concatenated by space
        NSArray *keys = [unpaddedNumber componentsSeparatedByString:@" "];
        for(__strong NSString *key in keys) {
            // trim
            key = [key stringByReplacingOccurrencesOfString:@" " withString:@""];
            NSArray *keyComps = [key componentsSeparatedByString:prefix];
            if(keyComps.count == 2) {
                NSString *keyValue = [self leftPadWithZero:keyComps[1] maxDigits:5];
                // add to result array
                [buf addObject:[NSString stringWithFormat:@"%@%@", prefix, keyValue]];
            }
        }
    } else {
        [buf addObject:unpaddedNumber];
    }
    
    return [NSArray arrayWithArray:buf];
}

+ (NSString *)leftPadWithZero:(NSString *)unpadded maxDigits:(NSInteger)maxDigits {
    NSString *padded = unpadded;
    if(padded.length < maxDigits) {
        NSInteger pad = maxDigits - padded.length;
        for(int i = 0;i < pad;i++) {
            padded = [NSString stringWithFormat:@"0%@", padded];
        }
    }
    return padded;
}

@end