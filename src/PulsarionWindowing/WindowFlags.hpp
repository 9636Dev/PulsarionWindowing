#pragma once

#include "Core.hpp"
#include <cstdint>

namespace Pulsarion::Windowing
{

    enum class WindowFlags : std::uint64_t
    {
        None = 0,
        TitleBar = 1 << 0,
        SysMenu = 1 << 1, // System menu
        Resizable = 1 << 2, // Thick frame for Windows
        ThickFrame = Resizable,
        MinimizeButton = 1 << 3,
        MaximizeButton = 1 << 4,
        Visible = 1 << 5, // Initially visible
        AlwaysOnTop = 1 << 6, // WS_EX_TOPMOST
        Caption = TitleBar | SysMenu | MinimizeButton | MaximizeButton,
        DefaultNoVisible = Caption,
        Default = Caption | Visible
    };

    PULSARION_WINDOWING_API WindowFlags operator|(WindowFlags lhs, WindowFlags rhs);
    PULSARION_WINDOWING_API WindowFlags operator&(WindowFlags lhs, WindowFlags rhs);
    PULSARION_WINDOWING_API bool HasFlag(const WindowFlags& flags, const WindowFlags& flag);
}
