#include "mir2/MirControls/MirButton.h"
#include "mir2/Engine/SoundList.h"

namespace devilution {

MirButton::MirButton()
{
    MirButton::SetSound(SoundList.ButtonB);
    label_ = std::make_shared<MirLabel>();
    new MirLabel
        {
            NotControl = true,
            Parent = this,
            //Font = new Font("Constantia", 8, FontStyle.Italic),
            //OutLine = true,
            //OutLineColour = Color.FromArgb(255, 70, 50, 30),
        };
}

void MirButton::SetForeColor(ColorPtr foreColor)
{
    if (!label_ && !label_->GetIsDisposed())
        label_->SetForeColor(foreColor);
}

ColorPtr MirButton::GetForeColor() const
{
    if (!label_ && !label_->GetIsDisposed()) return label_->GetForeColor();
    return nullptr;
}

int MirButton::GetHoverIndex()
{
    return hoverIndex_;
}

void MirButton::SetHoverIndex(int hoverIndex)
{
    if (hoverIndex_ == hoverIndex) return;
    hoverIndex_ = hoverIndex;
    OnHoverIndexChanged();
}

void MirButton::OnHoverIndexChanged()
{
    //if (HoverIndexChanged != null)
    //    HoverIndexChanged.Invoke(this, EventArgs.Empty);
}

int MirButton::GetIndex()
{
    if (!GetIsEnabled()) return MirImageControl::GetIndex();

    if (pressedIndex_ >= 0 && MirControl::ActiveControl == shared_from_this() && MirControl::MouseControl == shared_from_this())
        return pressedIndex_;

    if (hoverIndex_ >= 0 && MirControl::MouseControl == shared_from_this())
        return hoverIndex_;

    return MirImageControl::GetIndex();
}

void MirButton::SetPressedIndex(int pressedIndex)
{
    if (pressedIndex_ == pressedIndex) return;
    pressedIndex_ = pressedIndex;
    OnPressedIndexChanged();
}

 int MirButton::GetPressedIndex()
 {
    return pressedIndex_;
 }

 void MirButton::OnPressedIndexChanged()
 {
    //if (PressedIndexChanged != null)
    //    PressedIndexChanged.Invoke(this, EventArgs.Empty);
 }

bool MirButton::GetIsCenter()
{
    return isCenter_;
}

void MirButton::SetIsCenter(bool isCenter)
{
    isCenter_ = isCenter;
    if (isCenter_)
    {
        label_->SetSize(GetSize());
        label_->SetDrawFormat(TextFormatFlags.HorizontalCenter | TextFormatFlags.VerticalCenter);
    }
    else
        label_->SetAutoSize(true);
}

void MirButton::OnSizeChanged()
{
    MirControl::OnSizeChanged();

    if (label_ && !label_->GetIsDisposed())
        label_->SetSize(GetSize());
}

void MirButton::SetText(std::string text)
{
    if (!label_ || label_->GetIsDisposed()) return;
    label_->SetText(text);
    label_->SetIsVisible() = !text.Empty();
}

}
