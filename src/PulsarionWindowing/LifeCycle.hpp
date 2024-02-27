#pragma once

#include "PulsarionWindowing/Core.hpp"

#include <memory>

namespace Pulsarion::Windowing::Lifecycle
{
#ifdef PULSARION_WINDOWING_AUTO_MANAGE_LIFECYCLE
    struct PULSARION_WINDOWING_API Lifecycle
    {
    private:
        friend bool Initialize();
        friend void Destroy();
        static std::unique_ptr<Lifecycle> s_Instance;
        Lifecycle();
    public:
        ~Lifecycle();
    };
#endif
    bool Initialize();
    void Destroy();;

}
