#pragma once

#include "Core.hpp"

#include <chrono>

namespace Pulsarion::Windowing
{
    class PULSARION_WINDOWING_API FrameLimiter
    {
    public:
        explicit FrameLimiter(std::uint32_t targetFps);
        ~FrameLimiter();

        void StartFrame();
        void EndFrame(); // This is where it will sleep if needed

        void SetTargetFps(std::uint32_t targetFps) { m_TargetFps = targetFps; m_FrameTime = std::chrono::milliseconds(1000 / m_TargetFps); }
        [[nodiscard]] std::uint32_t GetTargetFps() const { return m_TargetFps; }
        [[nodiscard]] std::chrono::milliseconds GetFrameTime() const { return m_FrameTime; }

    private:
        std::uint32_t m_TargetFps;
        std::chrono::milliseconds m_FrameTime;
        std::chrono::time_point<std::chrono::steady_clock> m_LastFrameTime;

        #ifdef PULSARION_WINDOWING_USE_HIGH_RES_SLEEP
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        static std::uint32_t s_InstanceCount; // We use this so we don't prematurely release the high resolution sleep timer when destroying FrameLimiters
        #endif
    };
}
