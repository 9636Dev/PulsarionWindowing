#pragma once

#include "../Window.hpp"

namespace Pulsarion::Windowing
{
    struct CocoaWindowState : WindowEvents
    {
        bool CloseRequested = false; // This is when the user clicks the close button or Cmd+Q
        void* UserData = nullptr;

        CocoaWindowState() = default;

        #ifdef PULSARION_WINDOWING_LIMIT_EVENTS
        bool LimitEvents = false;
        std::uint64_t LimitedEvents = 0; // A bitmap is much more efficient,
        constexpr static std::uint32_t WINDOW_RESIZE_EVENT = 1;
        #endif

    };

    struct CocoaAppState
    {
        bool ShouldStop = false;
        std::function<void()> OnClose = nullptr;
    };
}
