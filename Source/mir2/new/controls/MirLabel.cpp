#include "MirLabel.h"
#include "Font.h"
#include "Util.h"  // Assuming generateTextSurface is in Util
#include "MirJNI.h"

MirLabel::MirLabel(MirControl* parent, long renderer_id, const Size& size, const Point& pos,
                   Font* font, const std::string& text, const Color& foreColor, const Color& backColor, int wraplength)
        : MirControlWithTexture(parent, renderer_id),
          drawFormat(0),  // WordBreak
          isOutLine(true),
          outLineColor(Color::Black),
          text(text),
          wraplength(wraplength),
          font(font),
          foreColor(foreColor),
          backColor(backColor)
{
    setLocation(pos);
    setSize(size);

    // Generate surface from text
    long font_surface = Font::generateTextSurface(this->font, this->text, this->foreColor, this->backColor, this->wraplength);
    updateTexture(font_surface, true);
}

bool MirLabel::getIsAutoSize() const {
    return isAutoSize;
}

void MirLabel::setIsAutoSize(bool isAutoSize) {
    if (this->isAutoSize == isAutoSize) return;
    this->isAutoSize = isAutoSize;
    onAutoSizeChanged();
}

const std::string& MirLabel::getText() const {
    return text;
}

void MirLabel::setText(const std::string& text) {
    if (this->text == text) return;
    this->text = text;

    long font_surface = Font::generateTextSurface(font, this->text, foreColor, backColor, wraplength);
    updateTexture(font_surface, true);

    onTextChanged();
}

Font* MirLabel::getFont() const {
    return font;
}

void MirLabel::setFont(Font* font) {
    if (this->font == font) return;
    this->font = font;
    onFontChanged();
}

int MirLabel::getDrawFormat() const {
    return drawFormat;
}

void MirLabel::setDrawFormat(int drawFormat) {
    if (this->drawFormat == drawFormat) return;
    this->drawFormat = drawFormat;
    onDrawFormatChanged();
}

bool MirLabel::getIsOutLine() const {
    return isOutLine;
}

void MirLabel::setIsOutLine(bool isOutLine) {
    if (this->isOutLine == isOutLine) return;
    this->isOutLine = isOutLine;
    onOutLineChanged();
}

const Color& MirLabel::getOutLineColor() const {
    return outLineColor;
}

void MirLabel::setOutLineColor(const Color& color) {
    if (outLineColor == color) return;
    outLineColor = color;
    onOutLineChanged();
}

void MirLabel::onAutoSizeChanged() {
    updateSize();
    if (onAutoSizeChangedCallback) {
        onAutoSizeChangedCallback(this, nullptr);
    }
}

void MirLabel::onTextChanged() {
    isDrawControlTexture = !text.empty();
    updateSize();
    if (onTextChangedCallback) {
        onTextChangedCallback(this, nullptr);
    }
}

void MirLabel::onFontChanged() {
    updateSize();
    if (onFontChangedCallback) {
        onFontChangedCallback(this, nullptr);
    }
}

void MirLabel::onOutLineChanged() {
    updateSize();
    if (onOutLineChangedCallback) {
        onOutLineChangedCallback(this, nullptr);
    }
}

void MirLabel::onDrawFormatChanged() {
    if (onDrawFormatChangedCallback) {
        onDrawFormatChangedCallback(this, nullptr);
    }
}

void MirLabel::updateSize() {
    if (!isAutoSize) return;

    if (text.empty()) {
        setSize(Size::Empty);
    } else {
        // Simulate MeasureText logic
        // You may need to call SDL_ttf or similar functions to measure text
        // Example: SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(...);
        // setSize(Size(surface->w, surface->h));
        // SDL_FreeSurface(surface);
    }

    if (isOutLine && getSize() != Size::Empty) {
        setSize(getSize() + Size(2, 2));  // Add outline margin
    }
}

void MirLabel::dispose(bool disposing) {
    MirControlWithTexture::dispose(disposing);

    if (!disposing) return;

    onAutoSizeChangedCallback = nullptr;
    onTextChangedCallback = nullptr;
    onFontChangedCallback = nullptr;
    onOutLineChangedCallback = nullptr;
    onDrawFormatChangedCallback = nullptr;

    font = nullptr;
}
