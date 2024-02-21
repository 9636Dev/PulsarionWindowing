#include "Window.hpp"

#include "PulsarionCore/Assert.hpp"

namespace Pulsarion::Windowing
{
    static std::string GetUniqueName() {
        static int counter = 0;
        return "PulsarionWindow" + std::to_string(counter++);
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
            creationData.X == WindowCreationData::Default ? CW_USEDEFAULT : static_cast<int>(creationData.X),
            creationData.Y == WindowCreationData::Default ? CW_USEDEFAULT : static_cast<int>(creationData.Y),
            creationData.Width == WindowCreationData::Default ? CW_USEDEFAULT : static_cast<int>(creationData.Width),
            creationData.Height == WindowCreationData::Default ? CW_USEDEFAULT : static_cast<int>(creationData.Height),
            nullptr,
            nullptr,
            GetModuleHandle(nullptr),
            &m_Data // We pass the data here
        );

        if (!m_WindowHandle)
            return; // The creation function will handle this

        // Set VSync with windows api
    }

    void WindowsWindow::SetVisible(bool visible)
    {
        ShowWindow(m_WindowHandle, visible ? SW_SHOW : SW_HIDE);
    }

    void WindowsWindow::SetTitle(const std::string& title)
    {
        SetWindowText(m_WindowHandle, title.c_str());
    }

    std::optional<std::string> WindowsWindow::GetTitle() const
    {
        int size = GetWindowTextLength(m_WindowHandle);
        if (size == 0)
            return std::nullopt;
        std::unique_ptr<char[]> buffer(new char[size + 1]);
        if (GetWindowText(m_WindowHandle, buffer.get(), size + 1))
        {
            PULSARION_ASSERT(buffer[size] == '\0', "The buffer should be null terminated");
            std::string str = std::string(buffer.get());
        }
        return std::nullopt;
    }

    void WindowsWindow::PollEvents()
    {
        m_Data.LimitedEvents.clear();
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

    void WindowsWindow::SetShouldClose(bool shouldClose)
    {
        m_Data.ShouldClose = shouldClose;
    }

    WindowsWindow::~WindowsWindow()
    {
        PostQuitMessage(0);
        DestroyWindow(m_WindowHandle);
        UnregisterClass(m_WindowClassName.c_str(), GetModuleHandle(nullptr));
    }

    LRESULT CALLBACK WindowsWindow::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        #ifdef PULSARION_WINDOWING_LIMIT_EVENTS
        #define LIMIT_EVENT(event) if (data->LimitEvents) { if (std::find(data->LimitedEvents.begin(), data->LimitedEvents.end(), event) == data->LimitedEvents.end()) { data->LimitedEvents.push_back(event); } else break; }
        #else
        #define LIMIT_EVENT(event)
        #endif

        switch (msg)
        {
        case WM_CREATE:
        {
            auto* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            auto* myData = reinterpret_cast<WindowsWindow::Data*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)myData);
            break;
        }
        case WM_SHOWWINDOW:
        {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (data->OnWindowVisibility)
                data->OnWindowVisibility(data->UserData, wParam);
            break;
        }
        case WM_CLOSE: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (data->OnClose)
                data->ShouldClose = data->OnClose(data->UserData);
            else
                data->ShouldClose = true;
            break;
        }
        case WM_SETFOCUS: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

            if (data->OnFocus)
                data->OnFocus(data->UserData, true);
            break;
        }
        case WM_KILLFOCUS: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

            if (data->OnFocus)
                data->OnFocus(data->UserData, false);
            break;
        }
        case WM_SIZE: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_SIZE);
            if (data->OnResize)
                data->OnResize(data->UserData, LOWORD(lParam), HIWORD(lParam));
            break;
        }
        case WM_MOVE: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_MOVE);
            if (data->OnMove)
                data->OnMove(data->UserData, LOWORD(lParam), HIWORD(lParam));
            break;
        }
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
