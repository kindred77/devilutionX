bool MirLib::Initialize(bool isSilence) {
    std::lock_guard<std::mutex> guard(lock_);
    if (initialized_) return true;

    std::string fullPath = fileName_ + ".lib";  // 根据你的 Settings.LIB_SUFFIX 替换
    if (!std::filesystem::exists(fullPath)) {
        if (!isSilence) {
            std::cerr << "Error: File not exists: " << fullPath << std::endl;
        }
        return false;
    }

    try {
        myRAF_ = std::make_unique<MyRandomAccessFile>(fullPath, "rb");

        int libVersion = static_cast<int>(myRAF_->readIntLE());
        if (libVersion != LibVersion) {
            throw std::runtime_error("Invalid mir lib file version.");
        }

        imageCnt_ = static_cast<int>(myRAF_->readIntLE());
        images_.resize(imageCnt_);
        indexList_.resize(imageCnt_);

        for (int i = 0; i < imageCnt_; ++i) {
            indexList_[i] = static_cast<int>(myRAF_->readIntLE());
        }

        initialized_ = true;
    } catch (const std::exception& e) {
        if (!isSilence) {
            std::cerr << "Error: Invalid mir lib file: " << fullPath << std::endl;
            std::cerr << e.what() << std::endl;
        }
        return false;
    }

    return true;
}

MirImage* MirLib::GetMirImage(int index) {
    std::lock_guard<std::mutex> guard(lock_);
    if (!initialized_) return nullptr;

    if (index < 0 || index >= imageCnt_) {
        std::cerr << "Error: Can not initialize image, invalid index: " << index << ", total images: " << imageCnt_ << std::endl;
        return nullptr;
    }

    if (!images_[index]) {
        try {
            myRAF_->seek(indexList_[index]);
            images_[index] = std::make_unique<MirImage>(*myRAF_);
        } catch (const std::exception& e) {
            std::cerr << "Error: Can not initialize image, lib name: " << fileName_ << ", index " << index << std::endl;
            std::cerr << e.what() << std::endl;
            return nullptr;
        }
    }

    return images_[index].get();
}

std::vector<MirImage*> MirLib::GetMirImages(const std::vector<int>& indexes) {
    std::vector<MirImage*> result;
    result.reserve(indexes.size());

    for (int idx : indexes) {
        result.push_back(GetMirImage(idx));
    }

    return result;
}

int MirLib::GetImageCount() {
    if (!Initialize(false)) {
        throw std::runtime_error("Lib not initialized.");
    }
    return imageCnt_;
}

std::string MirLib::GetFileName() const {
    return fileName_;
}

bool MirLib::IsInitialized() const {
    return initialized_;
}
