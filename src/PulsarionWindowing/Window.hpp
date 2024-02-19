#pragma once

#include "Core.hpp"
#include "WindowFlags.hpp"

#include <memory>
#include <string>
#include <limits>

namespace Pulsarion::Windowing
{
    class Window
    {
    public:
        virtual ~Window() = default;

        virtual void SetVisible(bool visible) = 0;
        virtual void PollEvents() = 0;
        [[nodiscard]] virtual bool ShouldClose() const = 0;
    };

    struct WindowCreationData
    {
        constexpr static std::uint32_t DefaultWidth = std::numeric_limits<std::uint32_t>::max();
        constexpr static std::uint32_t DefaultHeight = std::numeric_limits<std::uint32_t>::max();
        constexpr static std::uint32_t DefaultX = std::numeric_limits<std::uint32_t>::max();
        constexpr static std::uint32_t DefaultY = std::numeric_limits<std::uint32_t>::max();

        std::string Title;
        std::uint32_t Width;
        std::uint32_t Height;
        std::uint32_t X;
        std::uint32_t Y;
        WindowFlags Flags;

        explicit WindowCreationData(const std::string& title, std::uint32_t width = DefaultWidth, std::uint32_t height = DefaultHeight, std::uint32_t x = DefaultX, std::uint32_t y = DefaultY, WindowFlags flags = WindowFlags::Default)
            : Title(title), Width(width), Height(height), X(x), Y(y), Flags(flags) {}
    };

    extern PULSARION_WINDOWING_API std::shared_ptr<Window> CreateSharedWindow(WindowCreationData& creationData);
    extern PULSARION_WINDOWING_API std::unique_ptr<Window> CreateUniqueWindow(WindowCreationData& creationData);
}
