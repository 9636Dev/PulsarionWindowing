#pragma once

#include "../Window.hpp"

namespace Pulsarion::Windowing
{
    class CocoaWindow : public Window
    {
    public:
        explicit CocoaWindow(WindowCreationData& creationData);
        ~CocoaWindow() override;

        void SetVisible(bool visible) override;
        void PollEvents() override;
        [[nodiscard]] bool ShouldClose() const override;
    private:
        class Impl;
        Impl* m_Impl;
    };
}
