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
        friend std::shared_ptr<Window> CreateSharedWindow(WindowCreationData& creationData);
        friend std::unique_ptr<Window> CreateUniqueWindow(WindowCreationData& creationData);
        explicit WindowsWindow(WindowCreationData& creationData);
        ~WindowsWindow() override;

        inline void SetVisible(bool visible) override;
        void SetTitle(const std::string& title) override;
        [[nodiscard]] std::optional<std::string> GetTitle() const override;
        inline void PollEvents() override;
        [[nodiscard]] inline bool ShouldClose() const override;
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
