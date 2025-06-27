#pragma once
#include "MirControlWithStaticImage.h"
#include "UserItem.h"  // 假设 UserItem 已经被转换为 C++

class MirItemCell : public MirControlWithStaticImage {
public:
    MirItemCell(MirControl* parent, long renderer_id, MirImage* image);

    // 获取用户物品信息
    UserItem getItem() const;
};
