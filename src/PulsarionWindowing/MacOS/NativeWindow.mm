#include "NativeWindow.h"

@implementation PulsarionWindow
- (id)initWithContentRect:(NSRect)contentRect styleMask:(NSWindowStyleMask)style backing:(NSBackingStoreType)backingStoreType defer:(BOOL)deferCreation state:(std::shared_ptr<Pulsarion::Windowing::CocoaWindowState>)state {
    self = [super initWithContentRect:contentRect styleMask:style backing:backingStoreType defer:deferCreation];
    m_State = state;
    return self;
}

- (BOOL)canBecomeKeyWindow {
    return YES;
}

- (BOOL)canBecomeMainWindow {
    return YES;
}

@end
