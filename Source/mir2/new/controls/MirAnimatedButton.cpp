#include "MirAnimatedButton.h"
#include "MirMain.h" // 提供 MirMain::Time
#include "MirImage.h"
#include "MirControlWithDynamicImagesMouseEventDriven.h"

MirAnimatedButton::MirAnimatedButton(MirControl* parent, long renderer_id,
                                     const std::vector<std::shared_ptr<MirImage>>& animImages,
                                     const std::shared_ptr<MirImage>& normalImage,
                                     const std::shared_ptr<MirImage>& hoverImage,
                                     const std::shared_ptr<MirImage>& pressedImage,
                                     const Size& size, const Point& pos)
        : MirAnimatedControl(parent, renderer_id, animImages, false, 200),
          images(animImages) {

    if (!images.empty()) {
        this->animationCount = static_cast<int>(images.size());
    }

    this->isLoop = true;
    this->nextFadeTime = MirMain::Time;
    this->nextOffSet = MirMain::Time;

    // 鼠标进入时重置偏移
    onMouseEnter = [this](MirControl* control, void* obj) {
        setOffSet(0);
    };
}

// ========== Getters & Setters ==========

bool MirAnimatedButton::getIsAnimated() const {
    return isAnimated;
}

void MirAnimatedButton::setIsAnimated(bool isAnimated) {
    if (this->isAnimated == isAnimated) return;
    this->isAnimated = isAnimated;
    this->nextOffSet = MirMain::Time + fadeInDelay;
    onAnimatedChanged();
}

int MirAnimatedButton::getAnimationCount() const {
    return animationCount;
}

void MirAnimatedButton::setAnimationCount(int count) {
    if (this->animationCount == count) return;
    this->animationCount = count;
    onAnimationCountChanged();
}

long MirAnimatedButton::getAnimationDelay() const {
    return animationDelay;
}

void MirAnimatedButton::setAnimationDelay(long delay) {
    if (this->animationDelay == delay) return;
    this->animationDelay = delay;
    onAnimationDelayChanged();
}

bool MirAnimatedButton::getIsFadeIn() const {
    return isFadeIn;
}

void MirAnimatedButton::setIsFadeIn(bool fadeIn) {
    if (this->isFadeIn == fadeIn) return;
    this->nextFadeTime = MirMain::Time + fadeInDelay;
    this->isFadeIn = fadeIn;
    onFadeInChanged();
}

float MirAnimatedButton::getFadeInRate() const {
    return fadeInRate;
}

void MirAnimatedButton::setFadeInRate(float rate) {
    if (this->fadeInRate == rate) return;
    this->fadeInRate = rate;
    onFadeInRateChanged();
}

long MirAnimatedButton::getFadeInDelay() const {
    return fadeInDelay;
}

void MirAnimatedButton::setFadeInDelay(long delay) {
    if (this->fadeInDelay == delay) return;
    this->fadeInDelay = delay;
    onFadeInDelayChanged();
}

bool MirAnimatedButton::getIsLoop() const {
    return isLoop;
}

void MirAnimatedButton::setIsLoop(bool loop) {
    if (this->isLoop == loop) return;
    this->isLoop = loop;
    onLoopChanged();
}

int MirAnimatedButton::getOffSet() const {
    return offSet;
}

void MirAnimatedButton::setOffSet(int offset) {
    if (this->offSet == offset) return;
    this->offSet = offset;
    onOffSetChanged();
}

void MirAnimatedButton::setAfterAnimation(const ControlCommonListener& listener) {
    afterAnimation = listener;
}

// ========== Update Logic ==========

void MirAnimatedButton::updateOffSet() {
    if (isFadeIn && MirMain::Time > nextFadeTime) {
        opacity += fadeInRate;
        if (opacity > 1.0f) {
            opacity = 1.0f;
            isFadeIn = false;
        }
        nextFadeTime = MirMain::Time + fadeInDelay;
    }

    if (!getIsVisible() || !isAnimated || animationDelay == 0 || animationCount == 0) {
        return;
    }

    if (MirMain::Time < nextOffSet) {
        return;
    }

    nextOffSet = MirMain::Time + animationDelay;
    setOffSet(offSet + 1);

    if (offSet < animationCount) {
        return;
    }

    if (!isLoop) {
        setIsAnimated(false);
    } else {
        setOffSet(0);
    }

    if (afterAnimation) {
        afterAnimation(this, nullptr);
    }
}

// ========== Callbacks ==========

void MirAnimatedButton::onAnimatedChanged() {
    if (animatedChanged) {
        animatedChanged(this, nullptr);
    }
}

void MirAnimatedButton::onAnimationCountChanged() {
    if (animationCountChanged) {
        animationCountChanged(this, nullptr);
    }
}

void MirAnimatedButton::onAnimationDelayChanged() {
    if (animationDelayChanged) {
        animationDelayChanged(this, nullptr);
    }
}

void MirAnimatedButton::onFadeInChanged() {
    if (fadeInChanged) {
        fadeInChanged(this, nullptr);
    }
}

void MirAnimatedButton::onFadeInRateChanged() {
    if (fadeInRateChanged) {
        fadeInRateChanged(this, nullptr);
    }
}

void MirAnimatedButton::onFadeInDelayChanged() {
    if (fadeInDelayChanged) {
        fadeInDelayChanged(this, nullptr);
    }
}

void MirAnimatedButton::onLoopChanged() {
    if (loopChanged) {
        loopChanged(this, nullptr);
    }
}

void MirAnimatedButton::onOffSetChanged() {
    // 示例：设置当前图像
    // setMirImage(images[offSet]);
    if (offSetChanged) {
        offSetChanged(this, nullptr);
    }
}