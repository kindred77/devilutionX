#include "MirControlWithStaticImage.h"
#include "MirJNI.h"

MirControlWithStaticImage::MirControlWithStaticImage(MirControl* parent, long renderer_id, MirImage* image)
        : MirControlWithTexture(parent, renderer_id), image(image) {
    if (!image) {
        throw std::runtime_error("Image is null!");
    }

    foreColor = Color::White;
    this->image = image;
    setSize(image->getTrueSize());
    setLocation(this->Center());

    auto surface = image->getSurface(MirImage::ImageEffect::None);
    updateTexture(surface, false);
}

Point MirControlWithStaticImage::getLocation() const {
    if (isUseOffSet && image != nullptr) {
        return Point::add(MirControlWithTexture::getLocation(), image->getOffset());
    } else {
        return MirControlWithTexture::getLocation();
    }
}

bool MirControlWithStaticImage::getIsUseOffSet() const {
    return isUseOffSet;
}

void MirControlWithStaticImage::setIsUseOffSet(bool isUseOffSet) {
    if (this->isUseOffSet == isUseOffSet) {
        return;
    }
    this->isUseOffSet = isUseOffSet;
    onUseOffSetChanged();
}

void MirControlWithStaticImage::setIsPixelDetect(bool isPixelDetect) {
    if (this->isPixelDetect == isPixelDetect) {
        return;
    }
    this->isPixelDetect = isPixelDetect;
    onPixelDetectChanged();
}

MirImage* MirControlWithStaticImage::getImage() const {
    return image;
}

void MirControlWithStaticImage::setImage(MirImage* image) {
    if (this->image == image) {
        return;
    }
    this->image = image;
    if (image != nullptr) {
        auto surface = image->getSurface(MirImage::ImageEffect::None);
        updateTexture(surface, false);
    }
    onImageChanged();
}

bool MirControlWithStaticImage::isMouseOver(const Point& posInParent) const {
    if (!MirControlWithTexture::isMouseOver(posInParent)) {
        return false;
    }

    if (image == nullptr) {
        return !isPixelDetect;
    }

    Point localPos = Point::subtract(posInParent, getLocation());
    return !isPixelDetect || image->isVisiblePixel(localPos);
}

void MirControlWithStaticImage::onUseOffSetChanged() {
    onLocationChanged();
    if (useOffSetChanged != nullptr) {
        useOffSetChanged->doAction(this, nullptr);
    }
}

void MirControlWithStaticImage::onPixelDetectChanged() {
    if (pixelDetectChanged != nullptr) {
        pixelDetectChanged->doAction(this, nullptr);
    }
}

void MirControlWithStaticImage::onImageChanged() {
    onSizeChanged();
    if (imageChanged != nullptr) {
        imageChanged->doAction(this, nullptr);
    }
}

MirControlWithStaticImage::~MirControlWithStaticImage() {
    delete image;
    image = nullptr;

    pixelDetectChanged = nullptr;
    useOffSetChanged = nullptr;
    imageChanged = nullptr;
}
