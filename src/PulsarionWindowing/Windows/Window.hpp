#pragma once

#include "../Window.hpp"

#include <Windows.h>
#include <string>

namespace Pulsarion::Windowing
{
    class PULSARION_WINDOWING_API WindowsWindow : public Window
    {
    public:
        friend std::shared_ptr<Window> CreateSharedWindow(WindowCreationData& creationData);
        friend std::unique_ptr<Window> CreateUniqueWindow(WindowCreationData& creationData);
        explicit WindowsWindow(WindowCreationData& creationData);
        ~WindowsWindow() override;

        void SetVisible(bool visible) override;
        void PollEvents() override;
        [[nodiscard]] bool ShouldClose() const override;
    private:
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        struct Data
        {
            bool ShouldClose;

            Data() : ShouldClose(false) {}
        };

        std::string m_WindowClassName;
        HWND m_WindowHandle;
        Data m_Data;
    };
}
