#pragma once
#include "MirControlWithTexture.h"
#include "Font.h"
#include "Color.h"
#include <string>
#include <functional>

class Font;
class ControlCommonListener;

class MirLabel : public MirControlWithTexture {
public:
    using ControlCommonListener = std::function<void(MirControl*, void*)>;

    MirLabel(MirControl* parent, long renderer_id, const Size& size, const Point& pos,
             Font* font, const std::string& text, const Color& foreColor, const Color& backColor, int wraplength);

    // Getter and Setter for AutoSize
    bool getIsAutoSize() const;
    void setIsAutoSize(bool isAutoSize);

    // Getter and Setter for Text
    const std::string& getText() const;
    void setText(const std::string& text);

    // Getter and Setter for Font
    Font* getFont() const;
    void setFont(Font* font);

    // Getter and Setter for DrawFormat (if needed)
    // For simplicity, assuming it's just stored as an int or enum
    int getDrawFormat() const;
    void setDrawFormat(int drawFormat);

    // Getter and Setter for Outline
    bool getIsOutLine() const;
    void setIsOutLine(bool isOutLine);

    // Getter and Setter for Outline Color
    const Color& getOutLineColor() const;
    void setOutLineColor(const Color& color);

    // Event handlers
    void onAutoSizeChanged();
    void onTextChanged();
    void onFontChanged();
    void onOutLineChanged();
    void onDrawFormatChanged();

    // Dispose method to clean up resources
    void dispose(bool disposing) override;

private:
    void updateSize();  // Adjust size based on content if auto-size is enabled

    bool isAutoSize = false;
    std::string text;
    int wraplength = 0;
    Font* font = nullptr;
    Color foreColor;
    Color backColor;
    Color outLineColor;
    bool isOutLine = false;
    int drawFormat = 0;  // e.g., WordBreak

    ControlCommonListener onAutoSizeChangedCallback = nullptr;
    ControlCommonListener onTextChangedCallback = nullptr;
    ControlCommonListener onFontChangedCallback = nullptr;
    ControlCommonListener onOutLineChangedCallback = nullptr;
    ControlCommonListener onDrawFormatChangedCallback = nullptr;
};
