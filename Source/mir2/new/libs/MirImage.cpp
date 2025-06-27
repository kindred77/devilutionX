#include "MirImage.h"
#include "MyRandomAccessFile.h"
#include <SDL.h>
#include <stdexcept>
#include <iostream>

MirImage::MirImage(MyRandomAccessFile& reader, int index) {
    // 读取偏移量（假设索引对应固定结构）
    reader.seek(index * sizeof(int)); // 示例：每个图像记录为4字节偏移
    int offsetPos = reader.readIntLE();
    reader.seek(offsetPos);

    // 读取图像头信息
    width = reader.readShortLE();
    height = reader.readShortLE();
    offset.x = reader.readShortLE();
    offset.y = reader.readShortLE();

    // 读取图像数据长度
    int dataSize = reader.readIntLE();
    std::vector<uint8_t> pixelData(dataSize);
    reader.read(pixelData.data(), dataSize);

    // 创建原始图像表面
    SDL_RWops* rw = SDL_RWFromMem(pixelData.data(), static_cast<int>(pixelData.size()));
    if (!rw) {
        throw std::runtime_error("Failed to create SDL_RWops");
    }

    surfaceNone = SDL_LoadBMP_RW(rw, 1);
    if (!surfaceNone) {
        throw std::runtime_error("Failed to load image from data: " + std::string(SDL_GetError()));
    }

    // 初始化其他效果的图像表面
    surfaceRed = SDL_ConvertSurface(surfaceNone, surfaceNone->format, 0);
    applyColorEffect(surfaceNone, surfaceRed, 0xFF0000FF); // 红色滤镜

    surfaceGreen = SDL_ConvertSurface(surfaceNone, surfaceNone->format, 0);
    applyColorEffect(surfaceNone, surfaceGreen, 0x00FF00FF); // 绿色滤镜

    surfaceBlue = SDL_ConvertSurface(surfaceNone, surfaceNone->format, 0);
    applyColorEffect(surfaceNone, surfaceBlue, 0x0000FFFF); // 蓝色滤镜

    surfaceYellow = SDL_ConvertSurface(surfaceNone, surfaceNone->format, 0);
    applyColorEffect(surfaceNone, surfaceYellow, 0xFFFF00FF); // 黄色滤镜

    surfaceGray = SDL_ConvertSurface(surfaceNone, surfaceNone->format, 0);
    applyColorEffect(surfaceNone, surfaceGray, 0x010101FF); // 灰度处理
}

MirImage::~MirImage() {
    SDL_FreeSurface(surfaceNone);
    SDL_FreeSurface(surfaceRed);
    SDL_FreeSurface(surfaceGreen);
    SDL_FreeSurface(surfaceBlue);
    SDL_FreeSurface(surfaceYellow);
    SDL_FreeSurface(surfaceGray);
}

SDL_Surface* MirImage::getSurface(ImageEffect effect) const {
    switch (effect) {
        case ImageEffect::None: return surfaceNone;
        case ImageEffect::RedEffect: return surfaceRed;
        case ImageEffect::GreenEffect: return surfaceGreen;
        case ImageEffect::BlueEffect: return surfaceBlue;
        case ImageEffect::YellowEffect: return surfaceYellow;
        case ImageEffect::GrayEffect: return surfaceGray;
    }
    return surfaceNone;
}

void MirImage::applyColorEffect(SDL_Surface* src, SDL_Surface* dst, Uint32 colorMask) const {
    if (!src || !dst || src->format->BytesPerPixel != 4 || dst->format->BytesPerPixel != 4) {
        return;
    }

    Uint32* srcPixels = static_cast<Uint32*>(src->pixels);
    Uint32* dstPixels = static_cast<Uint32*>(dst->pixels);
    int totalPixels = src->w * src->h;

    for (int i = 0; i < totalPixels; ++i) {
        Uint32 pixel = srcPixels[i];
        Uint32 r = (pixel & 0xFF000000) >> 24;
        Uint32 g = (pixel & 0x00FF0000) >> 16;
        Uint32 b = (pixel & 0x0000FF00) >> 8;
        Uint32 a = (pixel & 0x000000FF);

        // 应用颜色掩码
        r = (r * ((colorMask & 0xFF000000) >> 24)) / 255;
        g = (g * ((colorMask & 0x00FF0000) >> 16)) / 255;
        b = (b * ((colorMask & 0x0000FF00) >> 8)) / 255;

        dstPixels[i] = (r << 24) | (g << 16) | (b << 8) | a;
    }
}
