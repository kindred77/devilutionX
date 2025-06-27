#pragma once
#include "MirControlWithDynamicImagesEventDriven.h"
#include "MirControl.h"
#include "CommonEvent.h"
#include "ControlCommonListener.h"

class MirControlWithDynamicImagesTimeDriven : public MirControlWithDynamicImagesEventDriven {
private:
    bool isLoop = false;
    long animationDelay = 200;
    long nextIndexTime = 0;
    bool isAnimated = false;
    int index = 0;

    ControlCommonListener* afterAnimation = nullptr;

public:
    MirControlWithDynamicImagesTimeDriven(MirControl* parent, const std::vector<std::shared_ptr<MirImage>>& images,
                                          bool isLoop, long animationDelay);

    void update();
    void setIsAnimated(bool isAnimated);
    void setAfterAnimation(ControlCommonListener* listener);

    // Override
    bool drawControl() override;
};
