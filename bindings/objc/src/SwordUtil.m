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

@end