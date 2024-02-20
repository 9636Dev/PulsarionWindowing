#pragma once

#include "Core.hpp"
#include "WindowFlags.hpp"

#include <memory>
#include <string>
#include <limits>
#include <functional>

namespace Pulsarion::Windowing
{
    class Window
    {
    public:
        virtual ~Window() = default;

        virtual void SetVisible(bool visible) = 0;
        virtual void PollEvents() = 0;
        [[nodiscard]] virtual bool ShouldClose() const = 0;
        virtual void SetShouldClose(bool shouldClose) = 0;

        // --- Event Callbacks ---
        using CloseCallback = std::function<bool()>;
        using WindowVisibilityCallback = std::function<void(bool)>; // A callback that doesn't return anything or take any parameters
        using FocusCallback = std::function<void(bool)>;
        using ResizeCallback = std::function<void(std::uint32_t, std::uint32_t)>;
        virtual void SetOnClose(CloseCallback&& onClose) = 0;
        [[nodiscard]] virtual const CloseCallback& GetOnClose() const = 0;
        virtual void SetOnWindowVisibility(WindowVisibilityCallback&& onWindowVisibility) = 0;
        [[nodiscard]] virtual const WindowVisibilityCallback& GetOnWindowVisibility() const = 0;
        virtual void SetUserData(void* userData) = 0;
        [[nodiscard]] virtual void* GetUserData() const = 0;
        virtual void SetOnFocus(FocusCallback&& onFocus) = 0;
        [[nodiscard]] virtual const FocusCallback& GetOnFocus() const = 0;
        virtual void SetOnResize(ResizeCallback&& onResize) = 0;
        [[nodiscard]] virtual const ResizeCallback& GetOnResize() const = 0;

        #ifdef PULSARION_WINDOWING_LIMIT_EVENTS
        virtual void LimitEvents(bool limitEvents) = 0;
        [[nodiscard]] virtual bool IsLimitingEvents() const = 0;
        #endif
    };

    struct WindowCreationData
    {
        constexpr static std::uint32_t Default = std::numeric_limits<std::uint32_t>::max();

        std::string Title;
        std::uint32_t Width;
        std::uint32_t Height;
        std::uint32_t X;
        std::uint32_t Y;
        WindowFlags Flags;

        explicit WindowCreationData(const std::string& title, std::uint32_t width = Default, std::uint32_t height = Default, std::uint32_t x = Default, std::uint32_t y = Default, WindowFlags flags = WindowFlags::Default)
            : Title(title), Width(width), Height(height), X(x), Y(y), Flags(flags) {}
    };

    extern PULSARION_WINDOWING_API std::shared_ptr<Window> CreateSharedWindow(WindowCreationData& creationData);
    extern PULSARION_WINDOWING_API std::unique_ptr<Window> CreateUniqueWindow(WindowCreationData& creationData);
}
