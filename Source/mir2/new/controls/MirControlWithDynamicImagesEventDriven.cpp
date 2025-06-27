#include "MirControlWithDynamicImagesEventDriven.h"

MirControlWithDynamicImagesEventDriven::MirControlWithDynamicImagesEventDriven(
        MirControl* parent, long renderer_id,
        const std::vector<std::shared_ptr<MirImage>>& images)
        : MirControlWithDynamicImages(parent, renderer_id, images)
{
}

void MirControlWithDynamicImagesEventDriven::registerEvent(
        EventEnum event, ImageProvider provider)
{
    eventActionMap[event] = provider;
}

std::shared_ptr<MirImage> MirControlWithDynamicImagesEventDriven::getEventImage(EventEnum event)
{
    auto it = eventActionMap.find(event);
    if (it != eventActionMap.end()) {
        auto img = it->second();
        if (img) {
            return img;
        }
    }
    return images.empty() ? nullptr : images[0];
}
