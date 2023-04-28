#pragma once

#include "utils/logged_fstream.hpp"
#include "utils/log.hpp"
#include "utils/stdcompat/cstddef.hpp"
#include "utils/png.hpp"

namespace devilution {

class MirLib;

SDL_RWops *SDL_RWops_FromMirLibFile(const MirLib &mirLib, size_t img_idx);

class MirImage;

class MirLib {
    const int LibVersion = 2; 
    bool initialized = false;
    int imageCnt = 0;
    const std::string file_name;
    std::unique_ptr<LoggedFStream> logged_fstream = nullptr;
    std::unique_ptr<MirImage*[]> images = nullptr;
    std::unique_ptr<int[]> indexList;
public:
    explicit MirLib(const char *file_name_)
    : file_name(std::move(file_name_))
    {

    }

    bool Initialize();

    inline const char* GetFilName() const
    {
        return file_name.c_str();
    }

    MirImage* operator[](const int idx)
	{
        LogVerbose("debug: 000000--image idx: {}---file offset: {}", idx, indexList[idx]);
        if (InitializeImage(idx))
        {
            return images[idx];
        }
		return nullptr;
	}

    bool InitializeImage(int index);

    static bool Read(
        LoggedFStream * logged_fstream_,
        const std::string file_name_,
        char * dstPos,
        const char* msgLabel,
        int readSize
    )
    {
        if (!logged_fstream_->Read(dstPos,
            static_cast<std::streamsize>(readSize))
        )
        {
            //logged_fstream->Close();
            LogError("Invalid mir lib file {}. Can not read the {}.",
                file_name_.c_str(),
                msgLabel);
            return false;
        }

        return true;
    }
};

using MirLibPtr = std::shared_ptr<MirLib>;

class MirImage {
    MirLibPtr mir_lib;
    bool initialized = false;

    short width, height, x, y, shadowX, shadowY;
    byte shadow;
    int length;

    //bool textureValid;
    //Texture image;
        //layer 2:
    short maskWidth, maskHeight, maskX, maskY;
    int maskLength;

    //Texture maskImage;
    bool hasMask;

    //long cleanTime;
    //Size trueSize;

    //std::shared_ptr<byte[]> image_data = nullptr;
    //std::shared_ptr<byte[]> mask_data = nullptr;

    std::shared_ptr<SDL_Surface> image_surface = nullptr;
    std::shared_ptr<SDL_Surface> mask_surface = nullptr;

    const int index = -1;
    std::unique_ptr<LoggedFStream> logged_fstream;
    const std::string file_name;

    bool initHeader();
    bool loadSurface();

    // bool readAndDecompress(const unsigned char * compressed_data,
    //     int data_length,
    //     std::shared_ptr<byte[]>& target_data);
public:
    explicit MirImage(const MirLibPtr mir_lib_, const int index_, LoggedFStream* logged_fstream_, const std::string file_name_)
    : mir_lib(mir_lib_)
    , index(index_)
    , logged_fstream(std::move(logged_fstream_))
    , file_name(file_name_)
    {
    }

    bool Initialize();

    inline short GetWidth() const
    {
        return width;
    }

    inline short GetHeight() const
    {
        return height;
    }

    inline short GetX() const
    {
        return x;
    }

    inline short GetY() const
    {
        return y;
    }

    inline int GetLength() const
    {
        return length;
    }

    inline bool IsInitialized() const
    {
        return initialized;
    }

    inline int GetImageIndex() const
    {
        return index;
    }

    inline std::shared_ptr<SDL_Surface> GetSurface() const
    {
        return image_surface;
    }

    inline std::shared_ptr<SDL_Surface> GetMaskSurface() const
    {
        return mask_surface;
    }
};

}