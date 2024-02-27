#pragma once

#include "Core.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "Cursor.hpp"
#include "WindowStyles.hpp"

#include <memory>
#include <string>
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
        virtual void SetCursorMode(CursorMode mode) = 0;
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
        using FullscreenCallback = std::function<void(void*, bool)>;
        using RestoreCallback = std::function<void(void*)>;
        using MouseEnterCallback = std::function<void(void*)>;
        using MouseLeaveCallback = std::function<void(void*)>;
        using MouseDownCallback = std::function<void(void*, Point, MouseCode)>;
        using MouseUpCallback = std::function<void(void*, Point, MouseCode)>;
        using MouseMoveCallback = std::function<void(void*, Point)>;
        using MouseWheelCallback = std::function<void(void*, Point, ScrollOffset)>;
        using KeyDownCallback = std::function<void(void*, KeyCode, Modifier, bool)>;
        using KeyUpCallback = std::function<void(void*, KeyCode, Modifier)>;
        using KeyTypedCallback = std::function<void(void*, char, Modifier)>;

        // ----- Window Event Callbacks -----
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
        virtual void SetOnFullscreen(FullscreenCallback&& onFullscreen) = 0;
        [[nodiscard]] virtual FullscreenCallback GetOnFullscreen() const = 0;
        virtual void SetOnRestore(RestoreCallback&& onRestore) = 0;
        [[nodiscard]] virtual RestoreCallback GetOnRestore() const = 0;

        // ----- Mouse Event Callbacks -----
        virtual void SetOnMouseEnter(MouseEnterCallback&& onMouseEnter) = 0;
        [[nodiscard]] virtual MouseEnterCallback GetOnMouseEnter() const = 0;
        virtual void SetOnMouseLeave(MouseLeaveCallback&& onMouseLeave) = 0;
        [[nodiscard]] virtual MouseLeaveCallback GetOnMouseLeave() const = 0;
        virtual void SetOnMouseDown(MouseDownCallback&& onMouseDown) = 0;
        [[nodiscard]] virtual MouseDownCallback GetOnMouseDown() const = 0;
        virtual void SetOnMouseUp(MouseUpCallback&& onMouseUp) = 0;
        [[nodiscard]] virtual MouseUpCallback GetOnMouseUp() const = 0;
        virtual void SetOnMouseMove(MouseMoveCallback&& onMouseMove) = 0;
        [[nodiscard]] virtual MouseMoveCallback GetOnMouseMove() const = 0;
        virtual void SetOnMouseWheel(MouseWheelCallback&& onMouseWheel) = 0;
        [[nodiscard]] virtual MouseWheelCallback GetOnMouseWheel() const = 0;

        // ----- Keyboard Event Callbacks -----
        virtual void SetOnKeyDown(KeyDownCallback&& onKeyDown) = 0;
        [[nodiscard]] virtual KeyDownCallback GetOnKeyDown() const = 0;
        virtual void SetOnKeyUp(KeyUpCallback&& onKeyUp) = 0;
        [[nodiscard]] virtual KeyUpCallback GetOnKeyUp() const = 0;
        virtual void SetOnKeyTyped(KeyTypedCallback&& onKeyTyped) = 0;
        [[nodiscard]] virtual KeyTypedCallback GetOnKeyTyped() const = 0;

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
        Window::FullscreenCallback OnFullscreen = nullptr;
        Window::RestoreCallback OnRestore = nullptr;
        Window::MouseEnterCallback OnMouseEnter = nullptr;
        Window::MouseLeaveCallback OnMouseLeave = nullptr;
        Window::MouseDownCallback OnMouseDown = nullptr;
        Window::MouseUpCallback OnMouseUp = nullptr;
        Window::MouseMoveCallback OnMouseMove = nullptr;
        Window::MouseWheelCallback OnMouseWheel = nullptr;
        Window::KeyDownCallback OnKeyDown = nullptr;
        Window::KeyUpCallback OnKeyUp = nullptr;
        Window::KeyTypedCallback OnKeyTyped = nullptr;
    };

    inline static void SetWindowEvents(Window& window, WindowEvents& events)
    {
        window.SetOnClose(std::move(events.OnClose));
        window.SetOnWindowVisibility(std::move(events.OnWindowVisibility));
        window.SetOnFocus(std::move(events.OnFocus));
        window.SetOnResize(std::move(events.OnResize));
        window.SetOnMove(std::move(events.OnMove));
        window.SetBeforeResize(std::move(events.BeforeResize));
        window.SetOnMinimize(std::move(events.OnMinimize));
        window.SetOnMaximize(std::move(events.OnMaximize));
        window.SetOnFullscreen(std::move(events.OnFullscreen));
        window.SetOnRestore(std::move(events.OnRestore));
        window.SetOnMouseEnter(std::move(events.OnMouseEnter));
        window.SetOnMouseLeave(std::move(events.OnMouseLeave));
        window.SetOnMouseDown(std::move(events.OnMouseDown));
        window.SetOnMouseUp(std::move(events.OnMouseUp));
        window.SetOnMouseMove(std::move(events.OnMouseMove));
        window.SetOnMouseWheel(std::move(events.OnMouseWheel));
        window.SetOnKeyDown(std::move(events.OnKeyDown));
        window.SetOnKeyUp(std::move(events.OnKeyUp));
        window.SetOnKeyTyped(std::move(events.OnKeyTyped));
    }

    extern PULSARION_WINDOWING_API std::shared_ptr<Window> CreateSharedWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config, std::optional<WindowEvents> events = std::nullopt);
    extern PULSARION_WINDOWING_API std::unique_ptr<Window> CreateUniqueWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config, std::optional<WindowEvents> events = std::nullopt);
}
