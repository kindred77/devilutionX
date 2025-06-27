#pragma once
#include "MirControlWithDynamicImages.h"
#include "CommonEvent.h"
#include <unordered_map>

class MirControlWithDynamicImagesEventDriven : public MirControlWithDynamicImages {
private:
    std::unordered_map<EventEnum, ImageProvider> eventActionMap;

public:
    explicit MirControlWithDynamicImagesEventDriven(
            MirControl* parent, long renderer_id,
            const std::vector<std::shared_ptr<MirImage>>& images);

    void registerEvent(EventEnum event, ImageProvider provider);

    std::shared_ptr<MirImage> getEventImage(EventEnum event);
};