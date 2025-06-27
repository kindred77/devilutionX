#include "ImGuiWindow.h"
#include "imgui.h"
#include "Util.h"  // 假设 Util.toCstyleBytes 对应为字符串转 char*
#include "Point.h"
#include "Size.h"
#include "Rectangle.h"

ImGuiWindow::ImGuiWindow(ImGuiLayout* parent, SDL_Window* window, SDL_Renderer* renderer,
                         const std::string& label, const Point& location, const Size& size,
                         bool hasTitlebar, bool movable, bool resizable, bool hasBackground, bool canBringToFront)
        : ImGuiControl(parent, reinterpret_cast<long>(window), reinterpret_cast<long>(renderer), label, location),
          isHasTitlebar(hasTitlebar),
          isMovable(movable),
          isResizable(resizable),
          isHasBackground(hasBackground),
          isCanBringToFront(canBringToFront)
{
    setSize(size);
}

bool ImGuiWindow::beginDraw() {
    Rectangle absoluteRect = getAbsoluteRectangle();

    // 将布尔值转换为 ImGui 的标志位
    ImGuiWindowFlags flags = ImGuiWindowFlags_None;

    if (!isHasTitlebar)       flags |= ImGuiWindowFlags_NoTitleBar;
    if (!isMovable)           flags |= ImGuiWindowFlags_NoMove;
    if (!isResizable)         flags |= ImGuiWindowFlags_NoResize;
    if (!isHasBackground)     flags |= ImGuiWindowFlags_NoBackground;
    if (!isCanBringToFront)   flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    // 开始窗口
    bool isVisible = ImGui::Begin(Util::toCstyleBytes(getLabel()), nullptr, flags);

    if (!isVisible) {
        ImGui::End();
        return false;
    }

    return true;
}

bool ImGuiWindow::endDraw() {
    ImGui::End();
    return true;
}
