#pragma once
#include "ImGuiControl.h"
#include <string>

class ImGuiWindow : public ImGuiControl {
public:
    // 构造函数
    ImGuiWindow(class ImGuiLayout* parent, SDL_Window* window, SDL_Renderer* renderer,
                const std::string& label, const Point& location, const Size& size,
                bool hasTitlebar, bool movable, bool resizable, bool hasBackground, bool canBringToFront);

    // 开始绘制（覆盖父类）
    bool beginDraw() override;

    // 结束绘制（覆盖父类）
    bool endDraw() override;

private:
    bool isHasTitlebar;
    bool isMovable;
    bool isResizable;
    bool isHasBackground;
    bool isCanBringToFront;
};
