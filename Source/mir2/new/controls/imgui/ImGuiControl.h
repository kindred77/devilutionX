#pragma once
#include "MirControlCanBeDrawn.h"
#include "Point.h"
#include "Size.h"

class ImGuiLayout;

class ImGuiControl : public MirControlCanBeDrawn {
public:
    // 构造函数
    ImGuiControl(MirControl* parent, long window_id, long renderer_id, const std::string& label, const Point& location);

    // 获取 Label
    const std::string& getLabel() const;

    // 强制返回父级为 ImGuiLayout
    ImGuiLayout* getParent() const;

    // 特殊处理绝对坐标：如果是 ImGuiWindow 的子控件，则使用相对坐标
    Point getAbsoluteLocation() const override;

protected:
    long getWindowId() const { return window_id; }
    long getRendererId() const { return renderer_id; }

private:
    long window_id;
    long renderer_id;
    std::string label;
};
