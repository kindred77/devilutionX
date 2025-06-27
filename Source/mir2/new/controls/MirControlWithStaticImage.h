#pragma once
#include "MirControlWithTexture.h"
#include "MirImage.h"
#include "ControlCommonListener.h"

class MirControlWithStaticImage : public MirControlWithTexture {
private:
    bool isUseOffSet = true;
    ControlCommonListener* useOffSetChanged = nullptr;

    bool isPixelDetect = false;
    ControlCommonListener* pixelDetectChanged = nullptr;

    MirImage* image = nullptr;
    ControlCommonListener* imageChanged = nullptr;

public:
    MirControlWithStaticImage(MirControl* parent, long renderer_id, MirImage* image);

    Point getLocation() const override;

    bool getIsUseOffSet() const;
    void setIsUseOffSet(bool isUseOffSet);

    void setIsPixelDetect(bool isPixelDetect);

    MirImage* getImage() const;
    void setImage(MirImage* image);

    bool isMouseOver(const Point& posInParent) const override;

    ~MirControlWithStaticImage();
};
