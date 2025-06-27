#pragma once
#include "MirControlCanBeDrawn.h"
#include "MirTexture.h"
#include "SDL.h"

class MirControlWithTexture : public MirControlCanBeDrawn {
public:
    explicit MirControlWithTexture(MirControl* parent, SDL_Renderer* renderer);

    bool updateTexture(SDL_Surface* surface, bool ifReleaseSurface);
    virtual bool drawControl() override;

protected:
    MirTexture controlTexture;
};