//
// Created by mbergmann on 18.12.12.
//
//


#import "FilterProviderFactory.h"
#import "DefaultFilterProvider.h"

@interface FilterProviderFactory ()

@property(nonatomic, retain) id <FilterProvider> filterProvider;

@end

@implementation FilterProviderFactory

+ (FilterProviderFactory *)providerFactory {
    static FilterProviderFactory *singleton = nil;

    if(singleton == nil) {
        singleton = [[FilterProviderFactory alloc] init];
    }

    return singleton;
}

- (void)initWithImpl:(id <FilterProvider>)aFilterProvider {
    self.filterProvider = aFilterProvider;
}

- (id <FilterProvider>)get {
    return self.filterProvider;
}

- (void)dealloc {
    self.filterProvider = nil;
    [super dealloc];
}


@end