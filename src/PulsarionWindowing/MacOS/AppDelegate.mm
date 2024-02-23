#include "AppDelegate.h"

@implementation PulsarionAppDelegate
- (instancetype)initWithAppState:(std::shared_ptr<Pulsarion::Windowing::CocoaAppState>)initAppState {
    self = [super init];
    if (self) {
        self->appState = initAppState;
    }
    return self;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    [NSApp stop:nil]; // We want to handle the event loop ourselves
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // We currently don't do anything here
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
    appState->ShouldStop = true;
    appState->OnClose();
    return NSTerminateCancel; // Only terminate app when destructor is called, maybe for fail-safe we add a timeout
}
@end
