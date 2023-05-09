#include "mpq/mir_lib.hpp"
#include "utils/file_util.h"
#include "utils/gzlib_util.hpp"

#ifndef USE_SDL1
using OffsetType = Sint64;
using SizeType = size_t;
#else
using OffsetType = int;
using SizeType = int;
#endif

namespace devilution {

struct MirLibData {
	// File information:
	std::optional<MirLibPtr> ownedMirLib;
	//MirLibPtr mirLib;
	size_t image_idx;
};

MirLibData *GetMirLibData(struct SDL_RWops *context)
{
	return reinterpret_cast<MirLibData *>(context->hidden.unknown.data1);
}

void SetMirLibData(struct SDL_RWops *context, MirLibData *data)
{
	context->hidden.unknown.data1 = data;
}

extern "C"
{

static SizeType MirLibFileRwRead(struct SDL_RWops *context, void *ptr, SizeType size, SizeType maxnum)
{
    MirLibData &data = *GetMirLibData(context);
    auto mir_lib = data.ownedMirLib->get();
    if (!mir_lib->Initialize())
    {
        SDL_SetError("MirLib: %s, does not initialized.", mir_lib->GetFilName());
		return 0;
    }

    if (!mir_lib->getImageData(data.image_idx, ptr))
    {
        SDL_SetError("MirImage: %zu, MirLib: %s, can not get data.", data.image_idx, mir_lib->GetFilName());
		return 0;
    }
    return mir_lib->operator[](data.image_idx)->header.length;
}

static int MirLibFileRwClose(struct SDL_RWops *context)
{
    MirLibData *data = GetMirLibData(context);
	//data->mpqArchive->CloseBlockOffsetTable(data->fileNumber);
    //data->mirLib->
	delete data;
	delete context;
	return 0;
}

}// extern "C"

SDL_RWops *SDL_RWops_FromMirLibFile(MirLibPtr mirLib, size_t img_idx)
{
    auto result = std::make_unique<SDL_RWops>();
	std::memset(result.get(), 0, sizeof(*result));

#ifndef USE_SDL1
	//result->size = &MirLibFileRwSize;
    result->size = nullptr;
	result->type = SDL_RWOPS_UNKNOWN;
#else
	result->type = 0;
#endif

    //result->seek = &MirLibFileRwSeek;
    result->seek = nullptr;
	result->read = &MirLibFileRwRead;
	result->write = nullptr;
	result->close = &MirLibFileRwClose;

    auto data = std::make_unique<MirLibData>();
    data->ownedMirLib = mirLib;
    data->image_idx = img_idx;

    SetMirLibData(result.get(), data.release());
	return result.release();
};

bool MirLib::Initialize()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (initialized)
    {
        return true;
    }
    if (!FileExists(file_name.c_str()))
    {
        LogError("File not found {}.", file_name.c_str());
        return false;
    }

    logged_fstream = std::make_unique<LoggedFStream>();
    if (!logged_fstream->Open(file_name.c_str(), "r+b"))
    {
        LogError("File can not open for read {}.", file_name.c_str());
		return false;
	}

    int libVersion;
    if (!Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&libVersion),
        "version",
        sizeof(int32_t))
    )
    {
        return false;
    }

    if (libVersion != LibVersion)
    {
        //logged_fstream->Close();
        LogError("Invalid mir lib file {}. Wrong version: {}.", file_name.c_str(), libVersion);
        return false;
    }

    if (!Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&imageCnt),
        "image count",
        sizeof(int32_t))
    )
    {
        return false;
    }

    images = std::unique_ptr<MirImagePtr[]>(new MirImagePtr[imageCnt]);

    indexList = std::unique_ptr<int[]>(new int[imageCnt]());
    if (!Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(indexList.get()),
        "index list",
        sizeof(int32_t) * imageCnt)
    )
    {
        return false;
    }

    initialized=true;
    LogInfo("File initialized successfully, path: {}, libVersion: {}, imageCnt: {}", file_name.c_str(), libVersion, imageCnt);
    return initialized;
};

bool
MirLib::initImageHeader(MirImagePtr& image)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&image->header),
        "image header",
        sizeof(MirImageHeader))
    )
    {
        return false;
    }
    image->initialized = true;
    return true;
}

bool
MirLib::initializeImage(int index)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (!initialized)
    {
        return false;
    }

    if (images == nullptr || index < 0 || index >= imageCnt)
    {
        LogError("Can not initialize image, mirlib do not initialized or invalid index {}.", index);
        return false;
    }
    
    if (images[index] == nullptr || !images[index]->initialized)
    {
        LogInfo("debug: --111111111---image idx: {}---file offset: {}", index, indexList[index]);
        logged_fstream->Seekp(indexList[index], std::ios::beg);
        if (!initImageHeader(images[index]))
        {
            LogError("Invalid mir lib file {}. Image init failed: {}.", file_name.c_str(), index);
            return false;
        }
    }
    //image header has been initialized
    else
    {
        logged_fstream->Seekp(indexList[index] + sizeof(MirImageHeader), std::ios::beg);

    }

    return true;
}

bool MirLib::getImageData(int index, void* data)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (!initializeImage(index))
    {
        return false;
    }
    auto *out = static_cast<char *>(data);
    if (!MirLib::Read(
	        logged_fstream.get(),
	        file_name,
	        out,
	        "image data",
	        images[index]->header.length))
    {
		return false;
	}

    return true;
}

}