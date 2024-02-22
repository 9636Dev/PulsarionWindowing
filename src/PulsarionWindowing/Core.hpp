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
#define PULSARION_WINDOWING_LIMIT_EVENTS // Limit the events without side effects to once per PollEvents call
#ifdef PULSARION_PLATFORM_WINDOWS
#define PULSARION_WINDOWING_USE_HIGH_RES_SLEEP // Use high resolution sleep for high frame accuracy by windows
#endif
#define PULSARION_WINDOWING_USE_BUSY_WAIT // Use busy wait instead of sleeping for high frame accuracy for last ~3ms.
#endif
