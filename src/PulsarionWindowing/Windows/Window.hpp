#pragma once

#include "../Window.hpp"

#include <Windows.h>
#include <string>
#include <functional>
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
        inline void PollEvents() override;
        [[nodiscard]] inline bool ShouldClose() const override;
        inline void SetShouldClose(bool shouldClose) override;

        // --- Event Callbacks ---
        void SetOnClose(CloseCallback&& onClose) override { m_Data.OnClose = std::move(onClose); }
        [[nodiscard]] const CloseCallback& GetOnClose() const override { return m_Data.OnClose; }
        void SetOnWindowVisibility(WindowVisibilityCallback&& onWindowVisibility) override { m_Data.OnWindowVisibility = std::move(onWindowVisibility); }
        [[nodiscard]] const WindowVisibilityCallback& GetOnWindowVisibility() const override { return m_Data.OnWindowVisibility; }
        void SetOnFocus(FocusCallback&& onFocus) override { m_Data.OnFocus = std::move(onFocus); }
        [[nodiscard]] const FocusCallback& GetOnFocus() const override { return m_Data.OnFocus; }
        void SetOnResize(ResizeCallback&& onResize) override { m_Data.OnResize = std::move(onResize); }
        [[nodiscard]] const ResizeCallback& GetOnResize() const override { return m_Data.OnResize; }
        void SetUserData(void* userData) override { m_Data.UserData = userData; }
        [[nodiscard]] void* GetUserData() const override { return m_Data.UserData; }

        #ifdef PULSARION_WINDOWING_LIMIT_EVENTS
        void LimitEvents(bool limitEvents) override { m_Data.LimitEvents = limitEvents; }
        [[nodiscard]] bool IsLimitingEvents() const override { return m_Data.LimitEvents; }
        #endif
    private:
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        struct Data
        {
        public:
            bool ShouldClose = false;
            void* UserData = nullptr;
            #ifdef PULSARION_WINDOWING_LIMIT_EVENTS
            bool LimitEvents = false;
            std::vector<UINT> LimitedEvents; // We use this as a set
            #endif

            // --- Event Callbacks ---
            std::function<bool()> OnClose = nullptr;
            std::function<void(bool)> OnWindowVisibility = nullptr;
            std::function<void(bool)> OnFocus = nullptr;
            std::function<void(std::uint32_t, std::uint32_t)> OnResize = nullptr;

            Data() = default;
        };

        std::string m_WindowClassName;
        HWND m_WindowHandle;
        Data m_Data;
    };
}
