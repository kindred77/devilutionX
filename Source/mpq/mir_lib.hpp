#pragma once


#include <mutex>

#include "utils/logged_fstream.hpp"
#include "utils/log.hpp"
#include "utils/stdcompat/cstddef.hpp"

namespace devilution {

struct MirImage
{
	bool initialized = false;

	short width, height, x, y, shadowX, shadowY;
	byte shadow;
	int length;

	const int index = -1;
};

using MirImagePtr = std::shared_ptr<MirImage>;

class MirLib {
    mutable std::mutex mutex;
    const int LibVersion = 2; 
    bool initialized = false;
    int imageCnt = 0;
    const std::string file_name;
    std::unique_ptr<LoggedFStream> logged_fstream = nullptr;
    std::unique_ptr<MirImagePtr[]> images = nullptr;
    std::unique_ptr<int[]> indexList;

    bool initImageHeader(MirImagePtr& image);

    bool initializeImage(int index);
public:
    explicit MirLib(const char *file_name_)
    : file_name(std::move(file_name_))
    {

    }

    virtual ~MirLib()
    {
        initialized = false;
        imageCnt = 0;
    }

    bool Initialize();

    inline const char* GetFilName() const
    {
        return file_name.c_str();
    }

    MirImagePtr operator[](const int idx)
	{
        LogVerbose("debug: 000000--image idx: {}---file offset: {}", idx, indexList[idx]);
        if (initializeImage(idx))
        {
            return images[idx];
        }
		return nullptr;
	}

    bool getImageData(int index, void* data);

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

SDL_RWops *SDL_RWops_FromMirLibFile(MirLibPtr mirLib, size_t img_idx);

// class MirImage {
//     MirLibPtr mir_lib;
//     bool initialized = false;

//     short width, height, x, y, shadowX, shadowY;
//     byte shadow;
//     int length;

//     const int index = -1;
//     std::unique_ptr<LoggedFStream> logged_fstream;
//     const std::string file_name;

//     bool initHeader();
// public:
//     explicit MirImage(const MirLibPtr mir_lib_, const int index_, LoggedFStream* logged_fstream_, const std::string file_name_)
//     : mir_lib(mir_lib_)
//     , index(index_)
//     , logged_fstream(std::move(logged_fstream_))
//     , file_name(file_name_)
//     {
//     }

//     virtual ~MirImage()
//     {
//         initialized = false;
//     }

//     bool getData(void* data);

//     bool Initialize();

//     inline short GetWidth() const
//     {
//         return width;
//     }

//     inline short GetHeight() const
//     {
//         return height;
//     }

//     inline short GetX() const
//     {
//         return x;
//     }

//     inline short GetY() const
//     {
//         return y;
//     }

//     inline int GetLength() const
//     {
//         return length;
//     }

//     inline bool IsInitialized() const
//     {
//         return initialized;
//     }

//     inline int GetImageIndex() const
//     {
//         return index;
//     }
// };

}