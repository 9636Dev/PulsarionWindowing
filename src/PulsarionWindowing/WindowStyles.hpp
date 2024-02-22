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

        // Not compatible with Borderless
        TitleBar = 1 << 0,
        CloseButton = 1 << 1,
        MiniaturizeButton = 1 << 2,
        Resizable = 1 << 3,
        Borderless = 1 << 4,

        NSTitled = TitleBar,
        NSClosable = CloseButton,
        NSMiniaturizable = MiniaturizeButton,
        NSResizable = Resizable,
        NSBorderless = Borderless,

        WSCaption = TitleBar,
        WSSysMenu = CloseButton,
        WSMinimizeBox = MiniaturizeButton,
        WSMaximizeBox = MiniaturizeButton,
        WSThickFrame = Resizable,
        WSNoBorder = Borderless, // Windows only has WS_BORDER, but we can just invert the flag
    };

    PULSARION_WINDOWING_API WindowStyles operator|(WindowStyles lhs, WindowStyles rhs);
    PULSARION_WINDOWING_API WindowStyles operator&(WindowStyles lhs, WindowStyles rhs);
    PULSARION_WINDOWING_API bool HasFlag(const WindowStyles& flags, const WindowStyles& flag);
}
