#include "View.h"

@implementation PulsarionView

- (instancetype)initWithState:(std::shared_ptr<Pulsarion::Windowing::CocoaWindowState>)initState
{
    self = [super init];
    if (self)
        state = initState;
    return self;
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)canBecomeKeyView {
    return YES;
}

- (BOOL)acceptsFirstMouse {
    return YES;
}

- (void)mouseEntered:(NSEvent *)event {
    state->OnMouseEnter(state->UserData);
}

- (void)mouseDown:(NSEvent *)event {
}

@end
