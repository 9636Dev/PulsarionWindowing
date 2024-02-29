#pragma once

#include "../Window.hpp"

#include <Windows.h>
#include <string>
#include <vector>

namespace Pulsarion::Windowing
{
    class PULSARION_WINDOWING_API WindowsWindow : public Window
    {
    public:
        friend std::shared_ptr<Window> CreateSharedWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config, std::optional<WindowEvents> events);
        friend std::unique_ptr<Window> CreateUniqueWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config, std::optional<WindowEvents> events);
        explicit WindowsWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config);
        ~WindowsWindow() override;

        inline void SetVisible(bool visible) override;
        void SetTitle(const std::string& title) override;
        [[nodiscard]] std::optional<std::string> GetTitle() const override;
        inline void PollEvents() override;
        [[nodiscard]] inline bool ShouldClose() const override;
        inline void SetCursorMode(CursorMode mode) override;
        inline void SetShouldClose(bool shouldClose) override;
        [[nodiscard]] void* GetNativeWindow() const override { return m_WindowHandle; }

        // --- Event Callbacks ---
        void SetOnClose(CloseCallback&& onClose) override { m_Data.OnClose = std::move(onClose); }
        [[nodiscard]] CloseCallback GetOnClose() const override { return m_Data.OnClose; }
        void SetOnWindowVisibility(VisibilityCallback&& onWindowVisibility) override { m_Data.OnWindowVisibility = std::move(onWindowVisibility); }
        [[nodiscard]] VisibilityCallback GetOnWindowVisibility() const override { return m_Data.OnWindowVisibility; }
        void SetOnFocus(FocusCallback&& onFocus) override { m_Data.OnFocus = std::move(onFocus); }
        [[nodiscard]] FocusCallback GetOnFocus() const override { return m_Data.OnFocus; }
        void SetOnResize(ResizeCallback&& onResize) override { m_Data.OnResize = std::move(onResize); }
        [[nodiscard]] ResizeCallback GetOnResize() const override { return m_Data.OnResize; }
        void SetOnMove(MoveCallback&& onMove) override { m_Data.OnMove = std::move(onMove); }
        [[nodiscard]] MoveCallback GetOnMove() const override { return m_Data.OnMove; }
        void SetBeforeResize(BeforeResizeCallback&& beforeResize) override { m_Data.BeforeResize = std::move(beforeResize); }
        [[nodiscard]] BeforeResizeCallback GetBeforeResize() const override { return m_Data.BeforeResize; }
        void SetOnMinimize(MinimizeCallback&& onMinimize) override { m_Data.OnMinimize = std::move(onMinimize); }
        [[nodiscard]] MinimizeCallback GetOnMinimize() const override { return m_Data.OnMinimize; }
        void SetOnMaximize(MaximizeCallback&& onMaximize) override { m_Data.OnMaximize = std::move(onMaximize); }
        [[nodiscard]] MaximizeCallback GetOnMaximize() const override { return m_Data.OnMaximize; }
        // We need to handle this ourselves when we click 'F11' or Call Fullscreen function (in the future)
        void SetOnFullscreen(FullscreenCallback&& onFullscreen) override { m_Data.OnFullscreen = std::move(onFullscreen); }
        [[nodiscard]] FullscreenCallback GetOnFullscreen() const override { return m_Data.OnFullscreen; }
        void SetOnRestore(RestoreCallback&& onRestore) override { m_Data.OnRestore = std::move(onRestore); }
        [[nodiscard]] RestoreCallback GetOnRestore() const override { return m_Data.OnRestore; }
        void SetOnMouseEnter(MouseEnterCallback&& onMouseEnter) override { m_Data.OnMouseEnter = std::move(onMouseEnter); }
        [[nodiscard]] MouseEnterCallback GetOnMouseEnter() const override { return m_Data.OnMouseEnter; }
        void SetOnMouseLeave(MouseLeaveCallback&& onMouseLeave) override { m_Data.OnMouseLeave = std::move(onMouseLeave); }
        [[nodiscard]] MouseLeaveCallback GetOnMouseLeave() const override { return m_Data.OnMouseLeave; }
        void SetOnMouseDown(MouseDownCallback&& onMouseDown) override { m_Data.OnMouseDown = std::move(onMouseDown); }
        [[nodiscard]] MouseDownCallback GetOnMouseDown() const override { return m_Data.OnMouseDown; }
        void SetOnMouseUp(MouseUpCallback&& onMouseUp) override { m_Data.OnMouseUp = std::move(onMouseUp); }
        [[nodiscard]] MouseUpCallback GetOnMouseUp() const override { return m_Data.OnMouseUp; }
        void SetOnMouseMove(MouseMoveCallback&& onMouseMove) override { m_Data.OnMouseMove = std::move(onMouseMove); }
        [[nodiscard]] MouseMoveCallback GetOnMouseMove() const override { return m_Data.OnMouseMove; }
        void SetOnMouseWheel(MouseWheelCallback&& onMouseWheel) override { m_Data.OnMouseWheel = std::move(onMouseWheel); }
        [[nodiscard]] MouseWheelCallback GetOnMouseWheel() const override { return m_Data.OnMouseWheel; }
        void SetOnKeyDown(KeyDownCallback&& onKeyDown) override { m_Data.OnKeyDown = std::move(onKeyDown); }
        [[nodiscard]] KeyDownCallback GetOnKeyDown() const override { return m_Data.OnKeyDown; }
        void SetOnKeyUp(KeyUpCallback&& onKeyUp) override { m_Data.OnKeyUp = std::move(onKeyUp); }
        [[nodiscard]] KeyUpCallback GetOnKeyUp() const override { return m_Data.OnKeyUp; }
        void SetOnKeyTyped(KeyTypedCallback&& onKeyTyped) override { m_Data.OnKeyTyped = std::move(onKeyTyped); }
        [[nodiscard]] KeyTypedCallback GetOnKeyTyped() const override { return m_Data.OnKeyTyped; }

        void SetUserData(void* userData) override { m_Data.UserData = userData; }
        [[nodiscard]] void* GetUserData() const override { return m_Data.UserData; }

        #ifdef PULSARION_WINDOWING_LIMIT_EVENTS
        void LimitEvents(bool limitEvents) override { m_Data.LimitEvents = limitEvents; }
        [[nodiscard]] bool IsLimitingEvents() const override { return m_Data.LimitEvents; }
        #endif
    private:
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        struct Data : WindowEvents
        {
        public:
            bool ShouldClose = false;
            bool TrackingMouse = false;
            void* UserData = nullptr;
            #ifdef PULSARION_WINDOWING_LIMIT_EVENTS
            bool LimitEvents = false;
            std::vector<UINT> LimitedEvents = {}; // We use this as a set
            #endif

            Data() = default;
        };

        std::string m_WindowClassName;
        HWND m_WindowHandle;
        Data m_Data;
    };
}
