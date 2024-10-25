#include "mpq/mir_lib.hpp"
#include "utils/file_util.h"
#include "utils/gzlib_util.hpp"

// #ifndef USE_SDL1
// using OffsetType = Sint64;
// using SizeType = size_t;
// #else
// using OffsetType = int;
// using SizeType = int;
// #endif

namespace devilution {

// struct MirLibData {
// 	// File information:
// 	std::optional<MirLibPtr> ownedMirLib;
// 	//MirLibPtr mirLib;
// 	size_t image_idx;
// };

// MirLibData *GetMirLibData(struct SDL_RWops *context)
// {
// 	return reinterpret_cast<MirLibData *>(context->hidden.unknown.data1);
// }

// void SetMirLibData(struct SDL_RWops *context, MirLibData *data)
// {
// 	context->hidden.unknown.data1 = data;
// }

// extern "C"
// {

// static Sint64 MirLibFileRwSize(struct SDL_RWops *context)
// {
//     std::cout << "MirLibFileRwSize" << std::endl;
//     MirLibData &data = *GetMirLibData(context);
//     auto mir_lib = data.ownedMirLib->get();
//     if (!mir_lib->operator[](data.image_idx)->initialized)
//     {
//         SDL_SetError("MirImage: %zu, MirLib: %s, can not initialize the image.", data.image_idx, mir_lib->GetFilName());
// 		return -1;
//     }
// 	return mir_lib->operator[](data.image_idx)->header.length;
// }

// static OffsetType MirLibFileRwSeek(struct SDL_RWops *context, OffsetType offset, int whence)
// {
//     std::cout << "MirLibFileRwSeek" << std::endl;
// 	return -1;
// }

// static SizeType MirLibFileRwRead(struct SDL_RWops *context, void *ptr, SizeType size, SizeType maxnum)
// {
//     std::cout << "MirLibFileRwRead" << std::endl;
//     MirLibData &data = *GetMirLibData(context);
//     std::cout << "MirLibFileRwRead------00000" << std::endl;
//     auto mir_lib = data.ownedMirLib->get();
//     std::cout << "MirLibFileRwRead------11111" << std::endl;
//     // if (!mir_lib->Initialize())
//     // {
//     //     std::cout << "MirLibFileRwRead----0000000" << std::endl;
//     //     SDL_SetError("MirLib: %s, does not initialized.", mir_lib->GetFilName());
// 	// 	return 0;
//     // }

//     if (!mir_lib->getImageData(data.image_idx, ptr))
//     {
//         std::cout << "MirLibFileRwRead----22222" << std::endl;
//         SDL_SetError("MirImage: %zu, MirLib: %s, can not get data.", data.image_idx, mir_lib->GetFilName());
// 		return 0;
//     }
//     std::cout << "MirLibFileRwRead-----data.image_idx: " << data.image_idx << ", length: " << mir_lib->operator[](data.image_idx)->header.length << std::endl;
//     std::cout << "MirLibFileRwRead-----width: " << mir_lib->operator[](data.image_idx)->header.width << std::endl;
//     std::cout << "MirLibFileRwRead-----height: " << mir_lib->operator[](data.image_idx)->header.height << std::endl;
//     std::cout << "MirLibFileRwRead-----size: " << size << std::endl;
//     std::cout << "MirLibFileRwRead-----maxnum: " << maxnum << std::endl;
//     //return mir_lib->operator[](data.image_idx)->header.length;
//     return 1;
// }

// static int MirLibFileRwClose(struct SDL_RWops *context)
// {
//     std::cout << "MirLibFileRwClose" << std::endl;
//     MirLibData *data = GetMirLibData(context);
// 	//data->mpqArchive->CloseBlockOffsetTable(data->fileNumber);
//     //data->mirLib->
// 	delete data;
// 	delete context;
// 	return 0;
// }

// }// extern "C"

// SDL_RWops *SDL_RWops_FromMirLibFile(MirLibPtr mirLib, size_t img_idx)
// {
//     auto result = std::make_unique<SDL_RWops>();
// 	std::memset(result.get(), 0, sizeof(*result));

// #ifndef USE_SDL1
// 	result->size = &MirLibFileRwSize;
//     //result->size = nullptr;
// 	result->type = SDL_RWOPS_UNKNOWN;
// #else
// 	result->type = 0;
// #endif

//     result->seek = &MirLibFileRwSeek;
//     //result->seek = nullptr;
// 	result->read = &MirLibFileRwRead;
// 	result->write = nullptr;
// 	result->close = &MirLibFileRwClose;

//     auto data = std::make_unique<MirLibData>();
//     data->ownedMirLib = mirLib;
//     data->image_idx = img_idx;

//     SetMirLibData(result.get(), data.release());
// 	return result.release();
// };

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

    images = std::make_unique<MirImageLRUMap>(MIRIMAGE_LRUCACHE_SIZE);

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
    LogDebug("File initialized successfully, path: {}, libVersion: {}, imageCnt: {}", file_name.c_str(), libVersion, imageCnt);
    return initialized;
};

bool
MirLib::initImageHeader(MirImagePtr& image)
{
    if (!image)
    {
        image = std::make_shared<MirImage>();
    }
    
    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&image->header.width),
        "image width",
        sizeof(short))
    )
    {
        return false;
    }
    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&image->header.height),
        "image height",
        sizeof(short))
    )
    {
        return false;
    }
    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&image->header.x),
        "image x",
        sizeof(short))
    )
    {
        return false;
    }
    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&image->header.y),
        "image y",
        sizeof(short))
    )
    {
        return false;
    }
    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&image->header.shadowX),
        "image shadowX",
        sizeof(short))
    )
    {
        return false;
    }
    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&image->header.shadowY),
        "image shadowY",
        sizeof(short))
    )
    {
        return false;
    }
    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&image->header.shadow),
        "image shadow",
        sizeof(byte))
    )
    {
        return false;
    }
    if (!MirLib::Read(
        logged_fstream.get(),
        file_name,
        reinterpret_cast<char *>(&image->header.length),
        "image length",
        sizeof(int))
    )
    {
        return false;
    }
    LogDebug("x: {}, y: {}, width: {}, height: {}, length: {}", image->header.x, image->header.y, image->header.width, image->header.height, image->header.length);
    return true;
}

bool
MirLib::initializeImage(int index)
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
    
    if (!images->contains(index) || !images->get(index).get()->initialized)
    {
        if (!logged_fstream->Seekp(indexList[index]))
        {
            LogError("Can not seek to {}, the mirlib file {} may be invalid.", indexList[index], file_name.c_str());
            return false;
        }
        if (!images->get(index))
        {
            images->insert(index, std::make_shared<MirImage>());
        }
        auto& img = images->get(index).get();
        if (!initImageHeader(img))
        {
            LogError("Image header init failed, mir lib file {}. image index: {}.", file_name.c_str(), index);
            return false;
        }
        img->data = new char[img->header.length];
		if (!MirLib::Read(
		        logged_fstream.get(),
		        file_name,
		        img->data,
		        "image data",
		        img->header.length)) {
			LogError("Can not read image data, mir lib file {}. image index: {}.", file_name.c_str(), index);
			return false;
		}
        img->initialized = true;
    }
    //image header has been initialized
    // else
    // {
    //     logged_fstream->Seekp(indexList[index] + MIRIMGHEADER_SIZE, std::ios::beg);

    // }

    return true;
}

bool MirLib::getImageData(int index, char* data)
{
    if (!initializeImage(index))
    {
        return false;
    }
    if (!MirLib::Read(
	        logged_fstream.get(),
	        file_name,
	        data,
	        "image data",
	        images->get(index).get()->header.length))
    {
		return false;
	}
    return true;
}

bool MirLib::CheckImage(int index)
{
    if (!initialized)
    {
        return false;
    }

    if (!images || index < 0 || index >= imageCnt)
        return false;

    auto img = this->operator[](index);
    if (!img || img->header.width == 0 || img->header.height == 0)
    {
        return false;
    }

    return true;
}

// bool MirLib::VisiblePixel(int index, PointPtr point, bool accuate)
// {
//     if (!CheckImage(index)) return false;

//     auto img = this->operator[](index);
//     if (accuate)
//     { 
//         return img->VisiblePixel(point);
//     }

//     int accuracy = 2;

//     for (int x = -accuracy; x <= accuracy; x++)
//         for (int y = -accuracy; y <= accuracy; y++)
//             if (img->VisiblePixel(std::make_shared<Point>(point->X() + x, point->Y() + y)))
//                 return true;

//     return false;
// }

}
