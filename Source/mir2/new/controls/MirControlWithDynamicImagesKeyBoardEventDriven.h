#pragma once
#include "MirControlWithDynamicImagesEventDriven.h"

class MirControlWithDynamicImagesKeyBoardEventDriven : public MirControlWithDynamicImagesEventDriven {
public:
    explicit MirControlWithDynamicImagesKeyBoardEventDriven(
            MirControl* parent, long renderer_id,
            const std::vector<std::shared_ptr<MirImage>>& images);
};