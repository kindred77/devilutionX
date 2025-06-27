#pragma once

#include <string>
#include <vector>
#include <memory>
#include <mutex>

class MirImage;

class MirLib {
public:
    static const int LibVersion = 2;

    // 构造函数
    explicit MirLib(const std::string& fileName);

    // 初始化镜像资源库
    bool Initialize(bool isSilence = false);

    // 获取某个图像实例
    MirImage* GetMirImage(int index);

    // 获取多个图像实例
    std::vector<MirImage*> GetMirImages(const std::vector<int>& indexes);

    // 获取图像总数
    int GetImageCount();

    // 文件名相关
    std::string GetFileName() const;

    // 判断是否已初始化
    bool IsInitialized() const;

private:
    struct FileDeleter {
        void operator()(void* ptr) const;
    };

    mutable std::mutex lock_;
    std::string fileName_;
    bool initialized_;
    int imageCnt_;
    std::vector<std::unique_ptr<MirImage>> images_;
    std::vector<int> indexList_;

    // 封装 MyRandomAccessFile 的 C++ 实现
    class MyRandomAccessFile;
    std::unique_ptr<MyRandomAccessFile, FileDeleter> myRAF_;
};
