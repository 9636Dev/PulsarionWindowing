#pragma once

#include "Common.hpp"
#include "View.h"

#include <Cocoa/Cocoa.h>
#include <memory>

@interface PulsarionWindow : NSWindow<NSWindowDelegate>
{
    @public std::shared_ptr<Pulsarion::Windowing::CocoaWindowState> m_State;
}

- (instancetype)initWithContentRect:(NSRect)contentRect styleMask:(NSWindowStyleMask)style backing:(NSBackingStoreType)backingStoreType defer:(BOOL)flag state:(std::shared_ptr<Pulsarion::Windowing::CocoaWindowState>)state;
@end
