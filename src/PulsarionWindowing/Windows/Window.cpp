#include "Window.hpp"
#include <chrono>
#include <functional>

namespace Pulsarion::Windowing
{
    static std::string GetUniqueName() {
        static int counter = 0;
        return "PulsarionWindow" + std::to_string(counter++);
    }

    inline static bool HasFlag(const WindowFlags& flags, const WindowFlags& flag)
    {
        return (static_cast<std::underlying_type_t<WindowFlags>>(flags) & static_cast<std::underlying_type_t<WindowFlags>>(flag)) != 0;
    }

    WindowsWindow::WindowsWindow(WindowCreationData& creationData)
    {
        m_Data = {};
        m_WindowClassName = GetUniqueName();

        WNDCLASS wc = {};
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpfnWndProc = WindowProc;
        wc.lpszClassName = m_WindowClassName.c_str();
        RegisterClass(&wc);

        DWORD style = 0;
        if (HasFlag(creationData.Flags, WindowFlags::TitleBar))
            style |= WS_CAPTION;
        if (HasFlag(creationData.Flags, WindowFlags::SysMenu))
            style |= WS_SYSMENU;
        if (HasFlag(creationData.Flags, WindowFlags::MinimizeButton))
            style |= WS_MINIMIZEBOX;
        if (HasFlag(creationData.Flags, WindowFlags::MaximizeButton))
            style |= WS_MAXIMIZEBOX;
        if (HasFlag(creationData.Flags, WindowFlags::Resizable))
            style |= WS_THICKFRAME;
        if (HasFlag(creationData.Flags, WindowFlags::Visible))
            style |= WS_VISIBLE;
        if (HasFlag(creationData.Flags, WindowFlags::AlwaysOnTop))
            style |= WS_EX_TOPMOST;

        m_WindowHandle = CreateWindow(
            m_WindowClassName.c_str(),
            creationData.Title.c_str(),
            style,
            creationData.X == creationData.DefaultX ? CW_USEDEFAULT : static_cast<int>(creationData.X),
            creationData.Y == creationData.DefaultY ? CW_USEDEFAULT : static_cast<int>(creationData.Y),
            creationData.Width == creationData.DefaultWidth ? CW_USEDEFAULT : static_cast<int>(creationData.Width),
            creationData.Height == creationData.DefaultHeight ? CW_USEDEFAULT : static_cast<int>(creationData.Height),
            nullptr,
            nullptr,
            GetModuleHandle(nullptr),
            &m_Data // We pass the data here
        );

        if (!m_WindowHandle)
            return; // The creation function will handle this
    }

    void WindowsWindow::SetVisible(bool visible)
    {
        ShowWindow(m_WindowHandle, visible ? SW_SHOW : SW_HIDE);
    }

    void WindowsWindow::PollEvents()
    {
        MSG msg = {};
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    bool WindowsWindow::ShouldClose() const
    {
        return m_Data.ShouldClose;
    }

    WindowsWindow::~WindowsWindow()
    {
        DestroyWindow(m_WindowHandle);
        UnregisterClass(m_WindowClassName.c_str(), GetModuleHandle(nullptr));
    }

    LRESULT CALLBACK WindowsWindow::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_CREATE:
        {
            auto* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            auto* myData = reinterpret_cast<WindowsWindow::Data*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)myData);
        }
        break;
        case WM_CLOSE:
            PostQuitMessage(0);
            ((WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->ShouldClose = true;
            break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }

        return 0;
    }

    std::shared_ptr<Window> CreateSharedWindow(WindowCreationData& creationData)
    {
        auto res = std::make_shared<WindowsWindow>(creationData);
        if (res->m_WindowHandle)
            return res;
        return nullptr;
    }

    std::unique_ptr<Window> CreateUniqueWindow(WindowCreationData& creationData)
    {
        auto res = std::make_unique<WindowsWindow>(creationData);
        if (res->m_WindowHandle)
            return res;
        return nullptr;
    }
}
