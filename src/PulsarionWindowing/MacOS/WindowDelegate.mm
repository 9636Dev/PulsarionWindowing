#include "WindowDelegate.h"

#include "NativeWindow.h"

@implementation PulsarionWindowDelegate

- (instancetype)initWithState:(std::shared_ptr<Pulsarion::Windowing::CocoaWindowState>)state
{
    self = [super init];
    if (self)
        m_State = state;
    return self;
}

- (BOOL)windowShouldClose:(id)sender {
    if (m_State->OnClose)
        m_State->CloseRequested = m_State->OnClose(m_State->UserData);
    else
        m_State->CloseRequested = true;

    return FALSE;
}

- (void)windowDidResize:(NSNotification *)notification {
    #ifdef PULSARION_WINDOWING_LIMIT_EVENTS
    if (m_State->LimitEvents)
    {
        if ((m_State->LimitedEvents & m_State->WINDOW_RESIZE_EVENT) == 0)
            return;
        m_State->LimitedEvents |= m_State->WINDOW_RESIZE_EVENT;
    }
    #endif

    if (m_State->OnResize)
    {
        auto frame = [[notification object] frame];
        m_State->OnResize(m_State->UserData, static_cast<std::uint32_t>(frame.size.width), static_cast<std::uint32_t>(frame.size.height));
    }
}

- (void)windowWillStartLiveResize:(NSNotification *)notification {
    if (m_State->BeforeResize)
    {
        NSRect frame = [[notification object] frame];
        m_State->BeforeResize(m_State->UserData);
    }
}

- (void)windowDidBecomeMain:(NSNotification *)notification {
    if (m_State->OnFocus)
        m_State->OnFocus(m_State->UserData, true);
}

- (void)windowDidResignMain:(NSNotification *)notification {
    if (m_State->OnFocus)
        m_State->OnFocus(m_State->UserData, false);
}


- (void)windowDidMove:(NSNotification *)notification {
    if (m_State->OnMove)
    {
        NSRect frame = [[notification object] frame];
        m_State->OnMove(m_State->UserData, static_cast<std::uint32_t>(frame.origin.x), static_cast<std::uint32_t>(frame.origin.y));
    }
}

- (void)windowDidMiniaturize:(NSNotification *)notification {
    if (m_State->OnMinimize)
        m_State->OnMinimize(m_State->UserData);
}

- (void)windowDidDeminiaturize:(NSNotification *)notification {
    if (m_State->OnRestore)
        m_State->OnRestore(m_State->UserData);
}

- (void)windowDidMaximize:(NSNotification *)notification {
    if (m_State->OnMaximize)
        m_State->OnMaximize(m_State->UserData);
}

- (void)windowDidDemaximize:(NSNotification *)notification {
    if (m_State->OnRestore)
        m_State->OnRestore(m_State->UserData);
}

- (void)windowDidEnterFullScreen:(NSNotification *)notification {
    if (m_State->OnFullscreen)
        m_State->OnFullscreen(m_State->UserData, true);
}

- (void)windowDidExitFullScreen:(NSNotification *)notification {
    if (m_State->OnFullscreen)
        m_State->OnFullscreen(m_State->UserData, false);
}
@end
