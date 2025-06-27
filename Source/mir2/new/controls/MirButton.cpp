#include "MirButton.h"
#include "MirLabel.h"
#include "SoundManager.h"

MirButton::MirButton(MirControl* parent, long renderer_id,
                     std::shared_ptr<MirImage> normalImage,
                     std::shared_ptr<MirImage> hoverImage,
                     std::shared_ptr<MirImage> pressedImage,
                     const Size& size, const Point& pos)
        : MirControlWithDynamicImagesMouseEventDriven(parent, renderer_id,
                                                      {normalImage, hoverImage, pressedImage})
{
    if (!normalImage) {
        setSize(size);
    }
    setLocation(pos);
    sound = SoundList::ButtonB;
}

MirButton::MirButton(MirControl* parent, long renderer_id,
                     std::shared_ptr<MirImage> normalImage,
                     std::shared_ptr<MirImage> hoverImage,
                     std::shared_ptr<MirImage> pressedImage,
                     const std::string& text,
                     const Size& labelSize, Font* font,
                     const Color& foreColor, const Color& backColor, int wraplength)
        : MirControlWithDynamicImagesMouseEventDriven(parent, renderer_id,
                                                      {normalImage, hoverImage, pressedImage})
{
    if (!normalImage) {
        throw std::runtime_error("Normal image can not be null!");
    }

    sound = SoundList::ButtonB;

    textLabel = std::make_unique<MirLabel>(this, renderer_id, labelSize, Point(0, 0),
                                           font, text, foreColor, backColor, wraplength);
    textLabel->setIsByPassEvent(true);
    textLabel->setLocation(textLabel->Center());
}

MirButton::~MirButton() {
    dispose(true);
}

std::shared_ptr<MirImage> MirButton::getHoverImage() const {
    return hoverImage;
}

void MirButton::setHoverImage(std::shared_ptr<MirImage> image) {
    if (this->hoverImage == image) return;
    this->hoverImage = image;
    onHoverImageChanged();
}

std::shared_ptr<MirImage> MirButton::getPressedImage() const {
    return pressedImage;
}

void MirButton::setPressedImage(std::shared_ptr<MirImage> image) {
    if (this->pressedImage == image) return;
    this->pressedImage = image;
    onPressedImageChanged();
}

void MirButton::onHoverImageChanged() {
    if (hoverImageChanged) {
        hoverImageChanged(this, nullptr);
    }
}

void MirButton::onPressedImageChanged() {
    if (pressedImageChanged) {
        pressedImageChanged(this, nullptr);
    }
}

std::string MirButton::getText() const {
    if (textLabel) {
        return textLabel->getText();
    }
    return "";
}

void MirButton::setText(const std::string& text) {
    if (textLabel) {
        textLabel->setText(text);
        textLabel->setIsVisible(!text.empty());
    }
}

void MirButton::setOnMouseLeftClick(const ControlCommonListener& listener) {
    onMouseLeftClick = listener;
}

void MirButton::dispose(bool disposing) {
    MirControlWithDynamicImagesMouseEventDriven::dispose(disposing);

    if (!disposing) return;

    textLabel.reset();
    hoverImage.reset();
    pressedImage.reset();
}
