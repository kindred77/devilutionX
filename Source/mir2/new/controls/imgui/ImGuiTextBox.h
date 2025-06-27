#pragma once
#include "ImGuiControl.h"
#include <functional>
#include <vector>
#include <string>

class Font;
class Color;
class Point;
class Size;

// 定义回调接口
using ControlCommonListener = std::function<void(class ImGuiTextBox*, const std::string&)>;

class ImGuiTextBox : public ImGuiControl {
public:
    // 构造函数
    ImGuiTextBox(ImGuiWindow* parent, SDL_Window* window, SDL_Renderer* renderer,
                 const std::string& label, const Point& location,
                 int width, int textMaxLength, Font* font,
                 const Color& backColor, const Color& foreColor,
                 bool isPassword, int multiLines);

    // 开始绘制（覆盖父类）
    bool beginDraw() override;

    // 结束绘制（覆盖父类）
    bool endDraw() override;

    // 设置输入完成的回调
    void setOnInputFinished(ControlCommonListener listener);

private:
    Font* font;
    std::vector<char> textBuf;  // 输入缓冲区
    Color backColor;
    Color foreColor;
    bool isPassword;
    int multiLines;

    ControlCommonListener onInputFinished;
};
