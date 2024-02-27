#pragma once

#include "PulsarionCore/Core.hpp"

#ifdef PULSARION_WINDOWING_BUILD_DLL
#define PULSARION_WINDOWING_API PULSARION_DLL_EXPORT
#else
#define PULSARION_WINDOWING_API PULSARION_DLL_IMPORT
#endif

#ifndef PULSARION_WINDOWING_NO_ADDITIONAL_FEATURES
#define PULSARION_WINDOWING_USE_ADDITIONAL_FEATURES
#endif

#ifdef PULSARION_WINDOWING_USE_ADDITIONAL_FEATURES
// Limit the events without side effects to once per PollEvents call
#define PULSARION_WINDOWING_LIMIT_EVENTS
// Automatically manage the lifecycle of the window by creating a singleton static instance and the destructor is called at the end of the program
#define PULSARION_WINDOWING_AUTO_MANAGE_LIFECYCLE
#ifdef PULSARION_PLATFORM_WINDOWS
// Use high resolution sleep for high frame accuracy by windows
#define PULSARION_WINDOWING_USE_HIGH_RES_SLEEP
#endif
// Use busy wait instead of sleeping for high frame accuracy for last ~3ms.
#define PULSARION_WINDOWING_USE_BUSY_WAIT
#endif
