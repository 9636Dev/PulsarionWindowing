#include "../Lifecycle.hpp"

#include "PulsarionWindowing/MacOS/AppDelegate.h"
#include "Window.hpp"
#include "Application.h"
#include "PulsarionCore/Assert.hpp"

#include <Cocoa/Cocoa.h>

namespace Pulsarion::Windowing
{
    //NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static PulsarionAppDelegate* s_AppDelegate = nullptr;


    static void _Init()
    {
        [PulsarionApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp activateIgnoringOtherApps:YES];
        s_AppDelegate = [[PulsarionAppDelegate alloc] init];
        [NSApp setDelegate:s_AppDelegate];
    }

#ifdef PULSARION_WINDOWING_AUTO_MANAGE_LIFECYCLE
    Lifecycle::Lifecycle::Lifecycle()
    {
        _Init();
    }

    Lifecycle::Lifecycle::~Lifecycle()
    {
        Destroy();
    }

    //NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    std::unique_ptr<Lifecycle::Lifecycle> Lifecycle::Lifecycle::s_Instance = nullptr;
#else
    static bool s_IsInitialized = false;
#endif


    bool Lifecycle::Initialize()
    {
    #ifdef PULSARION_WINDOWING_AUTO_MANAGE_LIFECYCLE
        if (Lifecycle::s_Instance != nullptr)
            return false;

        Lifecycle::s_Instance = std::unique_ptr<Lifecycle>(new Lifecycle());
        return true;
    #else
        if (s_IsInitialized)
            return false;
        s_IsInitialized = true;

        _Init();
        return true;
    #endif
    }

    void Lifecycle::Destroy()
    {
        if (s_AppDelegate == nullptr)
            return;
        // We don't have ARC, so we need to manually release the AppDelegate
        [NSApp setDelegate:nil];
        s_AppDelegate = nil;
        [NSApp stop:nil];
    }
}

