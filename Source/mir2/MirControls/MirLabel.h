#pragma once

#include "mir2/MirControls/MirLabel.h"

namespace devilution {

class MirLabel;
using MirLabelPtr = std::shared_ptr<MirLabel>;

class MirLabel : public MirControl
{
private:
    bool isAutoSize_ = false;
protected:
    void OnAutoSizeChanged(/* EventArgs e */);
public:
    void SetIsAutoSize(bool isAutoSize);
    bool GetIsAutoSize();
}

}
