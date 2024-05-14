#pragma once

#include "mir2/MirControls/MirLabel.h"
#include "mir2/MirControls/MirImageControl.h"
#include "mir2/Common/Color.h"

namespace devilution {

class MirButton;
using MirButtonPtr = std::shared_ptr<MirButton>;

class MirButton : public MirImageControl
{
private:
    MirLabelPtr label_ = nullptr;
    int hoverIndex_ = -1;
    int pressedIndex_ = -1;
    bool isCenter_ = false;
protected:
    void OnHoverIndexChanged();
    void OnPressedIndexChanged();
    void OnSizeChanged() override;
public:
    MirButton();
    void SetForeColor(ColorPtr foreColor) override;

	ColorPtr GetForeColor() const override;

    int GetHoverIndex();
    void SetHoverIndex(int hoverIndex);

    int GetIndex() override;

    void SetPressedIndex(int pressedIndex);

    int GetPressedIndex();

    bool GetIsCenter();
    void SetIsCenter(bool isCenter);

    void SetText(std::string text);
}

}
