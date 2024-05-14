#include "mir2/MirControls/MirLabel.h"

namespace devilution {

void MirLabel::SetIsAutoSize(bool isAutoSize)
{
    if (isAutoSize_ == isAutoSize) return;
    isAutoSize_ = isAutoSize;
    OnAutoSizeChanged(EventArgs.Empty);
}

void MirLabel::OnAutoSizeChanged(/* EventArgs e */)
{
    isTextureValid_ = false;
    GetSize();
    //if (AutoSizeChanged != null)
    //    AutoSizeChanged.Invoke(this, e);
}

bool MirLabel::GetIsAutoSize()
{
    return isAutoSize_;
}

}
