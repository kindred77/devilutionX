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

// #ifndef USE_SDL1
// static Sint64 MirLibFileRwSize(struct SDL_RWops *context)
// {
// 	return GetMirLibData(context)->size;
// }
// #endif

// static OffsetType MirLibFileRwSeek(struct SDL_RWops *context, OffsetType offset, int whence)
// {

// }

static SizeType MirLibFileRwRead(struct SDL_RWops *context, void *ptr, SizeType size, SizeType maxnum)
{
    MirLibData &data = *GetMirLibData(context);
    auto mir_lib = data.ownedMirLib->get();
    if (!mir_lib->Initialize())
    {
        SDL_SetError("MirLib: %s, does not initialized.", mir_lib->GetFilName());
		return 0;
    }
    if (!mir_lib->operator[](data.image_idx)->Initialize())
    {
        SDL_SetError("MirImage: %d, MirLib: %s, does not initialized.", data.image_idx, mir_lib->GetFilName());
		return 0;
    }
    ;
    if (!mir_lib->operator[](data.image_idx)->getData(ptr))
    {
        SDL_SetError("MirImage: %d, MirLib: %s, can not get data.", data.image_idx, mir_lib->GetFilName());
		return 0;
    }
    return mir_lib->operator[](data.image_idx)->GetLength();
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
MirLib::InitializeImage(int index)
{
    if (!initialized)
    {
        return false;
    }

    if (images == nullptr || index < 0 || index >= imageCnt)
    {
        LogError("Can not initialize image, mirlib do not initialized or invalid index {}.", index);
        return false;
    }
    
    if (images[index] == nullptr || !images[index]->IsInitialized())
    {
        LogInfo("debug: --111111111---image idx: {}---file offset: {}", index, indexList[index]);
        logged_fstream->Seekp(indexList[index], std::ios::beg);
        images[index] = std::make_shared<MirImage>(std::shared_ptr<MirLib>(this), index, logged_fstream.get(), file_name);
        if (!images[index]->Initialize())
        {
            LogError("Invalid mir lib file {}. Image init failed: {}.", file_name.c_str(), index);
            return false;
        }
    }

    return true;
}

bool
MirImage::initHeader()
{
    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&width),
        "image width",
        sizeof(short))
    )
    {
        return false;
    }

    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&height),
        "image height",
        sizeof(short))
    )
    {
        return false;
    }

    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&x),
        "image x",
        sizeof(short))
    )
    {
        return false;
    }

    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&y),
        "image y",
        sizeof(short))
    )
    {
        return false;
    }

    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&shadowX),
        "image shadowX",
        sizeof(short))
    )
    {
        return false;
    }

    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&shadowY),
        "image shadowY",
        sizeof(short))
    )
    {
        return false;
    }

    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&shadow),
        "image shadow",
        sizeof(byte))
    )
    {
        return false;
    }

    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&length),
        "image length",
        sizeof(int32_t))
    )
    {
        return false;
    }
    //check if there's a second layer and read it
    // hasMask = ((shadow >> 7) == static_cast<byte>(1)) ? true : false;
    // if (hasMask)
    // {
    //     byte tmpLength;
    //     if (!MirLib::Read(
    //         logged_fstream.get(),
    //         file_name,
    //         reinterpret_cast<char *>(&tmpLength),
    //         "image mask tmp length",
    //         sizeof(byte))
    //     )
    //     {
    //         return false;
    //     }

    //     if (!MirLib::Read(
    //         logged_fstream.get(),
    //         file_name,
    //         reinterpret_cast<char *>(&maskWidth),
    //         "image mask width",
    //         sizeof(int16_t))
    //     )
    //     {
    //         return false;
    //     }

    //     if (!MirLib::Read(
    //         logged_fstream.get(),
    //         file_name,
    //         reinterpret_cast<char *>(&maskHeight),
    //         "image mask height",
    //         sizeof(int16_t))
    //     )
    //     {
    //         return false;
    //     }

    //     if (!MirLib::Read(
    //         logged_fstream.get(),
    //         file_name,
    //         reinterpret_cast<char *>(&maskX),
    //         "image mask x",
    //         sizeof(int16_t))
    //     )
    //     {
    //         return false;
    //     }

    //     if (!MirLib::Read(
    //         logged_fstream.get(),
    //         file_name,
    //         reinterpret_cast<char *>(&maskY),
    //         "image mask y",
    //         sizeof(int16_t))
    //     )
    //     {
    //         return false;
    //     }

    //     if (!MirLib::Read(
    //         logged_fstream.get(),
    //         file_name,
    //         reinterpret_cast<char *>(&maskLength),
    //         "image mask length",
    //         sizeof(int32_t))
    //     )
    //     {
    //         return false;
    //     }
    // }

    return true;
}

// bool
// MirImage::readAndDecompress(
//     const unsigned char * compressed_data,
//     int data_length,
//     std::shared_ptr<byte[]>& target_data
//     )
// {
//     byte * decompressed_data = nullptr;
//     if (!gzip_decompress(
//         compressed_data,
//         data_length,
//         reinterpret_cast<unsigned char *>(decompressed_data)))
//     {
//         LogError("Image data decompression failed {}.", index);
//         return false;
//     }
//     LogInfo("get decompressed data length: {}", sizeof(decompressed_data));
//     // byte * decompressed_data = nullptr;
//     // uLong decomp_data_len = 0;
//     // if (!gzip_decompress(
//     //     const_cast<char *>(compressed_data),
//     //     data_length,
//     //     reinterpret_cast<char *>(decompressed_data),
//     //     decomp_data_len))
//     // {
//     //     LogError("Image data decompression failed {}.", index);
//     //     return false;
//     // }

//     target_data = std::shared_ptr<byte[]>(decompressed_data);

//     return true;
// }

bool MirImage::getData(void* data)
{
    auto *out = static_cast<char *>(data);
    if (!MirLib::Read(
	        logged_fstream.get(),
	        mir_lib->GetFilName(),
	        out,
	        "image data",
	        length))
    {
		return false;
	}
}

bool
MirImage::Initialize()
{
    LogVerbose("debug: --222222---");
    if (initialized)
    {
        return true;
    }
    LogVerbose("debug: --33333---");
    if (!initHeader())
    {
        return false;
    }
    LogVerbose("debug: --44444---");
    // if (!loadSurface())
    // {
    //     return false;
    // }
    initialized = true;

    LogInfo("Image initialized successfully, idx: {}, width: {}, height: {}, x: {}, y: {}.", index, width, height, x, y);

    return initialized;
}

}