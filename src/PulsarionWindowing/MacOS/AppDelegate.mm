#include "AppDelegate.h"

@implementation PulsarionAppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    [NSApp stop:nil]; // We want to handle the event loop ourselves
}
@end
