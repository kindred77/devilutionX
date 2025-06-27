#pragma once
#include "MirAnimatedControl.h"
#include <vector>
#include <memory>
#include <functional>

class MirImage;

using ControlCommonListener = std::function<void(class MirControl*, void*)>;

class MirAnimatedButton : public MirAnimatedControl {
public:
    explicit MirAnimatedButton(MirControl* parent, long renderer_id,
                               const std::vector<std::shared_ptr<MirImage>>& animImages,
                               const std::shared_ptr<MirImage>& normalImage,
                               const std::shared_ptr<MirImage>& hoverImage,
                               const std::shared_ptr<MirImage>& pressedImage,
                               const Size& size, const Point& pos);

    // 属性 Getters / Setters
    bool getIsAnimated() const;
    void setIsAnimated(bool isAnimated);

    int getAnimationCount() const;
    void setAnimationCount(int count);

    long getAnimationDelay() const;
    void setAnimationDelay(long delay);

    bool getIsFadeIn() const;
    void setIsFadeIn(bool fadeIn);

    float getFadeInRate() const;
    void setFadeInRate(float rate);

    long getFadeInDelay() const;
    void setFadeInDelay(long delay);

    bool getIsLoop() const;
    void setIsLoop(bool loop);

    int getOffSet() const;
    void setOffSet(int offset);

    void updateOffSet();

    // 设置动画结束后回调
    void setAfterAnimation(const ControlCommonListener& listener);

protected:
    void onAnimatedChanged();
    void onAnimationCountChanged();
    void onAnimationDelayChanged();
    void onFadeInChanged();
    void onFadeInRateChanged();
    void onFadeInDelayChanged();
    void onLoopChanged();
    void onOffSetChanged();

private:
    bool isAnimated = false;
    int animationCount = 0;
    long animationDelay = 200;
    long nextFadeTime = 0;
    bool isFadeIn = false;
    float fadeInRate = 0.1f;
    long fadeInDelay = 0;
    ControlCommonListener afterAnimation;
    bool isLoop = true;
    long nextOffSet = 0;
    int offSet = 0;
    std::vector<std::shared_ptr<MirImage>> images;
};