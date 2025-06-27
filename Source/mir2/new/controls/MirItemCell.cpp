#include "MirItemCell.h"

MirItemCell::MirItemCell(MirControl* parent, long renderer_id, MirImage* image)
        : MirControlWithStaticImage(parent, renderer_id, image) {}

UserItem MirItemCell::getItem() const {
    return UserItem::builder().build();  // 假设 UserItem::builder() 已经适配 C++
}
