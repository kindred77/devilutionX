#include "MirControlWithDynamicImagesTimeDriven.h"
#include <SDL.h>
#include <iostream>

MirControlWithDynamicImagesTimeDriven::MirControlWithDynamicImagesTimeDriven(
        MirControl* parent,
        const std::vector<std::shared_ptr<MirImage>>& images,
        bool isLoop, long animationDelay)
        : MirControlWithDynamicImagesEventDriven(parent, images), isLoop(isLoop), animationDelay(animationDelay) {
    registerEvent(CommonEvent::TimeInterval, [this]() -> std::shared_ptr<MirImage> {
        if (index >= 0 && index < static_cast<int>(images.size())) {
            return images[index];
        }
        return nullptr;
    });
}

void MirControlWithDynamicImagesTimeDriven::update() {
    if (!getIsVisible() || !isAnimated || animationDelay == 0 || images.empty()) {
        return;
    }

    long time = SDL_GetTicks();
    if (time < nextIndexTime) {
        return;
    }

    nextIndexTime = time + animationDelay;
    fireOffsetEvent();
}

void MirControlWithDynamicImagesTimeDriven::fireOffsetEvent() {
    index++;
    if (index < static_cast<int>(images.size())) {
        auto img = getEventImage(CommonEvent::TimeInterval);
        if (img) {
            updateTexture(img->getSurface(), false);
        } else {
            updateTexture(nullptr, false);
        }
        return;
    }

    // 动画结束
    ControlCommonListener* temp = afterAnimation;
    afterAnimation = nullptr;

    if (!isLoop) {
        isAnimated = false;
    } else {
        index = 0;
    }

    if (temp != nullptr) {
        temp->doAction(this, nullptr);
    }
}

void MirControlWithDynamicImagesTimeDriven::setIsAnimated(bool isAnimated) {
    if (this->isAnimated == isAnimated) {
        return;
    }
    this->isAnimated = isAnimated;
    nextIndexTime = SDL_GetTicks() + animationDelay;
}

void MirControlWithDynamicImagesTimeDriven::setAfterAnimation(ControlCommonListener* listener) {
    afterAnimation = listener;
}

bool MirControlWithDynamicImagesTimeDriven::drawControl() {
    update();
    return MirControlWithDynamicImagesEventDriven::drawControl();
}
