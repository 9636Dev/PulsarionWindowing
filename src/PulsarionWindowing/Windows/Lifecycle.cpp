#include "../LifeCycle.hpp"

namespace Pulsarion::Windowing
{
    Lifecycle::Lifecycle::Lifecycle()
    {

    }

    Lifecycle::Lifecycle::~Lifecycle()
    {

    }

    bool Lifecycle::Initialize()
    {
        bool initialized = false;
        if (initialized)
            return false;
        initialized = true;
        return true;
    }

    void Lifecycle::Destroy()
    {

    }
}
