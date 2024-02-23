#include "View.h"
#include "PulsarionWindowing/Mouse.hpp"

static Pulsarion::Windowing::MouseCode GetMouseCode(NSUInteger buttonNumber)
{
    if (buttonNumber <= 7)
        return static_cast<Pulsarion::Windowing::MouseCode>(buttonNumber);
    return Pulsarion::Windowing::MouseCode::Unknown;
}

@implementation PulsarionView

- (instancetype)initWithState:(std::shared_ptr<Pulsarion::Windowing::CocoaWindowState>)initState
{
    self = [super init];
    if (!self)
        return nil;

    state = initState;
    NSTrackingAreaOptions options = NSTrackingActiveInKeyWindow | NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved;
    trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:options owner:self userInfo:nil];
    [self addTrackingArea:trackingArea];
    return self;
}

- (void)viewWillMoveToWindow:(NSWindow *)newWindow {
    [super viewWillMoveToWindow:newWindow];
    if (newWindow) {
        [self updateTrackingAreas];
    }
}

- (void)updateTrackingAreas {
    [super updateTrackingAreas];
    [self removeTrackingArea:trackingArea];
    [trackingArea release];
    trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:NSTrackingActiveInKeyWindow | NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved owner:self userInfo:nil];
    [self addTrackingArea:trackingArea];
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
    if (state->OnMouseEnter)
        state->OnMouseEnter(state->UserData);
}

- (void)mouseExited:(NSEvent *)event {
    if (state->OnMouseLeave)
        state->OnMouseLeave(state->UserData);
}

- (void)mouseDown:(NSEvent *)event {
    if (state->OnMouseDown)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseDown(state->UserData, point, Pulsarion::Windowing::MouseCode::Button0);
    }
}

- (void)mouseUp:(NSEvent *)event {
    if (state->OnMouseUp)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseUp(state->UserData, point, Pulsarion::Windowing::MouseCode::Button0);
    }
}

- (void)rightMouseDown:(NSEvent *)event {
    if (state->OnMouseDown)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseDown(state->UserData, point, Pulsarion::Windowing::MouseCode::Button1);
    }
}

- (void)rightMouseUp:(NSEvent *)event {
    if (state->OnMouseUp)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseUp(state->UserData, point, Pulsarion::Windowing::MouseCode::Button1);
    }
}

- (void)otherMouseDown:(NSEvent *)event {
    if (state->OnMouseDown)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseDown(state->UserData, point, GetMouseCode(event.buttonNumber));
    }
}

- (void)otherMouseUp:(NSEvent *)event {
    if (state->OnMouseUp)
    {
         Pulsarion::Windowing::Point point = { static_cast<float>(event.locationInWindow.x), static_cast<float>(event.locationInWindow.y) };
         state->OnMouseUp(state->UserData, point, GetMouseCode(event.buttonNumber));
    }
}


@end
