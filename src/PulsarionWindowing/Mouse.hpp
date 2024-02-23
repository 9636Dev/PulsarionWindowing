#pragma once

#include "Core.hpp"

namespace Pulsarion::Windowing
{
    struct Point
    {
        float x;
        float y;
    };
    using ScrollOffset = Point;

    enum class MouseCode : std::uint8_t
    {
        Button0 = 0,
        Button1 = 1,
        Button2 = 2,
        Button3 = 3,
        Button4 = 4,
        Button5 = 5,
        Button6 = 6,
        Button7 = 7,
        Unknown = 255,

        ButtonLeft = Button0,
        ButtonRight = Button1,
        ButtonMiddle = Button2
    };
}
