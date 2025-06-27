#include "MirControlWithDynamicImagesMouseEventDriven.h"
#include "MirControl.h"
#include "CommonEvent.h"
#include "MirEngine.h" // 假设这是你封装 SDL 渲染的引擎类

MirControlWithDynamicImagesMouseEventDriven::MirControlWithDynamicImagesMouseEventDriven(
        MirControl* parent, long renderer_id,
        const std::vector<std::shared_ptr<MirImage>>& images)
        : MirControlWithDynamicImagesEventDriven(parent, renderer_id, images),
          hoverImage(nullptr), pressedImage(nullptr),
          hoverImageChanged(nullptr), pressedImageChanged(nullptr)
{
    if (images.size() >= 2) {
        this->hoverImage = images[1];
        this->pressedImage = images[2];
    }

    // 注册鼠标事件
    registerEvent(EventEnum::MouseLeave, [this]() { return images[0]; });
    registerEvent(EventEnum::MouseLeftUp, [this]() { return images[0]; });
    registerEvent(EventEnum::MouseEnter, [this]() { return images[1]; });
    registerEvent(EventEnum::MouseLeftDown, [this]() { return images[2]; });

    // 绑定事件回调
    this->onMouseEnter = [this](MirControl* control, void* arg) {
        auto img = getEventImage(EventEnum::MouseEnter);
        if (img) {
            updateTexture(img->getSurface(), false);
        }
    };

    this->onMouseLeftDown = [this](MirControl* control, void* arg) {
        auto img = getEventImage(EventEnum::MouseLeftDown);
        if (img) {
            updateTexture(img->getSurface(), false);
        }
    };

    this->onMouseLeftUp = [this](MirControl* control, void* arg) {
        auto img = getEventImage(EventEnum::MouseLeftUp);
        if (img) {
            updateTexture(img->getSurface(), false);
        }
    };

    this->onMouseLeave = [this](MirControl* control, void* arg) {
        auto img = getEventImage(EventEnum::MouseLeave);
        if (img) {
            updateTexture(img->getSurface(), false);
        }
    };
}

std::shared_ptr<MirImage> MirControlWithDynamicImagesMouseEventDriven::getHoverImage() const {
    return hoverImage;
}

void MirControlWithDynamicImagesMouseEventDriven::setHoverImage(std::shared_ptr<MirImage> image) {
    if (hoverImage == image) return;
    hoverImage = image;
    if (hoverImageChanged) {
        hoverImageChanged->doAction(this, nullptr);
    }
}

std::shared_ptr<MirImage> MirControlWithDynamicImagesMouseEventDriven::getPressedImage() const {
    return pressedImage;
}

void MirControlWithDynamicImagesMouseEventDriven::setPressedImage(std::shared_ptr<MirImage> image) {
    if (pressedImage == image) return;
    pressedImage = image;
    if (pressedImageChanged) {
        pressedImageChanged->doAction(this, nullptr);
    }
}
