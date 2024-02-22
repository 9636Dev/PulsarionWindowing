#pragma once

#include "Core.hpp"
#include <cstdint>

namespace Pulsarion::Windowing
{
    struct WindowBounds
    {
        std::int32_t X = 0;
        std::int32_t Y = 0;
        std::int32_t Width = 1280;
        std::int32_t Height = 720;

        WindowBounds(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height)
            : X(x), Y(y), Width(width), Height(height)
        {
        }

        WindowBounds() = default;
    };

    struct WindowConfig
    {
        bool StartVisible = true;
        bool StartFocused = true;
    };

    enum class WindowStyles : std::uint64_t
    {
        None = 0,
        NSTitled = 1 << 0,
        NSClosable = 1 << 1,
        NSMiniaturizable = 1 << 2,
        NSResizable = 1 << 3,
        NSBorderless = 1 << 4,
    };

    PULSARION_WINDOWING_API WindowStyles operator|(WindowStyles lhs, WindowStyles rhs);
    PULSARION_WINDOWING_API WindowStyles operator&(WindowStyles lhs, WindowStyles rhs);
    PULSARION_WINDOWING_API bool HasFlag(const WindowStyles& flags, const WindowStyles& flag);
}
