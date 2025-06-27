MirLibFactory::MirLibFactory() {
    initLibs();
    loadMapLibs();
}

void MirLibFactory::initLibs() {
    const std::string dataPath = Settings::DataPath;

    // 示例：ChrSel 库初始化
    chrSel_ = std::make_unique<MirLib>(dataPath + "ChrSel");
    prgUse_ = std::make_unique<MirLib>(dataPath + "Prguse");
    // ...其他库初始化
}

MirImage* MirLibFactory::GetMirMapImage(int mapLibIdx, int imgIdx) {
    bool ifUsingWemadeMir2TilesFileSplit = false;
    int newMapLibIdx = mapLibIdx;
    int newImgIdx = imgIdx;

    if (mapLibIdx == 0 && Settings::IS_WEMADEMIR2_TILES_FILE_SPLIT) {
        newMapLibIdx = imgIdx / Settings::WEMADEMIR2_TILES_FILE_SPLIT_CNT;
        newImgIdx = imgIdx % Settings::WEMADEMIR2_TILES_FILE_SPLIT_CNT;
        ifUsingWemadeMir2TilesFileSplit = true;
    }

    if (ifUsingWemadeMir2TilesFileSplit) {
        return wemadeMir2TilesExt_[newMapLibIdx]->GetMirImage(newImgIdx);
    } else {
        return mapLibs_[newMapLibIdx]->GetMirImage(newImgIdx);
    }
}