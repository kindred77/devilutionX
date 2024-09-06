#include "mir2/MirControls/MirLabel.h"

namespace devilution {

void MirLabel::SetIsAutoSize(bool isAutoSize)
{
    if (isAutoSize_ == isAutoSize) return;
    isAutoSize_ = isAutoSize;
    OnAutoSizeChanged(/* EventArgs.Empty */);
}

void MirLabel::OnAutoSizeChanged(/* EventArgs e */)
{
    isTextureValid_ = false;
    GetSize();
    //if (AutoSizeChanged != null)
    //    AutoSizeChanged.Invoke(this, e);
}

bool MirLabel::GetIsAutoSize() const
{
    return isAutoSize_;
}

void MirLabel::OnDrawFormatChanged(/* EventArgs e */)
{
    isTextureValid_ = false;

    // if (DrawFormatChanged != null)
    //     DrawFormatChanged.Invoke(this, e);
}

void MirLabel::SetDrawFormat(int drawFormat)
{
    if (drawFormat_ == drawFormat) return;
    drawFormat_ = drawFormat;
    OnDrawFormatChanged(/* EventArgs.Empty */);
}

int MirLabel::GetDrawFormat()
{
    return drawFormat_;
}

void MirLabel::OnFontChanged(/* EventArgs e */)
{
    isTextureValid_ = false;

    GetSize();

    // if (FontChanged != null)
    //     FontChanged.Invoke(this, e);
}

void MirLabel::SetFont(int font)
{
    if (font_ == font) return;
    font_ = font;
    OnFontChanged(/* EventArgs.Empty */);
}

int MirLabel::GetFont()
{
    return font_;
}

void MirLabel::OnOutLineChanged(/* EventArgs e */)
{
    isTextureValid_ = false;
    GetSize();
            
    // if (OutLineChanged != null)
    //     OutLineChanged.Invoke(this, e);
}

void MirLabel::SetIsOutLine(int isOutLine)
{
    if (isOutLine_ == isOutLine) return;
    isOutLine_ = isOutLine;
    OnOutLineChanged(/* EventArgs.Empty */);
}

int MirLabel::GetIsOutLine()
{
    return isOutLine_;
}

void MirLabel::OnOutLineColourChanged()
{
    isTextureValid_ = false;

    // if (OutLineColourChanged != null)
    //     OutLineColourChanged.Invoke(this, EventArgs.Empty);
}

void MirLabel::SetOutLineColor(ColorPtr outLineColor)
{
    if (outLineColor_ == outLineColor) return;
    outLineColor_ = outLineColor;
    OnOutLineColourChanged();
}

int MirLabel::GetOutLineColor()
{
    return outLineColor_;
}

void MirLabel::RefreshSize()
{
    if (!GetIsAutoSize()) return;

    if (text_.empty())
    {
        SetSize(Size::EMPTY);
    }
    else
    {
        Size = TextRenderer.MeasureText(CMain.Graphics, Text, Font);

        if (GetIsOutLine() && GetSize() != Size::EMPTY)
        {
            auto orig_size = GetSize();
            SetSize(orig_size->width + 2, orig_size->height + 2);
        }
    }
}

void MirLabel::SetText(str::string text)
{
    if (_text == value)
                    return;

                _text = value;
                OnTextChanged(EventArgs.Empty);
}

std::string MirLabel::GetText()
{
    return text_;
}

}
