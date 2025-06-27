#pragma once
#include "MirControlWithDynamicImagesEventDriven.h"

class ControlCommonListener;

class MirControlWithDynamicImagesMouseEventDriven : public MirControlWithDynamicImagesEventDriven {
protected:
    std::shared_ptr<MirImage> hoverImage;
    ControlCommonListener* hoverImageChanged;

    std::shared_ptr<MirImage> pressedImage;
    ControlCommonListener* pressedImageChanged;

public:
    MirControlWithDynamicImagesMouseEventDriven(
            MirControl* parent, long renderer_id,
            const std::vector<std::shared_ptr<MirImage>>& images);

    std::shared_ptr<MirImage> getHoverImage() const;
    void setHoverImage(std::shared_ptr<MirImage> image);

    std::shared_ptr<MirImage> getPressedImage() const;
    void setPressedImage(std::shared_ptr<MirImage> image);

    // 示例：绑定鼠标事件处理
    void onMouseEnter(MirControl* control, void* arg);
    void onMouseLeftDown(MirControl* control, void* arg);
    void onMouseLeftUp(MirControl* control, void* arg);
    void onMouseLeave(MirControl* control, void* arg);
};
