#pragma once

#include "../Window.hpp"

namespace Pulsarion::Windowing
{
    class CocoaWindow : public Window
    {
    public:
        explicit CocoaWindow(std::string title, const WindowBounds& bounds, const WindowStyles& styles, const WindowConfig& config);
        ~CocoaWindow() override;

        void SetVisible(bool visible) override;
        void PollEvents() override;
        [[nodiscard]] bool ShouldClose() const override;
        void SetShouldClose(bool shouldClose) override;
        void SetTitle(const std::string& title) override;
        [[nodiscard]] std::optional<std::string> GetTitle() const override;
        [[nodiscard]] void* GetNativeWindow() const override;

        void LimitEvents(bool limit) override;
        [[nodiscard]] bool IsLimitingEvents() const override;

        void SetOnClose(CloseCallback&& callback) override;
        [[nodiscard]] CloseCallback GetOnClose() const override;
        void SetOnWindowVisibility(VisibilityCallback&& callback) override;
        [[nodiscard]] VisibilityCallback GetOnWindowVisibility() const override;
        void SetOnFocus(FocusCallback&& callback) override;
        [[nodiscard]] FocusCallback GetOnFocus() const override;
        void SetOnResize(ResizeCallback&& callback) override;
        [[nodiscard]] ResizeCallback GetOnResize() const override;
        void SetOnMove(MoveCallback&& callback) override;
        [[nodiscard]] MoveCallback GetOnMove() const override;
        void SetBeforeResize(BeforeResizeCallback&& callback) override;
        [[nodiscard]] BeforeResizeCallback GetBeforeResize() const override;
        void SetOnMinimize(MinimizeCallback&& callback) override;
        [[nodiscard]] MinimizeCallback GetOnMinimize() const override;
        void SetOnMaximize(MaximizeCallback&& callback) override;
        [[nodiscard]] MaximizeCallback GetOnMaximize() const override;
        void SetOnRestore(RestoreCallback&& callback) override;
        [[nodiscard]] RestoreCallback GetOnRestore() const override;
        void SetOnMouseEnter(MouseEnterCallback&& callback) override;
        [[nodiscard]] MouseEnterCallback GetOnMouseEnter() const override;

        void SetUserData(void* userData) override;
        [[nodiscard]] void* GetUserData() const override;


    private:
        class Impl;
        Impl* m_Impl;
    };
}
