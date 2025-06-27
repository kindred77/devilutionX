#pragma once
#include "MirControlWithDynamicImagesTimeDriven.h"
#include <functional>

// 使用 std::function 替代 Java 的 ControlCommonListener
using ControlCommonListener = std::function<void(MirControl*, void*)>;

class MirAnimatedControl : public MirControlWithDynamicImagesTimeDriven {
public:
    explicit MirAnimatedControl(MirControl* parent, long renderer_id,
                                const std::vector<std::shared_ptr<MirImage>>& images,
                                bool isLoop, long animationDelay);

    // Getter / Setter
    bool getIsFadeIn() const;
    void setIsFadeIn(bool fadeIn);

    float getFadeInRate() const;
    void setFadeInRate(float rate);

    long getFadeInDelay() const;
    void setFadeInDelay(long delay);

    void setAfterAnimation(const ControlCommonListener& listener);
    void dispose(bool disposing) override;

protected:
    void onFadeInChanged();
    void onFadeInRateChanged();
    void onFadeInDelayChanged();

private:
    long nextFadeTime = 0;
    bool isFadeIn = false;
    float fadeInRate = 0.1f;
    long fadeInDelay = 0;
    ControlCommonListener afterAnimation;
};