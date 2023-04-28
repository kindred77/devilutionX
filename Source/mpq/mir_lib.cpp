#include "mpq/mir_lib.hpp"
#include "utils/file_util.h"
#include "utils/gzlib_util.hpp"

namespace devilution {

struct MirLibData {
	// File information:
	std::optional<MirLibPtr> ownedMirLib;
	MirLibPtr mirLib;
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

#ifndef USE_SDL1
static Sint64 MirLibFileRwSize(struct SDL_RWops *context)
{
	return GetMirLibData(context)->size;
}
#endif

static OffsetType MirLibFileRwSeek(struct SDL_RWops *context, OffsetType offset, int whence)
{

}

static SizeType MirLibFileRwRead(struct SDL_RWops *context, void *ptr, SizeType size, SizeType maxnum)
{
    MirLibData &data = *GetMirLibData(context);
    auto *out = static_cast<uint8_t *>(ptr);
}

static int MirLibFileRwClose(struct SDL_RWops *context)
{

}

}// extern "C"

SDL_RWops *SDL_RWops_FromMirLibFile(const MirLibPtr mirLib, size_t img_idx)
{
    auto result = std::make_unique<SDL_RWops>();
	std::memset(result.get(), 0, sizeof(*result));

#ifndef USE_SDL1
	result->size = &MirLibFileRwSize;
	result->type = SDL_RWOPS_UNKNOWN;
#else
	result->type = 0;
#endif

    result->seek = &MirLibFileRwSeek;
	result->read = &MirLibFileRwRead;
	result->write = nullptr;
	result->close = &MirLibFileRwClose;

    auto data = std::make_unique<MirLibData>();
    data->mirLib = mirLib;
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
    if (!logged_fstream->Open(file_name.c_str(), std::ios::in | std::ios::binary))
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

    images = std::unique_ptr<MirImage*[]>(new MirImage*[imageCnt]);

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
        images[index] = new MirImage(index, logged_fstream.get(), file_name);
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
    hasMask = ((shadow >> 7) == static_cast<byte>(1)) ? true : false;
    if (hasMask)
    {
        byte tmpLength;
        if (!MirLib::Read(
            logged_fstream.get(),
            file_name,
            reinterpret_cast<char *>(&tmpLength),
            "image mask tmp length",
            sizeof(byte))
        )
        {
            return false;
        }

        if (!MirLib::Read(
            logged_fstream.get(),
            file_name,
            reinterpret_cast<char *>(&maskWidth),
            "image mask width",
            sizeof(int16_t))
        )
        {
            return false;
        }

        if (!MirLib::Read(
            logged_fstream.get(),
            file_name,
            reinterpret_cast<char *>(&maskHeight),
            "image mask height",
            sizeof(int16_t))
        )
        {
            return false;
        }

        if (!MirLib::Read(
            logged_fstream.get(),
            file_name,
            reinterpret_cast<char *>(&maskX),
            "image mask x",
            sizeof(int16_t))
        )
        {
            return false;
        }

        if (!MirLib::Read(
            logged_fstream.get(),
            file_name,
            reinterpret_cast<char *>(&maskY),
            "image mask y",
            sizeof(int16_t))
        )
        {
            return false;
        }

        if (!MirLib::Read(
            logged_fstream.get(),
            file_name,
            reinterpret_cast<char *>(&maskLength),
            "image mask length",
            sizeof(int32_t))
        )
        {
            return false;
        }
    }

    return true;
}

bool
MirImage::readAndDecompress(
    const unsigned char * compressed_data,
    int data_length,
    std::shared_ptr<byte[]>& target_data
    )
{
    byte * decompressed_data = nullptr;
    if (!gzip_decompress(
        compressed_data,
        data_length,
        reinterpret_cast<unsigned char *>(decompressed_data)))
    {
        LogError("Image data decompression failed {}.", index);
        return false;
    }
    LogInfo("get decompressed data length: {}", sizeof(decompressed_data));
    // byte * decompressed_data = nullptr;
    // uLong decomp_data_len = 0;
    // if (!gzip_decompress(
    //     const_cast<char *>(compressed_data),
    //     data_length,
    //     reinterpret_cast<char *>(decompressed_data),
    //     decomp_data_len))
    // {
    //     LogError("Image data decompression failed {}.", index);
    //     return false;
    // }

    target_data = std::shared_ptr<byte[]>(decompressed_data);

    return true;
}

bool
MirImage::loadSurface()
{
    auto* rwopt = SDL_RWops_FromMirLibFile(mir_lib, index);
    image_surface = std::shared_ptr<SDL_Surface>(IMG_LoadPNG_RW(rwopt));
    LogVerbose("debug: --55555---");
    //image data
    //not compressed
	image_data = std::unique_ptr<byte[]>(new byte[length]());
	LogVerbose("debug: --6666---");
	if (!MirLib::Read(
	        logged_fstream.get(),
	        file_name,
	        reinterpret_cast<char *>(image_data.get()),
	        "image data",
	        length)) {
		return false;
	}

	//mask image data
    if (hasMask)
    {
        mask_data = std::unique_ptr<byte[]>(new byte[maskLength]());

        if (!MirLib::Read(
            logged_fstream.get(),
            file_name,
            reinterpret_cast<char *>(mask_data.get()),
            "mask image data",
            maskLength)
        )
        {
            return false;
        }

        //not compressed
        // if (!readAndDecompress(
        //     reinterpret_cast<const unsigned char *>(compressed_data.get()),
        //     maskLength, mask_data))
        // {
        //     LogError("File {}. Can not decompress mask image data. Image index {}.",
        //         file_name.c_str(),
        //         index);
        //     return false;
        // }
    }

    return true;
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
    if (!loadSurface())
    {
        return false;
    }
    initialized = true;

    LogInfo("Image initialized successfully, idx: {}, width: {}, height: {}, x: {}, y: {}.", index, width, height, x, y);

    return initialized;
}

}