// MirScene.h
#pragma once

#include <vector>
#include "MapCellInfo.h"
#include "MirMap.h"

class MirScene {
public:
    virtual ~MirScene() = default;

    // 初始化场景
    virtual bool InitializeScene(const std::string& mapPath) = 0;

    // 更新逻辑（如动画、移动等）
    virtual void Update(float deltaTime) = 0;

    // 绘制场景
    virtual void Render() const = 0;

    // 处理输入事件
    virtual void HandleInput() = 0;

protected:
    MirMap* currentMap; // 当前加载的地图
    std::vector<MapCellInfo*> activeCells; // 活跃的单元格
};
