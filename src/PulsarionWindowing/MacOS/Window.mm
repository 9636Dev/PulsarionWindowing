#include "Window.hpp"

#include <Cocoa/Cocoa.h>
#include <vector>
#include <utility>
#include <memory>
#include <functional>

namespace Pulsarion::Windowing
{
    static bool DefaultWindowCloseCallback() { return true; }
    static void DefaultVoidFunction() {}
    template<typename T>
    static void DefaultFunction(T _) {}

    struct CocoaWindowState
    {
        bool CloseRequested = false; // This is when the user clicks the close button or Cmd+Q
        void* UserData = nullptr;

        // --- Events ---

        std::function<bool()> OnClose = DefaultWindowCloseCallback;
        std::function<void()> OnWindowCreated = DefaultVoidFunction;


        CocoaWindowState() = default;
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
    for (auto& window : Pulsarion::Windowing::s_Windows)
    {
        if (window.first != sender)
            continue;
        window.second->CloseRequested = window.second->OnClose();
    }
    return FALSE;
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

                constexpr auto Default = WindowCreationData::DefaultX;
                CGFloat x = creationData.X == Default ? 0 : static_cast<CGFloat>(creationData.X);
                CGFloat y = creationData.Y == Default ? 0 : static_cast<CGFloat>(creationData.Y);
                CGFloat width = creationData.Width == Default ? 1280 : static_cast<CGFloat>(creationData.Width);
                CGFloat height = creationData.Height == Default ? 720 : static_cast<CGFloat>(creationData.Height);

                NSRect frame = NSMakeRect(x, y, width, height);
                m_WindowDelegate = [[PulsarionWindowDelegate alloc] init];
                m_Window = [[NSWindow alloc] initWithContentRect:frame styleMask:styles backing:NSBackingStoreBuffered defer:NO];
                [m_Window setDelegate:m_WindowDelegate];

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

        inline void SetTitle(const std::string& title) const
        {
            @autoreleasepool {
                [m_Window setTitle:[NSString stringWithUTF8String:title.c_str()]];
            }
        }

        inline void PollEvents() const
        {
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
    }

    void CocoaWindow::PollEvents()
    {
        m_Impl->PollEvents();
    }

    bool CocoaWindow::ShouldClose() const
    {
        return m_Impl->m_State->CloseRequested;
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
