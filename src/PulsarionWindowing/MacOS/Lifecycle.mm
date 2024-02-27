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

#ifdef PULSARION_WINDOWING_AUTO_MANAGE_LIFECYCLE
    Lifecycle::Lifecycle::Lifecycle()
    {
        [PulsarionApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp activateIgnoringOtherApps:YES];
        s_AppDelegate = [[PulsarionAppDelegate alloc] init];
        [NSApp setDelegate:s_AppDelegate];
    }

    Lifecycle::Lifecycle::~Lifecycle()
    {
        Destroy();
    }

    //NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    std::unique_ptr<Lifecycle::Lifecycle> Lifecycle::Lifecycle::s_Instance = nullptr;
#endif


    bool Lifecycle::Initialize()
    {
        if (Lifecycle::s_Instance != nullptr)
            return false;

        Lifecycle::s_Instance = std::unique_ptr<Lifecycle>(new Lifecycle());
        return true;
    }

    void Lifecycle::Destroy()
    {
        [NSApp stop:nil];
    }
}

