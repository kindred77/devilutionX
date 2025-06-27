#include "ImGuiTextBox.h"
#include "imgui.h"
#include "Util.h"  // 假设 Util.toCstyleBytes 对应为字符串转 char*
#include "Font.h"
#include "Color.h"

ImGuiTextBox::ImGuiTextBox(ImGuiWindow* parent, SDL_Window* window, SDL_Renderer* renderer,
                           const std::string& label, const Point& location,
                           int width, int textMaxLength, Font* font,
                           const Color& backColor, const Color& foreColor,
                           bool isPassword, int multiLines)
        : ImGuiControl(parent, 0, 0, label, location),
          font(font),
          textBuf(textMaxLength + 1, '\0'),
          backColor(backColor),
          foreColor(foreColor),
          isPassword(isPassword),
          multiLines(multiLines)
{
    if (font == nullptr || font->getImGuiFontID() == 0) {
        throw std::invalid_argument("Invalid font.");
    }
    if (multiLines <= 0) {
        throw std::invalid_argument("Invalid multiLines: " + std::to_string(multiLines));
    }
    if (multiLines > 1 && isPassword) {
        throw std::invalid_argument("Do not support password textbox with multilines yet.");
    }

    setSize(Size(width, 0));

    // 初始化背景和前景颜色
    ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImVec4(
            backColor.getRedF(), backColor.getGreenF(), backColor.getBlueF(), backColor.getAlphaF());
    ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(
            foreColor.getRedF(), foreColor.getGreenF(), foreColor.getBlueF(), foreColor.getAlphaF());
}

bool ImGuiTextBox::beginDraw() {
    ImGui::PushFont(reinterpret_cast<ImFont*>(font->getImGuiFontID()));

    Point pos = getAbsoluteLocation();
    int width = getSize().getWidth();

    bool valueChanged = false;

    if (multiLines <= 1) {
        valueChanged = ImGui::InputText(
                Util::toCstyleBytes(getLabel()), textBuf.data(), textBuf.size(),
                isPassword ? ImGuiInputTextFlags_Password : ImGuiInputTextFlags_None,
                nullptr, nullptr);
    } else {
        valueChanged = ImGui::InputTextMultiline(
                Util::toCstyleBytes(getLabel()), textBuf.data(), textBuf.size(),
                ImVec2(static_cast<float>(width), static_cast<float>(multiLines * 20)),  // 简单高度估算
                ImGuiInputTextFlags_None, nullptr, nullptr);
    }

    if (valueChanged && onInputFinished) {
        onInputFinished(this, Util::zeroEndBytesToString(textBuf.data()));
    }

    return true;
}

bool ImGuiTextBox::endDraw() {
    ImGui::PopFont();
    return true;
}

void ImGuiTextBox::setOnInputFinished(ControlCommonListener listener) {
    onInputFinished = listener;
}
