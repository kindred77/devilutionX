#pragma once

#include <mutex>

#include "utils/logged_fstream.hpp"
#include "utils/log.hpp"
#include "utils/stdcompat/cstddef.hpp"

#include <boost/compute/detail/lru_cache.hpp>

namespace devilution {

#define MIRIMGHEADER_SIZE 17

struct MirImageHeader
{
    short width, height, x, y, shadowX, shadowY;
    byte shadow;
    int length;
};

struct MirImage
{
    MirImageHeader header;
	bool initialized = false;
	//const int index = -1;
    char* data;
    virtual ~MirImage()
    {
        initialized = false;
        delete [] data;
        data = NULL;
    }
};

using MirImagePtr = std::shared_ptr<MirImage>;
using MirImageLRUMap = boost::compute::detail::lru_cache<int, MirImagePtr>;
const int MIRIMAGE_LRUCACHE_SIZE = 500;
class MirLib {
    mutable std::mutex mutex;
    const int LibVersion = 2; 
    bool initialized = false;
    int imageCnt = 0;
    const std::string file_name;
    std::unique_ptr<LoggedFStream> logged_fstream = nullptr;
    std::unique_ptr<MirImageLRUMap> images = nullptr;
    std::unique_ptr<int[]> indexList;

    bool initImageHeader(MirImagePtr& image);

    bool initializeImage(int index);

    bool getImageData(int index, char* data);
public:
    explicit MirLib(const char *file_name_)
    : file_name(std::move(file_name_))
    {

    }

    virtual ~MirLib()
    {
        if (logged_fstream) logged_fstream->Close();
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
        std::lock_guard<std::mutex> lock(mutex);
        if (initializeImage(idx))
        {
            return images->get(idx).get();
        }
		return nullptr;
	}

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

// SDL_RWops *SDL_RWops_FromMirLibFile(MirLibPtr mirLib, size_t img_idx);

}