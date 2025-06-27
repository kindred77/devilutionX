#include "MirAnimatedControl.h"
#include "MirMain.h" // 提供 MirMain::Time
#include "MirImage.h"

MirAnimatedControl::MirAnimatedControl(MirControl* parent, long renderer_id,
                                       const std::vector<std::shared_ptr<MirImage>>& images,
                                       bool isLoop, long animationDelay)
        : MirControlWithDynamicImagesTimeDriven(parent, renderer_id, images, isLoop, animationDelay)
{
}

bool MirAnimatedControl::getIsFadeIn() const {
    return isFadeIn;
}

void MirAnimatedControl::setIsFadeIn(bool fadeIn) {
    if (this->isFadeIn == fadeIn) return;
    this->nextFadeTime = MirMain::Time + fadeInDelay;
    this->isFadeIn = fadeIn;
    onFadeInChanged();
}

float MirAnimatedControl::getFadeInRate() const {
    return fadeInRate;
}

void MirAnimatedControl::setFadeInRate(float rate) {
    if (this->fadeInRate == rate) return;
    this->fadeInRate = rate;
    onFadeInRateChanged();
}

long MirAnimatedControl::getFadeInDelay() const {
    return fadeInDelay;
}

void MirAnimatedControl::setFadeInDelay(long delay) {
    if (this->fadeInDelay == delay) return;
    this->fadeInDelay = delay;
    onFadeInDelayChanged();
}

void MirAnimatedControl::setAfterAnimation(const ControlCommonListener& listener) {
    afterAnimation = listener;
}

void MirAnimatedControl::onFadeInChanged() {
    if (fadeInChanged != nullptr) {
        fadeInChanged(this, nullptr);
    }
}

void MirAnimatedControl::onFadeInRateChanged() {
    if (fadeInRateChanged != nullptr) {
        fadeInRateChanged(this, nullptr);
    }
}

void MirAnimatedControl::onFadeInDelayChanged() {
    if (fadeInDelayChanged != nullptr) {
        fadeInDelayChanged(this, nullptr);
    }
}

void MirAnimatedControl::dispose(bool disposing) {
    MirControlWithDynamicImagesTimeDriven::dispose(disposing);

    if (!disposing) return;

    afterAnimation = nullptr;
    isLoop = false;
}
