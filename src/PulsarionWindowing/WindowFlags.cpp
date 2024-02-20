#include "WindowFlags.hpp"

#include <type_traits>

namespace Pulsarion::Windowing
{
    WindowFlags operator|(WindowFlags lhs, WindowFlags rhs)
    {
        return static_cast<WindowFlags>(static_cast<std::uint64_t>(lhs) | static_cast<std::uint64_t>(rhs));
    }
    WindowFlags operator&(WindowFlags lhs, WindowFlags rhs)
    {
        return static_cast<WindowFlags>(static_cast<std::uint64_t>(lhs) & static_cast<std::uint64_t>(rhs));
    }

    bool HasFlag(const WindowFlags& flags, const WindowFlags& flag)
    {
        return (static_cast<std::underlying_type_t<WindowFlags>>(flags) & static_cast<std::underlying_type_t<WindowFlags>>(flag)) != 0;
    }
}
