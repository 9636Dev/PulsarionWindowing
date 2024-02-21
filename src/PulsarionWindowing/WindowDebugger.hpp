#pragma once

#include "Window.hpp"

#include "PulsarionCore/Log.hpp"

#include <memory>
#include <chrono>

namespace Pulsarion::Windowing
{
    struct DebugOptions
    {
        bool LogCalls = false;

        bool LogToggles = false;

        bool LogEvents = false;

        // Adds a logging function for the general state of the window
        bool LogState = false;

        bool LogDeltaTime = false;

        constexpr DebugOptions() = default;
        constexpr DebugOptions(bool logCalls, bool logToggles, bool logEvents, bool logState, bool logDeltaTime) : LogCalls(logCalls), LogToggles(logToggles), LogEvents(logEvents), LogState(logState), LogDeltaTime(logDeltaTime)
        {

        }
    };

    // We use a template so additional debug options won't affect performance
    template<DebugOptions options>
    class DebugWindow : public Window
    {
    protected:
        struct WindowData : WindowEvents
        {
            void* UserData = nullptr;

            WindowData() = default;
        };

        struct DeltaTime
        {
            // We log maximum once per second
            std::chrono::steady_clock::time_point LastLogTime = std::chrono::steady_clock::now();
            std::chrono::steady_clock::time_point LastFrameTime = std::chrono::steady_clock::now();
            std::size_t FrameCount = 0;
            std::size_t TotalTimeMicroseconds = 0;
        };

        void SetDebugCallbacks()
        requires (options.LogEvents)
        {
            m_Window->SetUserData(&m_State);

            m_Window->SetOnClose([](void* data) -> bool
            {
                PULSARION_LOG_TRACE("[Window::OnClose] Window close callback called");
                const auto& state = static_cast<WindowData*>(data);
                if (state->OnClose)
                    return state->OnClose(data);
                return true;
            });

            m_Window->SetOnWindowVisibility([this](void* data, bool visible)
            {
                PULSARION_LOG_TRACE("[Window::OnWindowVisibility] Window visibility callback called with visibility {0}", visible);
                const auto& state = static_cast<WindowData*>(data);
                if (state->OnWindowVisibility)
                    state->OnWindowVisibility(data, visible);
            });

            m_Window->SetOnFocus([this](void* data, bool focused)
            {
                PULSARION_LOG_TRACE("[Window::OnFocus] Window focus callback called with focus {0}", focused);
                const auto& state = static_cast<WindowData*>(data);
                if (state->OnFocus)
                    state->OnFocus(data, focused);
            });

            m_Window->SetOnResize([this](void* data, std::uint32_t width, std::uint32_t height)
            {
                PULSARION_LOG_TRACE("[Window::OnResize] Window resize callback called with width {0} and height {1}", width, height);
                const auto& state = static_cast<WindowData*>(data);
                if (state->OnResize)
                    state->OnResize(data, width, height);
            });

            m_Window->SetOnMove([this](void* data, std::uint32_t x, std::uint32_t y)
            {
                PULSARION_LOG_TRACE("[Window::OnMove] Window move callback called with x {0} and y {1}", x, y);
                const auto& state = static_cast<WindowData*>(data);
                if (state->OnMove)
                    state->OnMove(data, x, y);
            });

            m_Window->SetBeforeResize([this](void* data)
            {
                PULSARION_LOG_TRACE("[Window::BeforeResize] Window before resize callback called");
                const auto& state = static_cast<WindowData*>(data);
                if (state->BeforeResize)
                    state->BeforeResize(data);
            });
        }


        void LogDeltaTime()
        requires (options.LogDeltaTime)
        {
            const auto currentTime = std::chrono::steady_clock::now();
            const auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - m_DeltaTime.LastLogTime).count();
            if (deltaTime <= 1'000'000)
                return;

            const auto realDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - m_DeltaTime.LastLogTime).count() / m_DeltaTime.FrameCount;
            const auto realDeltaTimeMilliseconds = realDeltaTime / 1000.0;
            const auto averageDeltaTime = m_DeltaTime.TotalTimeMicroseconds / m_DeltaTime.FrameCount;
            const auto averageMilliseconds = averageDeltaTime / 1000.0;
            const auto averageFramesPerSecond = 1'000'000.0 / averageDeltaTime;
            const auto realFramesPerSecond = 1'000'000.0 / realDeltaTime;

            PULSARION_LOG_TRACE("[Window::LogDeltaTime] Frame Reporting (last {0}ms):", deltaTime / 1000.0);
            PULSARION_LOG_TRACE("  Real delta time: {0}ms", realDeltaTimeMilliseconds);
            PULSARION_LOG_TRACE("  Real frames per second: {0}", realFramesPerSecond);
            PULSARION_LOG_TRACE("  Application delta time: {0}ms", averageMilliseconds);
            PULSARION_LOG_TRACE("  Application frames per second: {0}", averageFramesPerSecond);

            m_DeltaTime.FrameCount = 0;
            m_DeltaTime.TotalTimeMicroseconds = 0;
            m_DeltaTime.LastLogTime = currentTime;
        }
    public:
        explicit DebugWindow(WindowCreationData& data) : m_Window(CreateSharedWindow(data)), m_State()
        {
            if constexpr (options.LogToggles)
                PULSARION_LOG_TRACE("[Window::Window] Creating window");
            if constexpr (options.LogEvents)
                SetDebugCallbacks();
            if constexpr (options.LogState)
                LogState();
        }
        ~DebugWindow() override = default;

        inline void SetVisible(bool visible) override
        {
            if constexpr (options.LogToggles)
                PULSARION_LOG_TRACE("[Window::SetVisible] Setting window visibility to {0}", visible);
            m_Window->SetVisible(visible);
        }

        inline void PollEvents() override
        {
            if constexpr (options.LogCalls)
                PULSARION_LOG_TRACE("[Window::PollEvents] Polling window events");
            if constexpr (options.LogDeltaTime)
            {
                m_DeltaTime.FrameCount++;
                m_DeltaTime.TotalTimeMicroseconds += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - m_DeltaTime.LastFrameTime).count();
                LogDeltaTime();
                m_DeltaTime.LastFrameTime = std::chrono::steady_clock::now();
            }

            m_Window->PollEvents();
        }

        [[nodiscard]] inline bool ShouldClose() const override
        {
            if constexpr (options.LogCalls)
                PULSARION_LOG_TRACE("[Window::ShouldClose] Checking if window should close");
            return m_Window->ShouldClose();
        }

        inline void SetShouldClose(bool shouldClose) override
        {
            if constexpr (options.LogToggles)
                PULSARION_LOG_TRACE("[Window::SetShouldClose] Setting window should close to {0}", shouldClose);
            m_Window->SetShouldClose(shouldClose);
        }

        inline void SetTitle(const std::string& title) override
        {
            if constexpr (options.LogToggles)
                PULSARION_LOG_TRACE("[Window::SetTitle] Setting window title to {0}", title);
            m_Window->SetTitle(title);
        }



        [[nodiscard]] inline std::optional<std::string> GetTitle() const override
        {
            if constexpr (options.LogCalls)
                PULSARION_LOG_TRACE("[Window::GetTitle] Getting window title");
            return m_Window->GetTitle();
        }

        #ifdef PULSARION_WINDOWING_LIMIT_EVENTS
        void LimitEvents(bool limitEvents) override
        {
            if constexpr (options.LogToggles)
                PULSARION_LOG_TRACE("[Window::LimitEvents] Setting window event limiting to {0}", limitEvents);
            m_Window->LimitEvents(limitEvents);
        }

        [[nodiscard]] bool IsLimitingEvents() const override
        {
            if constexpr (options.LogCalls)
                PULSARION_LOG_TRACE("[Window::IsLimitingEvents] Getting window event limiting");
            return m_Window->IsLimitingEvents();
        }
        #endif

        void LogState() const
        requires (options.LogState)
        {
            PULSARION_LOG_TRACE("[Window::LogState] Window state:");
            PULSARION_LOG_TRACE("  Title: {0}", m_Window->GetTitle().value_or("No title"));
            PULSARION_LOG_TRACE("  Should close: {0}", m_Window->ShouldClose()); // We don't use ShouldClose() because it logs the function call
        }

        // --- Event Callbacks ---

        void SetOnClose(CloseCallback&& onClose) override
        {
            if constexpr (options.LogToggles)
                PULSARION_LOG_TRACE("[Window::SetOnClose] Setting window close callback");
            m_State.OnClose = std::move(onClose);
        }

        [[nodiscard]] CloseCallback GetOnClose() const override
        {
            if constexpr (options.LogCalls)
                PULSARION_LOG_TRACE("[Window::GetOnClose] Getting window close callback");
            if constexpr (!options.LogEvents)
                return m_Window->GetOnClose();
            return m_State.OnClose;
        }

        void SetOnWindowVisibility(VisibilityCallback&& onWindowVisibility) override
        {
            if constexpr (options.LogToggles)
                PULSARION_LOG_TRACE("[Window::SetOnWindowVisibility] Setting window visibility callback");
            if constexpr (!options.LogEvents)
                m_Window->SetOnWindowVisibility(std::move(onWindowVisibility));
            else
                m_State.OnWindowVisibility = std::move(onWindowVisibility);
        }

        [[nodiscard]] VisibilityCallback GetOnWindowVisibility() const override
        {
            if constexpr (options.LogCalls)
                PULSARION_LOG_TRACE("[Window::GetOnWindowVisibility] Getting window visibility callback");
            if constexpr (!options.LogEvents)
                return m_Window->GetOnWindowVisibility();
            return m_State.OnWindowVisibility;
        }

        void SetOnFocus(FocusCallback&& onFocus) override
        {
            if constexpr (options.LogToggles)
                PULSARION_LOG_TRACE("[Window::SetOnFocus] Setting window focus callback");
            if constexpr (!options.LogEvents)
                m_Window->SetOnFocus(std::move(onFocus));
            else
                m_State.OnFocus = std::move(onFocus);
        }

        [[nodiscard]] FocusCallback GetOnFocus() const override
        {
            if constexpr (options.LogCalls)
                PULSARION_LOG_TRACE("[Window::GetOnFocus] Getting window focus callback");
            if constexpr (!options.LogEvents)
                return m_Window->GetOnFocus();
            return m_State.OnFocus;
        }

        void SetOnResize(ResizeCallback&& onResize) override
        {
            if constexpr (options.LogToggles)
                PULSARION_LOG_TRACE("[Window::SetOnResize] Setting window resize callback");
            if constexpr (!options.LogEvents)
                m_Window->SetOnResize(std::move(onResize));
            else
                m_State.OnResize = std::move(onResize);
        }

        [[nodiscard]] ResizeCallback GetOnResize() const override
        {
            if constexpr (options.LogCalls)
                PULSARION_LOG_TRACE("[Window::GetOnResize] Getting window resize callback");
            if constexpr (!options.LogEvents)
                return m_Window->GetOnResize();
            return m_State.OnResize;
        }

        void SetOnMove(MoveCallback&& onMove) override
        {
            if constexpr (options.LogToggles)
                PULSARION_LOG_TRACE("[Window::SetOnMove] Setting window move callback");
            if constexpr (!options.LogEvents)
                m_Window->SetOnMove(std::move(onMove));
            else
                m_State.OnMove = std::move(onMove);
        }

        [[nodiscard]] MoveCallback GetOnMove() const override
        {
            if constexpr (options.LogCalls)
                PULSARION_LOG_TRACE("[Window::GetOnMove] Getting window move callback");
            if constexpr (!options.LogEvents)
                return m_Window->GetOnMove();
            return m_State.OnMove;
        }

        void SetBeforeResize(BeforeResizeCallback&& beforeResize) override
        {
            if constexpr (options.LogToggles)
                PULSARION_LOG_TRACE("[Window::SetBeforeResize] Setting window before resize callback");
            if constexpr (!options.LogEvents)
                m_Window->SetBeforeResize(std::move(beforeResize));
            else
                m_State.BeforeResize = std::move(beforeResize);
        }

        [[nodiscard]] BeforeResizeCallback GetBeforeResize() const override
        {
            if constexpr (options.LogCalls)
                PULSARION_LOG_TRACE("[Window::GetBeforeResize] Getting window before resize callback");
            if constexpr (!options.LogEvents)
                return m_Window->GetBeforeResize();
            return m_State.BeforeResize;
        }

        void SetUserData(void* userData) override
        {
            if constexpr (options.LogToggles)
                PULSARION_LOG_TRACE("[Window::SetUserData] Setting window user data");
            if constexpr (!options.LogEvents)
                m_Window->SetUserData(userData);
            else
                m_State.UserData = userData;
        }

        [[nodiscard]] void* GetUserData() const override
        {
            if constexpr (options.LogToggles)
                PULSARION_LOG_TRACE("[Window::GetUserData] Getting window user data");
            if constexpr (!options.LogEvents)
                return m_Window->GetUserData(); // We don't use the state

            return m_State.UserData;
        }

    private:
        struct None { };
        std::shared_ptr<Window> m_Window;
        WindowData  m_State;
        DeltaTime m_DeltaTime;
    };
}
