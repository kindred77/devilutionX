// MirMap.cpp
#include "MirMap.h"
#include <iostream>
#include <cstring>
#include <stdexcept>

MirMap::MirMap(const std::string& fileName) : fileName(fileName), width(0), height(0) {
    std::ifstream file(fileName + ".map", std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open map file: " + fileName);
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    data.resize(static_cast<size_t>(fileSize));
    if (!file.read(reinterpret_cast<char*>(data.data()), fileSize)) {
        throw std::runtime_error("Failed to read map file: " + fileName);
    }

    // Check for custom map format signature
    if (data.size() >= 4 && data[2] == 0x43 && data[3] == 0x23) {
        LoadMapType100();
        return;
    }

    // Wemade Mir3 maps start with blank bytes
    if (data[0] == 0) {
        LoadMapType5();
        return;
    }

    // Shanda Mir3 maps start with "(C) SNDA, MIR3."
    if (data[0] == 0x0F && data[5] == 0x53 && data[14] == 0x33) {
        LoadMapType6();
        return;
    }

    // AntiHack maps
    if (data[0] == 0x15 && data[4] == 0x32 && data[6] == 0x41 && data[19] == 0x31) {
        LoadMapType4();
        return;
    }

    // 2010 map format
    if (data[0] == 0x10 && data[2] == 'a' && data[7] == '1' && data[14] == '1') {
        LoadMapType1();
        return;
    }

    // Shanda's 2012 format and older formats
    if ((data[4] == 0x0F || data[4] == 0x03) && data[18] == 0x0D && data[19] == 0x0A) {
        int W = Util::ToInt16(data.data(), 0);
        int H = Util::ToInt16(data.data(), 2);
        if (data.size() > (52 + W * H * 14)) {
            LoadMapType3();
        } else {
            LoadMapType2();
        }
        return;
    }

    // 3/4 Heroes map format
    if (data[0] == 0x0D && data[1] == 0x4C && data[7] == 0x20 && data[11] == 0x6D) {
        LoadMapType7();
        return;
    }

    // Default old school format
    LoadMapType0();
}

MirMap::~MirMap() {
    // 清理所有 MapObject
    for (auto& row : mapCells) {
        for (auto& cell : row) {
            for (auto obj : cell.CellObjects) {
                delete obj;
            }
        }
    }
}

const std::string& MirMap::getFileName() const {
    return fileName;
}

int MirMap::getWidth() const {
    return width;
}

int MirMap::getHeight() const {
    return height;
}

std::vector<std::vector<MapCellInfo>>& MirMap::getMapCells() {
    return mapCells;
}

void MirMap::LoadMapType0() {
    size_t offset = 0;
    width = Util::ToInt16(data.data(), offset);
    offset += 2;
    height = Util::ToInt16(data.data(), offset);
    offset = 52;

    mapCells.resize(width, std::vector<MapCellInfo>(height));

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            MapCellInfo& cell = mapCells[x][y];
            cell.BackIndex = 0;
            cell.MiddleIndex = 1;
            cell.BackImage = Util::ToInt16(data.data(), offset);
            offset += 2;
            cell.MiddleImage = Util::ToInt16(data.data(), offset);
            offset += 2;
            cell.FrontImage = Util::ToInt16(data.data(), offset);
            offset += 2;
            cell.DoorIndex = static_cast<byte>(data[offset++] & 0x7F);
            cell.DoorOffset = data[offset++];
            cell.FrontAnimationFrame = data[offset++];
            cell.FrontAnimationTick = data[offset++];
            cell.FrontIndex = static_cast<short>(data[offset++] + 2);
            cell.Light = data[offset++];

            if ((cell.BackImage & 0x8000) != 0) {
                cell.BackImage = (cell.BackImage & 0x7FFF) | 0x20000000;
            }

            if (cell.Light >= 100 && cell.Light <= 119) {
                cell.FishingCell = true;
            }
        }
    }

    std::cout << "Loaded map type: 0, Width: " << width << ", Height: " << height << std::endl;
}

void MirMap::LoadMapType1() {
    size_t offset = 21;
    int w = Util::ToInt16(data.data(), offset);
    offset += 2;
    int xorKey = Util::ToInt16(data.data(), offset);
    offset += 2;
    int h = Util::ToInt16(data.data(), offset);
    width = w ^ xorKey;
    height = h ^ xorKey;
    offset = 54;

    mapCells.resize(width, std::vector<MapCellInfo>(height));

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            MapCellInfo& cell = mapCells[x][y];
            cell.BackImage = Util::ToInt32(data.data(), offset) ^ 0xAA38AA38;
            offset += 4;
            cell.MiddleImage = Util::ToInt16(data.data(), offset) ^ xorKey;
            offset += 2;
            cell.FrontImage = Util::ToInt16(data.data(), offset) ^ xorKey;
            offset += 2;
            cell.DoorIndex = static_cast<byte>(data[offset] & 0x7F);
            offset++;
            cell.DoorOffset = data[offset++];
            cell.FrontAnimationFrame = data[offset++];
            cell.FrontAnimationTick = data[offset++];
            cell.FrontIndex = static_cast<short>(data[offset++] + 2);
            cell.Light = data[offset++];
            cell.Unknown = data[offset++];
            offset++;

            if (cell.Light >= 100 && cell.Light <= 119) {
                cell.FishingCell = true;
            }
        }
    }

    std::cout << "Loaded map type: 1, Width: " << width << ", Height: " << height << std::endl;
}

