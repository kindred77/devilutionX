#pragma once

#include <string>
#include <memory>
#include <vector>
#include <SDL.h>

class MyRandomAccessFile;

enum class ImageEffect {
    None,
    RedEffect,
    GreenEffect,
    BlueEffect,
    YellowEffect,
    GrayEffect
};

class MirImage {
public:
    explicit MirImage(MyRandomAccessFile& reader, int index);
    ~MirImage();

    SDL_Surface* getSurface(ImageEffect effect) const;
    Point getOffset() const;

    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Size getTrueSize() const { return Size(width, height); }

private:
    mutable SDL_Surface* surfaceNone = nullptr;
    mutable SDL_Surface* surfaceRed = nullptr;
    mutable SDL_Surface* surfaceGreen = nullptr;
    mutable SDL_Surface* surfaceBlue = nullptr;
    mutable SDL_Surface* surfaceYellow = nullptr;
    mutable SDL_Surface* surfaceGray = nullptr;

    Point offset;
    int width = 0;
    int height = 0;

    void applyColorEffect(SDL_Surface* src, SDL_Surface* dst, Uint32 colorMask) const;
};
