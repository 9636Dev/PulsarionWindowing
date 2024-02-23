#pragma once

#include <Cocoa/Cocoa.h>
#include <memory>

#include "Common.hpp"

@interface PulsarionAppDelegate : NSObject<NSApplicationDelegate>
{
    std::shared_ptr<Pulsarion::Windowing::CocoaAppState> appState;
}

- (instancetype)initWithAppState:(std::shared_ptr<Pulsarion::Windowing::CocoaAppState>)appState;
@end

