#include "Window.hpp"

#include "PulsarionCore/Assert.hpp"

#include "Common.hpp"
#include "AppDelegate.h"
#include "WindowDelegate.h"
#include "NativeWindow.h"

#include <Cocoa/Cocoa.h>
#include <vector>
#include <utility>
#include <memory>
#include <functional>

namespace Pulsarion::Windowing
{
    class CocoaWindow::Impl
    {
    public:
        static CGFloat ConvertCocoaY(CGFloat y, CGFloat height)
        {
            return height - y;
        }

        PulsarionWindow* m_Window;
        PulsarionAppDelegate* m_AppDelegate;
        PulsarionWindowDelegate* m_WindowDelegate;
        PulsarionView* m_View;
        std::shared_ptr<CocoaWindowState> m_State;

        inline explicit Impl(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config)
            : m_State(std::make_shared<CocoaWindowState>())
        {
            @autoreleasepool {
                [NSApplication sharedApplication];
                m_AppDelegate = [[PulsarionAppDelegate alloc] init];
                [[NSApplication sharedApplication] setDelegate:m_AppDelegate];

                NSUInteger styleMask = 0;
                if (HasFlag(styles, WindowStyles::NSTitled))
                    styleMask |= NSWindowStyleMaskTitled;
                if (HasFlag(styles, WindowStyles::NSClosable))
                    styleMask |= NSWindowStyleMaskClosable;
                if (HasFlag(styles, WindowStyles::NSMiniaturizable))
                    styleMask |= NSWindowStyleMaskMiniaturizable;
                if (HasFlag(styles, WindowStyles::NSResizable))
                    styleMask |= NSWindowStyleMaskResizable;
                if (HasFlag(styles, WindowStyles::NSBorderless))
                    styleMask |= NSWindowStyleMaskBorderless;
                // Get height of screen
                NSRect screenRect = [[NSScreen mainScreen] frame];
                NSRect frame = NSMakeRect(bounds.X, ConvertCocoaY(bounds.Y, screenRect.size.height), bounds.Width, bounds.Height);
                m_Window = [[PulsarionWindow alloc] initWithContentRect:frame styleMask:styleMask backing:NSBackingStoreBuffered defer:NO state:m_State];
                m_WindowDelegate = [[PulsarionWindowDelegate alloc] initWithState:m_State];
                m_View = [[PulsarionView alloc] initWithState:m_State];

                [m_Window setDelegate:m_WindowDelegate];
                [m_Window setContentView:m_View];

                //if (HasFlag(creationData.Flags, WindowStyles::AlwaysOnTop))
                //    [m_Window setLevel:NSFloatingWindowLevel];
                SetTitle(title);

                //if (HasFlag(creationData.Flags, WindowStyles::Visible))
                //    [m_Window makeKeyAndOrderFront:nil];

                if (![[NSRunningApplication currentApplication] isFinishedLaunching])
                    [NSApp run];
            }
        }

        ~Impl()
        {
            @autoreleasepool {
                if (m_Window != nil)
                    [m_Window close];
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

    CocoaWindow::CocoaWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config)
        : m_Impl(new Impl(std::move(title), bounds, styles, config))
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

    void CocoaWindow::SetOnMinimize(Window::MinimizeCallback&& callback)
    {
        m_Impl->m_State->OnMinimize = std::move(callback);
    }

    Window::MinimizeCallback CocoaWindow::GetOnMinimize() const
    {
        return m_Impl->m_State->OnMinimize;
    }

    void CocoaWindow::SetOnMaximize(Window::MaximizeCallback&& callback)
    {
        m_Impl->m_State->OnMaximize = std::move(callback);
    }

    Window::MaximizeCallback CocoaWindow::GetOnMaximize() const
    {
        return m_Impl->m_State->OnMaximize;
    }

    void CocoaWindow::SetOnRestore(Window::RestoreCallback&& callback)
    {
        m_Impl->m_State->OnRestore = std::move(callback);
    }

    Window::RestoreCallback CocoaWindow::GetOnRestore() const
    {
        return m_Impl->m_State->OnRestore;
    }

    void CocoaWindow::SetOnMouseEnter(Window::MouseEnterCallback&& callback)
    {
        m_Impl->m_State->OnMouseEnter = std::move(callback);
    }

    Window::MouseEnterCallback CocoaWindow::GetOnMouseEnter() const
    {
        return m_Impl->m_State->OnMouseEnter;
    }

    void CocoaWindow::SetUserData(void* userData)
    {
        m_Impl->m_State->UserData = userData;
    }

    void* CocoaWindow::GetUserData() const
    {
        return m_Impl->m_State->UserData;
    }


    static void SetEvents(Window& window, WindowEvents& events)
    {
        window.SetOnClose(std::move(events.OnClose));
        window.SetOnWindowVisibility(std::move(events.OnWindowVisibility));
        window.SetOnFocus(std::move(events.OnFocus));
        window.SetOnResize(std::move(events.OnResize));
        window.SetOnMove(std::move(events.OnMove));
        window.SetBeforeResize(std::move(events.BeforeResize));
        window.SetOnMinimize(std::move(events.OnMinimize));
        window.SetOnMaximize(std::move(events.OnMaximize));
        window.SetOnRestore(std::move(events.OnRestore));
        window.SetOnMouseEnter(std::move(events.OnMouseEnter));
    }

    std::shared_ptr<Window> CreateSharedWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config, std::optional<WindowEvents> events)
    {
        auto window = std::make_shared<CocoaWindow>(std::move(title), bounds, styles, config);
        if (events)
            SetEvents(*window, *events);
        return window;
    }

    std::unique_ptr<Window> CreateUniqueWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config, std::optional<WindowEvents> events)
    {
        auto window = std::make_unique<CocoaWindow>(std::move(title), bounds, styles, config);
        if (events)
            SetEvents(*window, *events);
        return window;
    }
}
