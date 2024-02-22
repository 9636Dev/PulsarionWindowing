#pragma once

#include <Cocoa/Cocoa.h>
#include "Common.hpp"

@interface PulsarionView: NSView

{
    std::shared_ptr<Pulsarion::Windowing::CocoaWindowState> state;
}

- (void)mouseEntered:(NSEvent *)event;

- (instancetype)initWithState:(std::shared_ptr<Pulsarion::Windowing::CocoaWindowState>)state;

@end


