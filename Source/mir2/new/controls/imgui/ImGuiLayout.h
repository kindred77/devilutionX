#pragma once
#include "ImGuiControl.h"
#include <SDL.h>

class ImGuiLayout : public ImGuiControl {
public:
    // 构造函数
    ImGuiLayout(MirControl* parent, SDL_Window* window, SDL_Renderer* renderer);

    // 开始绘制
    bool beginDraw() override;

    // 结束绘制
    bool endDraw() override;

    // 获取 ImGui 上下文
    void* getImGuiContext() const;

private:
    void* imgui_context;
    SDL_Window* window;
    SDL_Renderer* renderer;
};
