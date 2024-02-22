#pragma once

#include <Cocoa/Cocoa.h>
#include <memory>

#include "Common.hpp"

@interface PulsarionWindowDelegate : NSObject <NSWindowDelegate>
{
    std::shared_ptr<Pulsarion::Windowing::CocoaWindowState> m_State;
}

- (instancetype)initWithState:(std::shared_ptr<Pulsarion::Windowing::CocoaWindowState>)state;
@end

