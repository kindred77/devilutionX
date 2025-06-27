#pragma once
#include "MirControlWithTexture.h"
#include <vector>
#include <memory>

class MirImage;

class MirControlWithDynamicImages : public MirControlWithTexture {
protected:
    std::vector<std::shared_ptr<MirImage>> images;

public:
    explicit MirControlWithDynamicImages(MirControl* parent, long renderer_id,
                                         const std::vector<std::shared_ptr<MirImage>>& images);

    virtual ~MirControlWithDynamicImages() = default;

    const std::vector<std::shared_ptr<MirImage>>& getImages() const;
    void setImages(const std::vector<std::shared_ptr<MirImage>>& images);

    void updateWithCurrentImage();
};