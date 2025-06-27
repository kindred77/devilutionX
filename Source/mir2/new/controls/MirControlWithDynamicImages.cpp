#include "MirControlWithDynamicImages.h"

MirControlWithDynamicImages::MirControlWithDynamicImages(
        MirControl* parent, long renderer_id,
        const std::vector<std::shared_ptr<MirImage>>& images)
        : MirControlWithTexture(parent, renderer_id), images(images)
{
    if (!images.empty() && images[0]) {
        setSize(images[0]->getTrueSize());
        setLocation(Center());
        updateWithCurrentImage();
    }
}

const std::vector<std::shared_ptr<MirImage>>& MirControlWithDynamicImages::getImages() const {
    return images;
}

void MirControlWithDynamicImages::setImages(const std::vector<std::shared_ptr<MirImage>>& newImages) {
    images = newImages;
    if (!images.empty() && images[0]) {
        setSize(images[0]->getTrueSize());
        setLocation(Center());
        updateWithCurrentImage();
    }
}

void MirControlWithDynamicImages::updateWithCurrentImage() {
    if (images.empty()) return;

    auto surface = images[0]->getSurface(MirImage::ImageEffect::None);
    updateTexture(surface, false);
}
