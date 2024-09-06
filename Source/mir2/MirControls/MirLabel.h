#pragma once

#include "mir2/MirControls/MirLabel.h"
#include "mir2/Common/Color.h"
#include "mir2/Common/Size.h"

#include <string>

namespace devilution {

class MirLabel;
using MirLabelPtr = std::shared_ptr<MirLabel>;

class MirLabel : public MirControl
{
private:
    bool isAutoSize_ = false;
    bool isOutLine_ = false;
    int drawFormat_ = -1;
    int font_ = -1;
    ColorPtr outLineColor_ = nullptr;
    std::string text_;
protected:
    void OnAutoSizeChanged(/* EventArgs e */);
    void OnDrawFormatChanged(/* EventArgs e */);
    void OnFontChanged(/* EventArgs e */);
    void OnOutLineChanged(/* EventArgs e */);
    void OnOutLineColourChanged();
public:
    void SetIsAutoSize(bool isAutoSize);
    bool GetIsAutoSize() const;

    void SetDrawFormat(int drawFormat);
    int GetDrawFormat();

    void SetFont(int font);
    int GetFont();

    void SetIsOutLine(int isOutLine);
    int GetIsOutLine();

    void SetOutLineColor(ColorPtr outLineColor);
    int GetOutLineColor();

    void RefreshSize();

    void SetText(str::string text);
    std::string GetText();
}

}
