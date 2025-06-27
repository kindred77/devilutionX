MirControlWithTexture::MirControlWithTexture(MirControl* parent, void* renderer_id)
        : MirControlCanBeDrawn(parent, renderer_id) {}

bool MirControlWithTexture::updateTexture(void* surface_id, bool ifReleaseSurface) {
    std::lock_guard<std::mutex> lock(textureMutex);
    controlTexture.update(renderer_id, surface_id, ifReleaseSurface);

    if (getIsBorder()) {
        updateBorderInfo();
    }

    return true;
}

bool MirControlWithTexture::beginDraw() {
    return true;
}

bool MirControlWithTexture::endDraw() {
    return true;
}

bool MirControlWithTexture::drawControl() {
    std::lock_guard<std::mutex> lock(textureMutex);

    if (controlTexture.isValid()) {
        if (getIsBlending()) {
            SDL_SetTextureBlendMode(controlTexture.getTexture(), SDL_BLENDMODE_BLEND);
        }

        Rectangle dstRect = getAbsoluteRectangle();
        SDL_Rect sdlRect = {dstRect.getX(), dstRect.getY(), dstRect.getWidth(), dstRect.getHeight()};
        SDL_RenderCopy((SDL_Renderer*)renderer_id, controlTexture.getTexture(), nullptr, &sdlRect);
    }

    return true;
}