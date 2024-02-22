#include "FrameLimiter.hpp"

#include <thread>

#ifdef PULSARION_WINDOWING_USE_HIGH_RES_SLEEP
#include <Windows.h>
#endif

namespace Pulsarion::Windowing
{
    #ifdef PULSARION_WINDOWING_USE_HIGH_RES_SLEEP
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    std::uint32_t FrameLimiter::s_InstanceCount = 0;
    #endif

    FrameLimiter::FrameLimiter(std::uint32_t targetFps)
        : m_TargetFps(targetFps), m_FrameTime(std::chrono::milliseconds(1000 / m_TargetFps)), m_LastFrameTime(std::chrono::steady_clock::now())
    {
        #ifdef PULSARION_WINDOWING_USE_HIGH_RES_SLEEP
        if (s_InstanceCount == 0)
            timeBeginPeriod(1);
        s_InstanceCount++;
        #endif
    }

    FrameLimiter::~FrameLimiter()
    {
        #ifdef PULSARION_WINDOWING_USE_HIGH_RES_SLEEP
        s_InstanceCount--;
        if (s_InstanceCount == 0)
            timeEndPeriod(1);
        #endif
    }

    void FrameLimiter::StartFrame()
    {
        m_LastFrameTime = std::chrono::steady_clock::now();
    }

    void FrameLimiter::EndFrame()
    {
        if (m_TargetFps >= 100'000)
            return; // No need to limit frame rate if it's too high
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_LastFrameTime);

        #ifdef PULSARION_WINDOWING_USE_HIGH_RES_SLEEP
        if (elapsedTime < m_FrameTime)
        {
            #ifdef PULSARION_WINDOWING_USE_BUSY_WAIT
            auto sleepTime = m_FrameTime - elapsedTime - std::chrono::milliseconds(1);
            #else
            auto sleepTime = m_FrameTime - elapsedTime;
            #endif
            if (sleepTime > std::chrono::milliseconds(1))
                Sleep(static_cast<DWORD>(sleepTime.count()));

            #ifdef PULSARION_WINDOWING_USE_BUSY_WAIT
            auto end = currentTime + sleepTime;
            while (std::chrono::steady_clock::now() < end)
            {
                // Busy wait
            }
            #endif
        }
        #elif defined(PULSARION_WINDOWING_USE_BUSY_WAIT)
        if (elapsedTime < m_FrameTime)
        {
            auto sleepTime = m_FrameTime - elapsedTime;
            if (sleepTime > std::chrono::milliseconds(16)) // Windows has a limit of 15ms for sleep time
                std::this_thread::sleep_for(sleepTime);
            else
            {
                auto end = currentTime + sleepTime;
                while (std::chrono::steady_clock::now() < end)
                {
                    // Busy wait
                }
            }
        }
        #else
        if (elapsedTime < m_FrameTime)
            std::this_thread::sleep_for(m_FrameTime - elapsedTime);
        #endif
    }
}
