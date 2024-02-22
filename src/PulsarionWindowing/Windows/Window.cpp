#include "Window.hpp"

#include "PulsarionCore/Assert.hpp"

namespace Pulsarion::Windowing
{
    static std::string GetUniqueName() {
        static int counter = 0;
        return "PulsarionWindow" + std::to_string(counter++);
    }

    WindowsWindow::WindowsWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config)
    {
        m_Data = {};
        m_WindowClassName = GetUniqueName();

        WNDCLASS wc = {};
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpfnWndProc = WindowProc;
        wc.lpszClassName = m_WindowClassName.c_str();
        RegisterClass(&wc);

        DWORD styleMask = 0;
        if (HasFlag(styles, WindowStyles::WSCaption))
            styleMask |= WS_CAPTION;
        if (HasFlag(styles, WindowStyles::WSSysMenu))
            styleMask |= WS_SYSMENU;
        if (HasFlag(styles, WindowStyles::WSMinimizeBox))
            styleMask |= WS_MINIMIZEBOX;
        if (HasFlag(styles, WindowStyles::WSMaximizeBox))
            styleMask |= WS_MAXIMIZEBOX;
        if (HasFlag(styles, WindowStyles::WSThickFrame))
            styleMask |= WS_THICKFRAME;
        //if (HasFlag(styles.Flags, WindowFlags::AlwaysOnTop))
        //    styleMask |= WS_EX_TOPMOST;

        if (config.StartVisible)
            styleMask |= WS_VISIBLE;

        m_WindowHandle = CreateWindow(
            m_WindowClassName.c_str(),
            title.c_str(),
            styleMask,
            bounds.X, bounds.Y, bounds.Width, bounds.Height,
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
            return std::string(buffer.get());
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

            switch (wParam)
            {
            case SIZE_MINIMIZED:
                if (data->OnMinimize)
                    data->OnMinimize(data->UserData);
                break;
            case SIZE_MAXIMIZED:
                if (data->OnMaximize)
                    data->OnMaximize(data->UserData);
                break;
            case SIZE_RESTORED:
                if (data->OnRestore)
                    data->OnRestore(data->UserData);
                break;
            default:
                LIMIT_EVENT(WM_SIZE);
                if (data->OnResize)
                    data->OnResize(data->UserData, LOWORD(lParam), HIWORD(lParam));
                break;
            }
        }
        case WM_MOVE: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_MOVE);
            if (data->OnMove)
                data->OnMove(data->UserData, LOWORD(lParam), HIWORD(lParam));
            break;
        }
        case WM_MOUSEMOVE: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (!data->TrackingMouse) {
                TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = hWnd;
                TrackMouseEvent(&tme);
                data->TrackingMouse = true;
                if (data->OnMouseEnter)
                    data->OnMouseEnter(data->UserData);
            }
            break;
        }
        case WM_MOUSELEAVE: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            data->TrackingMouse = false;
            if (data->OnMouseLeave)
                data->OnMouseLeave(data->UserData);
            break;
        }
        case WM_MOUSEHOVER: {
            break;
        }
        case WM_SYSCOMMAND: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            // TODO: In the future we have a BeforeMinimize event and BeforeMaximize event
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }

        return 0;
    }

    std::shared_ptr<Window> CreateSharedWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config, std::optional<WindowEvents> events) {
        auto res = std::make_shared<WindowsWindow>(std::move(title), bounds, styles, config);
        if (!res->m_WindowHandle)
            return nullptr;
        if (events.has_value())
            SetWindowEvents(*res, *events);
        return res;
    }

    std::unique_ptr<Window> CreateUniqueWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config, std::optional<WindowEvents> events)
    {
        auto res = std::make_unique<WindowsWindow>(std::move(title), bounds, styles, config);
        if (events.has_value())
            SetWindowEvents(*res, *events);
        return res;
    }
}
