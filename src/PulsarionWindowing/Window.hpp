#pragma once

#include "Core.hpp"
#include "WindowFlags.hpp"

#include <memory>
#include <string>
#include <limits>
#include <functional>
#include <optional>

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
        virtual void SetTitle(const std::string& title) = 0;
        [[nodiscard]] virtual std::optional<std::string> GetTitle() const = 0;
        [[nodiscard]] virtual void* GetNativeWindow() const = 0;

        // --- Event Callbacks ---
        using CloseCallback = std::function<bool(void*)>;
        using VisibilityCallback = std::function<void(void*, bool)>; // A callback that doesn't return anything or take any parameters
        using FocusCallback = std::function<void(void*, bool)>;
        using ResizeCallback = std::function<void(void*, std::uint32_t, std::uint32_t)>;
        using MoveCallback = std::function<void(void*, std::uint32_t, std::uint32_t)>;
        using BeforeResizeCallback = std::function<void(void*)>;
        using MinimizeCallback = std::function<void(void*)>;
        using MaximizeCallback = std::function<void(void*)>;
        using RestoreCallback = std::function<void(void*)>;

        virtual void SetOnClose(CloseCallback&& onClose) = 0;
        [[nodiscard]] virtual CloseCallback GetOnClose() const = 0;
        virtual void SetOnWindowVisibility(VisibilityCallback&& onWindowVisibility) = 0;
        [[nodiscard]] virtual VisibilityCallback GetOnWindowVisibility() const = 0;
        virtual void SetUserData(void* userData) = 0;
        [[nodiscard]] virtual void* GetUserData() const = 0;
        virtual void SetOnFocus(FocusCallback&& onFocus) = 0;
        [[nodiscard]] virtual FocusCallback GetOnFocus() const = 0;
        virtual void SetOnResize(ResizeCallback&& onResize) = 0;
        [[nodiscard]] virtual ResizeCallback GetOnResize() const = 0;
        virtual void SetOnMove(MoveCallback&& onMove) = 0;
        [[nodiscard]] virtual MoveCallback GetOnMove() const = 0;
        virtual void SetBeforeResize(BeforeResizeCallback&& beforeResize) = 0;
        [[nodiscard]] virtual BeforeResizeCallback GetBeforeResize() const = 0;
        virtual void SetOnMinimize(MinimizeCallback&& onMinimize) = 0;
        [[nodiscard]] virtual MinimizeCallback GetOnMinimize() const = 0;
        virtual void SetOnMaximize(MaximizeCallback&& onMaximize) = 0;
        [[nodiscard]] virtual MaximizeCallback GetOnMaximize() const = 0;
        virtual void SetOnRestore(RestoreCallback&& onRestore) = 0;
        [[nodiscard]] virtual RestoreCallback GetOnRestore() const = 0;

        #ifdef PULSARION_WINDOWING_LIMIT_EVENTS
        virtual void LimitEvents(bool limitEvents) = 0;
        [[nodiscard]] virtual bool IsLimitingEvents() const = 0;
        #endif
    };

    struct WindowEvents
    {
        Window::CloseCallback OnClose = nullptr;
        Window::VisibilityCallback OnWindowVisibility = nullptr;
        Window::FocusCallback OnFocus = nullptr;
        Window::ResizeCallback OnResize = nullptr;
        Window::MoveCallback OnMove = nullptr;
        Window::BeforeResizeCallback BeforeResize = nullptr;
        Window::MinimizeCallback OnMinimize = nullptr;
        Window::MaximizeCallback OnMaximize = nullptr;
        Window::RestoreCallback OnRestore = nullptr;
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

        explicit WindowCreationData(std::string title, std::uint32_t width = Default, std::uint32_t height = Default, std::uint32_t x = Default, std::uint32_t y = Default, WindowFlags flags = WindowFlags::Default)
            : Title(std::move(title)), Width(width), Height(height), X(x), Y(y), Flags(flags) {}
    };

    extern PULSARION_WINDOWING_API std::shared_ptr<Window> CreateSharedWindow(WindowCreationData& creationData);
    extern PULSARION_WINDOWING_API std::unique_ptr<Window> CreateUniqueWindow(WindowCreationData& creationData);
}
