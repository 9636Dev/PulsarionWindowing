#include "AppDelegate.h"

#include "Application.h"

@implementation PulsarionAppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSMenu *menubar = [[NSMenu alloc] init];

    // Create the application menu
    NSMenuItem *appMenuItem = [[NSMenuItem alloc] init];
    [menubar addItem:appMenuItem];
    NSApp.mainMenu = menubar;

    // Create the application menu content
    NSMenu *appMenu = [[NSMenu alloc] init];
    NSString *appName = [[NSRunningApplication currentApplication] localizedName];
    NSString *quitTitle = [NSString stringWithFormat:@"Quit %@", appName];

    // Create Quit menu item with Cmd+Q shortcut
    NSMenuItem *quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle
                                                          action:@selector(terminate:)
                                                   keyEquivalent:@"q"];
    [quitMenuItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
    [appMenu addItem:quitMenuItem];

    // Attach the application menu
    appMenuItem.submenu = appMenu;

    [NSApp stop:nil]; // We want to handle the event loop ourselves
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return NO; // We want to handle the event loop ourselves
}

- (BOOL)applicationShouldHandleReopen:(NSApplication *)sender hasVisibleWindows:(BOOL)flag {
    return YES; // We want to handle the event loop ourselves
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // We currently don't do anything here
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
    if ([sender isKindOfClass:[PulsarionApplication class]])
        ((PulsarionApplication *)sender).IsCloseRequested = true;
    return NSTerminateCancel; // Only terminate app when destructor is called, maybe for fail-safe we add a timeout
}
@end
