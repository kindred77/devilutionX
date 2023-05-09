#pragma once

#include "zlib.h"
#include "utils/log.hpp"

namespace {

[[maybe_unused]] int gzip_compress(const char *src, uInt srcLen, char *dest, uInt destLen)
{
	z_stream c_stream;
	int err = 0;
	int windowBits = 15;
	int GZIP_ENCODING = 16;

	if (src && srcLen > 0)
	{
		c_stream.zalloc = (alloc_func)0;
		c_stream.zfree = (free_func)0;
		c_stream.opaque = (voidpf)0;
		if (deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
			windowBits | GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY) != Z_OK) return -1;
		c_stream.next_in = (Bytef *)src;
		c_stream.avail_in = srcLen;
		c_stream.next_out = (Bytef *)dest;
		c_stream.avail_out = destLen;
		while (c_stream.avail_in != 0 && c_stream.total_out < destLen)
		{
			if (deflate(&c_stream, Z_NO_FLUSH) != Z_OK) return -1;
		}
		if (c_stream.avail_in != 0) return c_stream.avail_in;
		for (;;)
		{
			if ((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END) break;
			if (err != Z_OK) return -1;
		}
		if (deflateEnd(&c_stream) != Z_OK) return -1;
		return c_stream.total_out;
	}
	return -1;
}

[[maybe_unused]] bool gzip_decompress(const unsigned char *src, uInt srcLen, unsigned char * dst/*, uInt dstLen*/)
{
	const int COPY_BUFFER_SIZE = srcLen*4;
	z_stream strm;
	strm.zalloc = NULL;
	strm.zfree = NULL;
	strm.opaque = NULL;

	auto copy_buffer = std::unique_ptr<Bytef[]>(new Bytef[COPY_BUFFER_SIZE]);

	strm.avail_in = srcLen;
	strm.avail_out = COPY_BUFFER_SIZE;
	strm.next_in = (Bytef *)src;
	strm.next_out = copy_buffer.get();

	devilution::LogInfo("debug: gzip_decompress--0000---");
	int err = -1;//, ret = -1;
	err = inflateInit2(&strm, MAX_WBITS + 16);
	devilution::LogInfo("debug: gzip_decompress--1111---");
	if (err == Z_OK)
	{
		devilution::LogInfo("debug: gzip_decompress--2222---");
		err = inflate(&strm, Z_FINISH);
		devilution::LogInfo("debug: gzip_decompress--3333---");
		if (err == Z_STREAM_END)
		{
			devilution::LogInfo("debug: gzip_decompress--3333---Z_STREAM_END---srcLen: {}, total_out: {}", srcLen, strm.total_out);
			dst = new unsigned char[strm.total_out];
			memcpy(dst, copy_buffer.get(), strm.total_out);
			//ret = strm.total_out;
		}
		else
		{
			devilution::LogError("Inflate failed. UNKNOWN ERROR: {}", err);
			devilution::LogInfo("debug: gzip_decompress--4444---");
			inflateEnd(&strm);
			devilution::LogInfo("debug: gzip_decompress--5555---");
			return false;
		}
	}
	//not enough memory
	else if (err == Z_MEM_ERROR)
	{
		devilution::LogError("Can not init when decompressiong. Z_MEM_ERROR");
		return false;
	}
	//zlib library version is incompatible
	else if (err == Z_VERSION_ERROR)
	{
		devilution::LogError("Can not init when decompressiong. Z_VERSION_ERROR");
		return false;
	}
	else
	{
		devilution::LogError("Can not init when decompressiong. UNKNOWN ERROR: {}", err);
		devilution::LogInfo("debug: gzip_decompress--6666---");
		inflateEnd(&strm);
		devilution::LogInfo("debug: gzip_decompress--7777---");
		return false;
	}
	devilution::LogInfo("debug: gzip_decompress--8888---");
	inflateEnd(&strm);
	devilution::LogInfo("debug: gzip_decompress--9999---");
	return true;
}


[[maybe_unused]] bool gzip_decompress2(
	char *src, uLong srcLen, 
	char *dst, uLong& dstLen, 
	uLong copy_buffer_size = 10240,
	uLong dst_init_length = 10240)
{
	int ret = 0;
	uLong have = 0;
    int offset=0;
    z_stream d_stream;

    //(*pOutputbuf) = m_pOutputbuf;

    uLong comprLen = srcLen, max_output_buffer_size = 0;

	auto copy_buffer = std::make_shared<Bytef[]>(copy_buffer_size);
	//auto copy_buffer = std::unique_ptr<Bytef[]>(new Bytef[copy_buffer_size]());

	//init dest buffer
	dst = reinterpret_cast<char *>(realloc(0, dst_init_length * sizeof(Bytef)));
    memset(dst, 0, dst_init_length * sizeof(Bytef));
	
    d_stream.zalloc = Z_NULL;
    d_stream.zfree = Z_NULL;
    d_stream.opaque = Z_NULL;
    d_stream.next_in = Z_NULL;
    d_stream.avail_in = 0;
    ret = inflateInit2(&d_stream, 47);

    if (ret != Z_OK)
    {
		devilution::LogInfo("GZip uncompression init failed: {}", ret);
        return false;
    }

    d_stream.next_in = reinterpret_cast<Bytef *>(src);
    d_stream.avail_in = comprLen;

    do
    {
        d_stream.next_out = copy_buffer.get();
        d_stream.avail_out = copy_buffer_size;
        ret = inflate(&d_stream, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR)
        {
			devilution::LogInfo("GZip uncompression inflate failed: {}", ret);
            return false;
        }

        switch (ret)
        {
        case Z_NEED_DICT:
            {
				devilution::LogInfo("GZip uncompression inflate need dict.");
                return false;
            }break;
        case Z_DATA_ERROR:
            {
				devilution::LogInfo("GZip uncompression inflate data error.");
                (void)inflateEnd(&d_stream);
                return false;
            }break;
        case Z_MEM_ERROR:
            {
				devilution::LogInfo("GZip uncompression inflate memory error.");
                (void)inflateEnd(&d_stream);
                return false;
            }break;
        }
        have = copy_buffer_size - d_stream.avail_out;
        uLong length = offset + have;
        if(length > max_output_buffer_size)
        {
            max_output_buffer_size = length + copy_buffer_size;
            auto newoutputbuf = reinterpret_cast<char *>(realloc(dst, sizeof(Bytef) * max_output_buffer_size));
            if (newoutputbuf != dst)
            {
                dst = newoutputbuf;
                //m_pOutputbuf = (*pOutputbuf);
            }
        }
        memcpy(dst + offset, copy_buffer.get(), have);
        offset += have;
		memset(copy_buffer.get(), 0, have);
    }
	while (d_stream.avail_out == 0 /*ret != Z_STREAM_END*/);

    ret = inflateEnd(&d_stream);

	if (ret != Z_OK)
	{
		devilution::LogInfo("GZip uncompression inflateEnd failed {}", ret);
	}

    dstLen = offset;
    return true;
}

}