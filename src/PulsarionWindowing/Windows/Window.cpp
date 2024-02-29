#include "Window.hpp"

#include "PulsarionCore/Assert.hpp"

namespace Pulsarion::Windowing
{
    static std::string GetUniqueName() {
        static int counter = 0;
        return "PulsarionWindow" + std::to_string(counter++);
    }

    KeyCode ConvertFromVirtualKey(UINT vkCode) {
        if (vkCode >= 'A' && vkCode <= 'Z') {
            return static_cast<KeyCode>(vkCode);
        }
        if (vkCode >= '0' && vkCode <= '9') {
            return static_cast<KeyCode>(vkCode);
        }

        switch (vkCode) {
            case VK_BACK: return KeyCode::Backspace;
            case VK_TAB: return KeyCode::Tab;
            //case VK_CLEAR:
            case VK_RETURN: return KeyCode::Enter;
            case VK_SHIFT: return KeyCode::LeftShift;
            case VK_CONTROL: return KeyCode::LeftControl;
            case VK_MENU: return KeyCode::LeftAlt;
            case VK_PAUSE: return KeyCode::Pause;
            case VK_CAPITAL: return KeyCode::CapsLock;
            case VK_ESCAPE: return KeyCode::Escape;
            case VK_SPACE: return KeyCode::Space;
            case VK_PRIOR: return KeyCode::PageUp;
            case VK_NEXT: return KeyCode::PageDown;
            case VK_END: return KeyCode::End;
            case VK_HOME: return KeyCode::Home;
            case VK_LEFT: return KeyCode::Left;
            case VK_UP: return KeyCode::Up;
            case VK_RIGHT: return KeyCode::Right;
            case VK_DOWN: return KeyCode::Down;
            //case VK_SELECT: return KeyCode::Select;
            //case VK_PRINT: return KeyCode::Print;
            //case VK_EXECUTE: return KeyCode::Execute;
            case VK_SNAPSHOT: return KeyCode::PrintScreen;
            case VK_INSERT: return KeyCode::Insert;
            case VK_DELETE: return KeyCode::Delete;
            //case VK_HELP: return KeyCode::Help;
            case VK_F1: return KeyCode::F1;
            case VK_F2: return KeyCode::F2;
            case VK_F3: return KeyCode::F3;
            case VK_F4: return KeyCode::F4;
            case VK_F5: return KeyCode::F5;
            case VK_F6: return KeyCode::F6;
            case VK_F7: return KeyCode::F7;
            case VK_F8: return KeyCode::F8;
            case VK_F9: return KeyCode::F9;
            case VK_F10: return KeyCode::F10;
            case VK_F11: return KeyCode::F11;
            case VK_F12: return KeyCode::F12;
            case VK_F13: return KeyCode::F13;
            case VK_F14: return KeyCode::F14;
            case VK_F15: return KeyCode::F15;
            case VK_F16: return KeyCode::F16;
            case VK_F17: return KeyCode::F17;
            case VK_F18: return KeyCode::F18;
            case VK_F19: return KeyCode::F19;
            case VK_F20: return KeyCode::F20;
            case VK_F21: return KeyCode::F21;
            case VK_F22: return KeyCode::F22;
            case VK_F23: return KeyCode::F23;
            case VK_F24: return KeyCode::F24;
            case VK_NUMLOCK: return KeyCode::NumLock;
            case VK_SCROLL: return KeyCode::ScrollLock;
            case VK_LSHIFT: return KeyCode::LeftShift;
            case VK_RSHIFT: return KeyCode::RightShift;
            case VK_LCONTROL: return KeyCode::LeftControl;
            case VK_RCONTROL: return KeyCode::RightControl;
            case VK_LMENU: return KeyCode::LeftAlt;
            case VK_RMENU: return KeyCode::RightAlt;
            case VK_OEM_1: return KeyCode::Semicolon;
            case VK_OEM_PLUS: return KeyCode::Equal;
            case VK_OEM_COMMA: return KeyCode::Comma;
            case VK_OEM_MINUS: return KeyCode::Minus;
            case VK_OEM_PERIOD: return KeyCode::Period;
            case VK_OEM_2: return KeyCode::Slash;
            case VK_OEM_3: return KeyCode::GraveAccent;
            case VK_OEM_4: return KeyCode::LeftBracket;
            case VK_OEM_5: return KeyCode::Backslash;
            case VK_OEM_6: return KeyCode::RightBracket;
            case VK_OEM_7: return KeyCode::Apostrophe;
            //case VK_OEM_8: return KeyCode::OEM_8;
            case VK_OEM_102: return KeyCode::Backslash;
            default: return KeyCode::Unknown;
        }
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

    void WindowsWindow::SetCursorMode(CursorMode mode)
    {
        if (mode == CursorMode::Normal)
            ShowCursor(TRUE);
        else
            ShowCursor(FALSE);
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

    static Point GetMousePosition(LPARAM lParam)
    {
        return Point(LOWORD(lParam), HIWORD(lParam));
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
        case WM_LBUTTONDOWN: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_LBUTTONDOWN);
            if (data->OnMouseDown)
                data->OnMouseDown(data->UserData, GetMousePosition(lParam), MouseCode::Button0);
            break;
        }
        case WM_LBUTTONUP: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_LBUTTONUP);
            if (data->OnMouseUp)
                data->OnMouseUp(data->UserData, GetMousePosition(lParam), MouseCode::Button0);
            break;
        }
        case WM_RBUTTONDOWN: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_RBUTTONDOWN);
            if (data->OnMouseDown)
                data->OnMouseDown(data->UserData, GetMousePosition(lParam), MouseCode::Button1);
            break;
        }
        case WM_RBUTTONUP: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_RBUTTONUP);
            if (data->OnMouseUp)
                data->OnMouseUp(data->UserData, GetMousePosition(lParam), MouseCode::Button1);
            break;
        }
        case WM_MBUTTONDOWN: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_MBUTTONDOWN);
            if (data->OnMouseDown)
                data->OnMouseDown(data->UserData, GetMousePosition(lParam), MouseCode::Button2);
            break;
        }
        case WM_MBUTTONUP: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_MBUTTONUP);
            if (data->OnMouseUp)
                data->OnMouseUp(data->UserData, GetMousePosition(lParam), MouseCode::Button2);
            break;
        }
        case WM_XBUTTONDOWN: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_XBUTTONDOWN);
            if (data->OnMouseDown)
                data->OnMouseDown(data->UserData, GetMousePosition(lParam), GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? MouseCode::Button3 : MouseCode::Button4);
            break;
        }
        case WM_XBUTTONUP: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_XBUTTONUP);
            if (data->OnMouseUp)
                data->OnMouseUp(data->UserData, GetMousePosition(lParam), GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? MouseCode::Button3 : MouseCode::Button4);
            break;
        }
        case WM_MOUSEWHEEL: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_MOUSEWHEEL);
            if (data->OnMouseWheel)
                data->OnMouseWheel(data->UserData, GetMousePosition(lParam), ScrollOffset(0.0f, GET_WHEEL_DELTA_WPARAM(wParam)));
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

            LIMIT_EVENT(WM_MOUSEMOVE);
            if (data->OnMouseMove)
                data->OnMouseMove(data->UserData, GetMousePosition(lParam));
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
        case WM_KEYDOWN: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_KEYDOWN);
            Modifier modifier = 0;
            if (GetKeyState(VK_SHIFT) & 0x8000)
                modifier |= 0x01;
            if (GetKeyState(VK_CONTROL) & 0x8000)
                modifier |= 0x02;
            if (GetKeyState(VK_MENU) & 0x8000)
                modifier |= 0x04;
            if (GetKeyState(VK_LWIN) & 0x8000 || GetKeyState(VK_RWIN) & 0x8000)
                modifier |= 0x08;
            bool repeat = lParam & (1 << 30);
            if (data->OnKeyDown)
                data->OnKeyDown(data->UserData, ConvertFromVirtualKey(wParam), modifier, repeat);
            if (data->OnKeyTyped)
            {
                auto c = MapVirtualKeyA(wParam, MAPVK_VK_TO_CHAR);
                // Convert to char
                if (c >= 32 && c <= 126)
                    data->OnKeyTyped(data->UserData, static_cast<char>(c), modifier);
            }
            break;
        }
        case WM_KEYUP: {
            auto* data = (WindowsWindow::Data*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            LIMIT_EVENT(WM_KEYDOWN);
            Modifier modifier = 0;
            if (GetKeyState(VK_SHIFT) & 0x8000)
                modifier |= 0x01;
            if (GetKeyState(VK_CONTROL) & 0x8000)
                modifier |= 0x02;
            if (GetKeyState(VK_MENU) & 0x8000)
                modifier |= 0x04;
            if (GetKeyState(VK_LWIN) & 0x8000 || GetKeyState(VK_RWIN) & 0x8000)
                modifier |= 0x08;
            if (data->OnKeyUp)
                data->OnKeyUp(data->UserData, ConvertFromVirtualKey(wParam), modifier);
            break;
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
