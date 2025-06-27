#include "ImGuiControl.h"
#include "ImGuiLayout.h"
#include "ImGuiWindow.h"
#include <stdexcept>

ImGuiControl::ImGuiControl(MirControl* parent, long window_id, long renderer_id, const std::string& label, const Point& location)
        : MirControlCanBeDrawn(parent, renderer_id), window_id(window_id), renderer_id(renderer_id), label(label) {

    // 父检查逻辑
    if (parent == nullptr ||
        (!dynamic_cast<ImGuiLayout*>(parent) &&
         !dynamic_cast<ImGuiWindow*>(parent) &&
         !dynamic_cast<ImGuiLayout*>(this))) {
        throw std::runtime_error("ImGui control must be in ImGuiLayout or ImGuiWindow, only the ImGuiLayout can be in MirControl!");
    }

    setLocation(location);
}

const std::string& ImGuiControl::getLabel() const {
    return label;
}

ImGuiLayout* ImGuiControl::getParent() const {
    return dynamic_cast<ImGuiLayout*>(MirControlCanBeDrawn::getParent());
}

Point ImGuiControl::getAbsoluteLocation() const {
    if (auto* parentWindow = dynamic_cast<ImGuiWindow*>(getParent())) {
        return MirControl::getLocation(); // 相对于窗口的位置
    } else {
        return MirControl::getAbsoluteLocation(); // 使用 MirControl 默认的绝对位置
    }
}
