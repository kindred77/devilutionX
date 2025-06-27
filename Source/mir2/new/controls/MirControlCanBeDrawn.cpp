#include "MirControlCanBeDrawn.h"

MirControlCanBeDrawn::MirControlCanBeDrawn(MirControl* parent, RendererID renderer_id)
        : MirControl(parent), renderer_id(renderer_id) {
    isDrawControlTexture = true;
}

void MirControlCanBeDrawn::drawBorder() {
    updateBorderInfo();
    if (borderInfo.empty() || borderInfo.size() % 2 != 0) return;

    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer_id, &r, &g, &b, &a);

    SDL_SetRenderDrawColor(renderer_id, borderColor.getRed(), borderColor.getGreen(),
                           borderColor.getBlue(), borderColor.getAlpha());

    for (size_t i = 0; i < borderInfo.size(); i += 2) {
        SDL_RenderDrawLine(renderer_id,
                           static_cast<int>(borderInfo[i].getX()),
                           static_cast<int>(borderInfo[i].getY()),
                           static_cast<int>(borderInfo[i + 1].getX()),
                           static_cast<int>(borderInfo[i + 1].getY()));
    }

    SDL_SetRenderDrawColor(renderer_id, r, g, b, a); // Restore original color
}

void MirControlCanBeDrawn::updateBorderInfo() {
    if (size == Size::Empty()) {
        borderInfo.clear();
        borderRectangle = Rectangle::Empty();
        return;
    }

    Rectangle displayRect = getAbsoluteRectangle();

    if (!borderRectangle.equals(displayRect)) {
        borderInfo = {
                Vector2(displayRect.getLeft() - 1, displayRect.getTop() - 1),
                Vector2(displayRect.getRight(), displayRect.getTop() - 1),

                Vector2(displayRect.getLeft() - 1, displayRect.getTop() - 1),
                Vector2(displayRect.getLeft() - 1, displayRect.getBottom()),

                Vector2(displayRect.getLeft() - 1, displayRect.getBottom()),
                Vector2(displayRect.getRight(), displayRect.getBottom()),

                Vector2(displayRect.getRight(), displayRect.getTop() - 1),
                Vector2(displayRect.getRight(), displayRect.getBottom())
        };

        borderRectangle = displayRect;
    }
}

