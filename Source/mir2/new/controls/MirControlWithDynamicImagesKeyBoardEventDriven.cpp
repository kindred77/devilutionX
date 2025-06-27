#include "MirControlWithDynamicImagesKeyBoardEventDriven.h"

MirControlWithDynamicImagesKeyBoardEventDriven::MirControlWithDynamicImagesKeyBoardEventDriven(
        MirControl* parent, long renderer_id,
        const std::vector<std::shared_ptr<MirImage>>& images)
        : MirControlWithDynamicImagesEventDriven(parent, renderer_id, images)
{
    // 可以在这里扩展键盘事件处理
}