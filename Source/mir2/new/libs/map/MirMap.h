// MirMap.h
#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "MapCellInfo.h"

class MirMap {
private:
    int width;
    int height;
    std::vector<std::vector<MapCellInfo>> mapCells;
    std::string fileName;
    std::vector<uint8_t> data;

    void LoadMapType0();
    void LoadMapType1();
    void LoadMapType2();
    void LoadMapType3();
    void LoadMapType4();
    void LoadMapType5();
    void LoadMapType6();
    void LoadMapType7();
    void LoadMapType100();

public:
    explicit MirMap(const std::string& fileName);
    ~MirMap();

    const std::string& getFileName() const;
    int getWidth() const;
    int getHeight() const;

    std::vector<std::vector<MapCellInfo>>& getMapCells();
};
