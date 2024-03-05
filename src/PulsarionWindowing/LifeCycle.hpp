#pragma once

#include "PulsarionWindowing/Core.hpp"

#include <memory>

namespace Pulsarion::Windowing::Lifecycle
{
#ifdef PULSARION_WINDOWING_AUTO_MANAGE_LIFECYCLE
    struct PULSARION_WINDOWING_API Lifecycle
    {
    private:
        PULSARION_WINDOWING_API friend bool Initialize();
        PULSARION_WINDOWING_API friend void Destroy();
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        static std::unique_ptr<Lifecycle> s_Instance;
        Lifecycle();
    public:
        ~Lifecycle();
    };
#endif
    PULSARION_WINDOWING_API bool Initialize();
    PULSARION_WINDOWING_API void Destroy();;

}
