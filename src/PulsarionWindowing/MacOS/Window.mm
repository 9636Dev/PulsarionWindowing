#include "Window.hpp"

#include "PulsarionCore/Assert.hpp"

#include "../Lifecycle.hpp"

#include "Common.hpp"
#include "AppDelegate.h"
#include "PulsarionWindowing/MacOS/Application.h"
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
        PulsarionWindowDelegate* m_WindowDelegate;
        PulsarionView* m_View;
        std::shared_ptr<CocoaWindowState> m_State;

        inline explicit Impl(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config)
            : m_State(std::make_shared<CocoaWindowState>())
        {
            @autoreleasepool {
                bool initialized = Lifecycle::Initialize();

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
                NSTrackingAreaOptions options = NSTrackingActiveInKeyWindow | NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved;

                [m_Window setDelegate:m_WindowDelegate];
                [m_Window setContentView:m_View];

                SetTitle(title);
                if (config.StartVisible)
                    SetVisible(true);

                if (initialized) // First time running or the app was terminated
                    [NSApp run];
            }
        }

        ~Impl()
        {
            @autoreleasepool {
                if (m_Window != nil) {
                    [m_Window close];
                    [m_Window setDelegate:nil];
                }
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

        inline void SetCursorMode(CursorMode mode) const
        {
            static CursorMode currentMode = CursorMode::Normal;
            if (mode == currentMode)
                return;
            currentMode = mode;
            @autoreleasepool {
                if (mode == CursorMode::Normal)
                    [NSCursor unhide];
                else
                    [NSCursor hide];
            }
        }

        inline void PollEvents() const
        {
            #ifdef PULSARION_WINDOWING_LIMIT_EVENTS
            m_State->LimitedEvents = 0;
            #endif
            @autoreleasepool {
                bool appClosed = [NSApp isKindOfClass:[PulsarionApplication class]] && static_cast<PulsarionApplication*>(NSApp).IsCloseRequested;
                NSLog(@"App closed: %d", appClosed);

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
        PULSARION_ASSERT([NSApp isKindOfClass:[PulsarionApplication class]], "NSApp is not of type PulsarionApplication");
        return static_cast<PulsarionApplication*>(NSApp).IsCloseRequested || m_Impl->m_State->CloseRequested;
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

#ifdef PULSARION_WINDOWING_LIMIT_EVENTS
    void CocoaWindow::LimitEvents(bool limited)
    {
        m_Impl->m_State->LimitEvents = limited;
    }

    bool CocoaWindow::IsLimitingEvents() const
    {
        return m_Impl->m_State->LimitEvents;
    }
#endif

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

    void CocoaWindow::SetOnFullscreen(Window::FullscreenCallback&& callback)
    {
        m_Impl->m_State->OnFullscreen = std::move(callback);
    }

    Window::FullscreenCallback CocoaWindow::GetOnFullscreen() const
    {
        return m_Impl->m_State->OnFullscreen;
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

    void CocoaWindow::SetOnMouseLeave(Window::MouseLeaveCallback&& callback)
    {
        m_Impl->m_State->OnMouseLeave = std::move(callback);
    }

    Window::MouseLeaveCallback CocoaWindow::GetOnMouseLeave() const
    {
        return m_Impl->m_State->OnMouseLeave;
    }

    void CocoaWindow::SetOnMouseDown(Window::MouseDownCallback&& callback)
    {
        m_Impl->m_State->OnMouseDown = std::move(callback);
    }

    Window::MouseDownCallback CocoaWindow::GetOnMouseDown() const
    {
        return m_Impl->m_State->OnMouseDown;
    }

    void CocoaWindow::SetOnMouseUp(Window::MouseUpCallback&& callback)
    {
        m_Impl->m_State->OnMouseUp = std::move(callback);
    }

    Window::MouseUpCallback CocoaWindow::GetOnMouseUp() const
    {
        return m_Impl->m_State->OnMouseUp;
    }

    void CocoaWindow::SetOnMouseMove(Window::MouseMoveCallback&& callback)
    {
        m_Impl->m_State->OnMouseMove = std::move(callback);
    }

    Window::MouseMoveCallback CocoaWindow::GetOnMouseMove() const
    {
        return m_Impl->m_State->OnMouseMove;
    }

    void CocoaWindow::SetOnMouseWheel(Window::MouseWheelCallback&& callback)
    {
        m_Impl->m_State->OnMouseWheel = std::move(callback);
    }

    Window::MouseWheelCallback CocoaWindow::GetOnMouseWheel() const
    {
        return m_Impl->m_State->OnMouseWheel;
    }

    void CocoaWindow::SetOnKeyDown(Window::KeyDownCallback&& callback)
    {
        m_Impl->m_State->OnKeyDown = std::move(callback);
    }

    Window::KeyDownCallback CocoaWindow::GetOnKeyDown() const
    {
        return m_Impl->m_State->OnKeyDown;
    }

    void CocoaWindow::SetOnKeyUp(Window::KeyUpCallback&& callback)
    {
        m_Impl->m_State->OnKeyUp = std::move(callback);
    }

    Window::KeyUpCallback CocoaWindow::GetOnKeyUp() const
    {
        return m_Impl->m_State->OnKeyUp;
    }

    void CocoaWindow::SetOnKeyTyped(Window::KeyTypedCallback&& callback)
    {
        m_Impl->m_State->OnKeyTyped = std::move(callback);
    }

    Window::KeyTypedCallback CocoaWindow::GetOnKeyTyped() const
    {
        return m_Impl->m_State->OnKeyTyped;
    }

    void CocoaWindow::SetUserData(void* userData)
    {
        m_Impl->m_State->UserData = userData;
    }

    void* CocoaWindow::GetUserData() const
    {
        return m_Impl->m_State->UserData;
    }

    void CocoaWindow::SetCursorMode(CursorMode mode)
    {
        m_Impl->SetCursorMode(mode);
    }


    std::shared_ptr<Window> CreateSharedWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config, std::optional<WindowEvents> events)
    {
        auto window = std::make_shared<CocoaWindow>(std::move(title), bounds, styles, config);
        if (events.has_value())
            SetWindowEvents(*window, *events);
        return window;
    }

    std::unique_ptr<Window> CreateUniqueWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config, std::optional<WindowEvents> events)
    {
        auto window = std::make_unique<CocoaWindow>(std::move(title), bounds, styles, config);
        if (events.has_value())
            SetWindowEvents(*window, *events);
        return window;
    }
}
