#include "FrameLimiter.hpp"

#include <thread>

namespace Pulsarion::Windowing
{
    FrameLimiter::FrameLimiter(std::uint32_t targetFps)
        : m_TargetFps(targetFps), m_FrameTime(std::chrono::milliseconds(1000 / m_TargetFps)), m_LastFrameTime(std::chrono::steady_clock::now())
    {
    }

    void FrameLimiter::StartFrame()
    {
        m_LastFrameTime = std::chrono::steady_clock::now();
    }

    void FrameLimiter::EndFrame()
    {
        // TODO: Make more sophisticated frame limiter, based on actual frame time and average of last 10 frames, so that it can adapt to changing frame times by decreasing or increasing the sleep time
        if (m_TargetFps >= 100'000)
            return; // No need to limit frame rate if it's too high
        std::chrono::time_point<std::chrono::steady_clock> currentTime = std::chrono::steady_clock::now();
        std::chrono::milliseconds elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_LastFrameTime);

        if (elapsedTime < m_FrameTime)
            std::this_thread::sleep_for(m_FrameTime - elapsedTime);
    }
}
