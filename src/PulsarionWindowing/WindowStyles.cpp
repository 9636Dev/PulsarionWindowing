#include "WindowStyles.hpp"

#include <type_traits>

namespace Pulsarion::Windowing
{
    WindowStyles operator|(WindowStyles lhs, WindowStyles rhs)
    {
        return static_cast<WindowStyles>(static_cast<std::uint64_t>(lhs) | static_cast<std::uint64_t>(rhs));
    }
    WindowStyles operator&(WindowStyles lhs, WindowStyles rhs)
    {
        return static_cast<WindowStyles>(static_cast<std::uint64_t>(lhs) & static_cast<std::uint64_t>(rhs));
    }

    bool HasFlag(const WindowStyles& flags, const WindowStyles& flag)
    {
        return (static_cast<std::underlying_type_t<WindowStyles>>(flags) & static_cast<std::underlying_type_t<WindowStyles>>(flag)) != 0;
    }
}
