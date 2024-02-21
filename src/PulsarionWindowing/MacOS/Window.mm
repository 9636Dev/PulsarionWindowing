#include "Window.hpp"

#include <Cocoa/Cocoa.h>
#include <vector>
#include <utility>
#include <memory>
#include <functional>

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

    //NOLINTNEXTLINE We need to keep track of the windows, so it has to be non-const and global
    static std::vector<std::pair<NSWindow*, std::shared_ptr<CocoaWindowState>>> s_Windows;


}

@interface PulsarionAppDelegate : NSObject<NSApplicationDelegate>
@end

@implementation PulsarionAppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    [NSApp stop:nil]; // We want to handle the event loop ourselves
}
@end

@interface PulsarionWindowDelegate : NSObject<NSWindowDelegate>
@end

@implementation PulsarionWindowDelegate
- (BOOL)windowShouldClose:(id)sender {
    auto window = std::find_if(Pulsarion::Windowing::s_Windows.begin(), Pulsarion::Windowing::s_Windows.end(), [sender](const auto& pair) { return pair.first == sender; });
    if (window == Pulsarion::Windowing::s_Windows.end())
        return TRUE; // If there is no window, we should close it

    if (window->second->OnClose)
        window->second->CloseRequested = window->second->OnClose(window->second->UserData);
    else
        window->second->CloseRequested = TRUE;

    return FALSE;
}

- (void)windowDidResize:(NSNotification *)notification {
    auto window = std::find_if(Pulsarion::Windowing::s_Windows.begin(), Pulsarion::Windowing::s_Windows.end(), [notification](const auto& pair) { return pair.first == [notification object]; });
    if (window == Pulsarion::Windowing::s_Windows.end())
        return;

    #ifdef PULSARION_WINDOWING_LIMIT_EVENTS
    if (window->second->LimitEvents)
    {
        if ((window->second->LimitedEvents & window->second->WINDOW_RESIZE_EVENT) == 0)
            return;
        window->second->LimitedEvents |= window->second->WINDOW_RESIZE_EVENT;
    }
    #endif

    if (window->second->OnResize)
    {
        NSRect frame = [[notification object] frame];
        window->second->OnResize(window->second->UserData, static_cast<std::uint32_t>(frame.size.width), static_cast<std::uint32_t>(frame.size.height));
    }
}

- (void)windowWillStartLiveResize:(NSNotification *)notification {
    auto window = std::find_if(Pulsarion::Windowing::s_Windows.begin(), Pulsarion::Windowing::s_Windows.end(), [notification](const auto& pair) { return pair.first == [notification object]; });
    if (window == Pulsarion::Windowing::s_Windows.end())
        return;

    if (window->second->BeforeResize)
    {
        NSRect frame = [[notification object] frame];
        window->second->BeforeResize(window->second->UserData);
    }
}

- (void)windowDidBecomeMain:(NSNotification *)notification {
    auto window = std::find_if(Pulsarion::Windowing::s_Windows.begin(), Pulsarion::Windowing::s_Windows.end(), [notification](const auto& pair) { return pair.first == [notification object]; });
    if (window == Pulsarion::Windowing::s_Windows.end())
        return;

    if (window->second->OnFocus)
        window->second->OnFocus(window->second->UserData, true);
}

- (void)windowDidResignMain:(NSNotification *)notification {
    auto window = std::find_if(Pulsarion::Windowing::s_Windows.begin(), Pulsarion::Windowing::s_Windows.end(), [notification](const auto& pair) { return pair.first == [notification object]; });
    if (window == Pulsarion::Windowing::s_Windows.end())
        return;

    if (window->second->OnFocus)
        window->second->OnFocus(window->second->UserData, false);
}


- (void)windowDidMove:(NSNotification *)notification {
    auto window = std::find_if(Pulsarion::Windowing::s_Windows.begin(), Pulsarion::Windowing::s_Windows.end(), [notification](const auto& pair) { return pair.first == [notification object]; });
    if (window == Pulsarion::Windowing::s_Windows.end())
        return;

    if (window->second->OnMove)
    {
        NSRect frame = [[notification object] frame];
        window->second->OnMove(window->second->UserData, static_cast<std::uint32_t>(frame.origin.x), static_cast<std::uint32_t>(frame.origin.y));
    }
}
@end

namespace Pulsarion::Windowing
{
    class CocoaWindow::Impl
    {
    public:
        NSWindow* m_Window;
        PulsarionAppDelegate* m_AppDelegate;
        PulsarionWindowDelegate* m_WindowDelegate;
        std::shared_ptr<CocoaWindowState> m_State;

        inline explicit Impl(WindowCreationData& creationData)
            : m_State(std::make_shared<CocoaWindowState>())
        {
            @autoreleasepool {
                [NSApplication sharedApplication];
                m_AppDelegate = [[PulsarionAppDelegate alloc] init];
                [[NSApplication sharedApplication] setDelegate:m_AppDelegate];


                NSUInteger styles = 0;
                if (HasFlag(creationData.Flags, WindowFlags::Caption))
                    styles |= NSWindowStyleMaskTitled;
                if (HasFlag(creationData.Flags, WindowFlags::SysMenu))
                    styles |= NSWindowStyleMaskClosable;
                if (HasFlag(creationData.Flags, WindowFlags::MinimizeButton))
                    styles |= NSWindowStyleMaskMiniaturizable;
                if (HasFlag(creationData.Flags, WindowFlags::MaximizeButton))
                    styles |= NSWindowStyleMaskResizable;
                if (HasFlag(creationData.Flags, WindowFlags::Resizable))
                    styles |= NSWindowStyleMaskResizable;

                CGFloat x = creationData.X == WindowCreationData::Default ? 0 : static_cast<CGFloat>(creationData.X);
                CGFloat y = creationData.Y == WindowCreationData::Default ? 0 : static_cast<CGFloat>(creationData.Y);
                CGFloat width = creationData.Width == WindowCreationData::Default ? 1280 : static_cast<CGFloat>(creationData.Width);
                CGFloat height = creationData.Height == WindowCreationData::Default ? 720 : static_cast<CGFloat>(creationData.Height);

                NSRect frame = NSMakeRect(x, y, width, height);
                m_WindowDelegate = [[PulsarionWindowDelegate alloc] init];
                m_Window = [[NSWindow alloc] initWithContentRect:frame styleMask:styles backing:NSBackingStoreBuffered defer:NO];
                [m_Window setDelegate:m_WindowDelegate];

                if (HasFlag(creationData.Flags, WindowFlags::AlwaysOnTop))
                    [m_Window setLevel:NSFloatingWindowLevel];

                s_Windows.emplace_back(m_Window,  m_State);
                SetTitle(creationData.Title);

                if (HasFlag(creationData.Flags, WindowFlags::Visible))
                    [m_Window makeKeyAndOrderFront:nil];

                if (![[NSRunningApplication currentApplication] isFinishedLaunching])
                    [NSApp run];
            }
        }

        ~Impl()
        {
            @autoreleasepool {
                [m_Window close];
                s_Windows.erase(std::remove_if(s_Windows.begin(), s_Windows.end(), [this](const auto& pair) { return pair.first == m_Window; }), s_Windows.end());
            }
        }

        inline void SetVisible(bool visible) const
        {
            if (visible == [m_Window isVisible])
                return;
            @autoreleasepool {
                if (visible)
                    [m_Window makeKeyAndOrderFront:nil];
                else
                    [m_Window orderOut:nil];

                if (m_State->OnWindowVisibility)
                    m_State->OnWindowVisibility(m_State->UserData, visible);
            }
        }

        inline void SetTitle(const std::string& title) const
        {
            @autoreleasepool {
                [m_Window setTitle:[NSString stringWithUTF8String:title.c_str()]];
            }
        }

        [[nodiscard]] inline std::string GetTitle() const
        {
            @autoreleasepool {
                return [[m_Window title] UTF8String];
            }
        }

        inline void PollEvents() const
        {
            #ifdef PULSARION_WINDOWING_LIMIT_EVENTS
            m_State->LimitedEvents = 0;
            #endif
            @autoreleasepool {
                NSEvent* event;
                do
                {
                    event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
                    if (event)
                        [NSApp sendEvent:event];
                } while (event);
            }
        }
    };


    CocoaWindow::CocoaWindow(WindowCreationData& creationData)
        : m_Impl(new Impl(creationData))
    {
    }

    CocoaWindow::~CocoaWindow()
    {
        delete m_Impl;
    }

    void CocoaWindow::SetVisible(bool visible)
    {
        m_Impl->SetVisible(visible);
    }

    void CocoaWindow::PollEvents()
    {
        m_Impl->PollEvents();
    }

    bool CocoaWindow::ShouldClose() const
    {
        return m_Impl->m_State->CloseRequested;
    }

    void CocoaWindow::SetShouldClose(bool shouldClose)
    {
        m_Impl->m_State->CloseRequested = shouldClose;
    }

    void CocoaWindow::SetTitle(const std::string& title)
    {
        m_Impl->SetTitle(title);
    }

    std::optional<std::string> CocoaWindow::GetTitle() const
    {
        return m_Impl->GetTitle();
    }

    void* CocoaWindow::GetNativeWindow() const
    {
        return m_Impl->m_Window;
    }

    void CocoaWindow::LimitEvents(bool limited)
    {
        m_Impl->m_State->LimitEvents = limited;
    }

    bool CocoaWindow::IsLimitingEvents() const
    {
        return m_Impl->m_State->LimitEvents;
    }

    void CocoaWindow::SetOnClose(Window::CloseCallback&& callback)
    {
        m_Impl->m_State->OnClose = std::move(callback);
    }

    Window::CloseCallback CocoaWindow::GetOnClose() const
    {
        return m_Impl->m_State->OnClose;
    }

    void CocoaWindow::SetOnWindowVisibility(Window::VisibilityCallback&& callback)
    {
        m_Impl->m_State->OnWindowVisibility = std::move(callback);
    }

    Window::VisibilityCallback CocoaWindow::GetOnWindowVisibility() const
    {
        return m_Impl->m_State->OnWindowVisibility;
    }

    void CocoaWindow::SetOnFocus(Window::FocusCallback&& callback)
    {
        m_Impl->m_State->OnFocus = std::move(callback);
    }

    Window::FocusCallback CocoaWindow::GetOnFocus() const
    {
        return m_Impl->m_State->OnFocus;
    }

    void CocoaWindow::SetOnResize(Window::ResizeCallback&& callback)
    {
        m_Impl->m_State->OnResize = std::move(callback);
    }

    Window::ResizeCallback CocoaWindow::GetOnResize() const
    {
        return m_Impl->m_State->OnResize;
    }

    void CocoaWindow::SetOnMove(Window::MoveCallback&& callback)
    {
        m_Impl->m_State->OnMove = std::move(callback);
    }

    Window::MoveCallback CocoaWindow::GetOnMove() const
    {
        return m_Impl->m_State->OnMove;
    }

    void CocoaWindow::SetBeforeResize(Window::BeforeResizeCallback&& callback)
    {
        m_Impl->m_State->BeforeResize = std::move(callback);
    }

    Window::BeforeResizeCallback CocoaWindow::GetBeforeResize() const
    {
        return m_Impl->m_State->BeforeResize;
    }

    void CocoaWindow::SetUserData(void* userData)
    {
        m_Impl->m_State->UserData = userData;
    }

    void* CocoaWindow::GetUserData() const
    {
        return m_Impl->m_State->UserData;
    }

    std::shared_ptr<Window> CreateSharedWindow(WindowCreationData& creationData)
    {
        return std::make_shared<CocoaWindow>(creationData);
    }

    std::unique_ptr<Window> CreateUniqueWindow(WindowCreationData& creationData)
    {
        return std::make_unique<CocoaWindow>(creationData);
    }
}
