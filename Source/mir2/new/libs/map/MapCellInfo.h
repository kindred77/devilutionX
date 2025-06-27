// MapCellInfo.h
#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "MapObject.h"
#include "MonsterObject.h"

class MapObject;
class MonsterObject;

class MapCellInfo {
public:
    using MapObjectPtr = std::shared_ptr<MapObject>;
    using MapObjectList = std::vector<MapObjectPtr>;

    // 构造函数
    MapCellInfo();

    // 添加对象到最前面
    void addObject(const MapObjectPtr& ob);

    // 删除指定对象
    void removeObject(const MapObjectPtr& ob);

    // 查找指定 ID 的对象
    MapObjectPtr findObject(long objectID) const;

    // 绘制所有存活对象
    void drawObjects(void* surface) const;

    // 绘制死亡对象（特定类型除外）
    void drawDeadObjects(void* surface) const;

    // 排序对象
    void sort();

    // Getters & Setters
    short getBackIndex() const { return backIndex; }
    int getBackImage() const { return backImage; }

    short getMiddleIndex() const { return middleIndex; }
    int getMiddleImage() const { return middleImage; }

    short getFrontIndex() const { return frontIndex; }
    int getFrontImage() const { return frontImage; }

    byte getDoorIndex() const { return doorIndex; }
    byte getDoorOffset() const { return doorOffset; }

    byte getFrontAnimationFrame() const { return frontAnimationFrame; }
    byte getFrontAnimationTick() const { return frontAnimationTick; }

    byte getMiddleAnimationFrame() const { return middleAnimationFrame; }
    byte getMiddleAnimationTick() const { return middleAnimationTick; }

    short getTileAnimationImage() const { return tileAnimationImage; }
    short getTileAnimationOffset() const { return tileAnimationOffset; }
    byte getTileAnimationFrames() const { return tileAnimationFrames; }

    byte getLight() const { return light; }
    byte getUnknown() const { return unknown; }

    bool isFishingCell() const { return fishingCell; }

private:
    short backIndex;
    int backImage;

    short middleIndex;
    int middleImage;

    short frontIndex;
    int frontImage;

    byte doorIndex;
    byte doorOffset;

    byte frontAnimationFrame;
    byte frontAnimationTick;

    byte middleAnimationFrame;
    byte middleAnimationTick;

    short tileAnimationImage;
    short tileAnimationOffset;
    byte tileAnimationFrames;

    byte light;
    byte unknown;
    bool fishingCell;

    MapObjectList cellObjects;
};
