#ifndef __FPZ_COMPRESS_HPP__
#define __FPZ_COMPRESS_HPP__
#include <memory>
#include <utility>
#include <zlib.h>
#include <cstring>
#include "detail.hpp"

namespace fpz {
using zdata_t = std::pair<std::unique_ptr<typename fpz::byte_t[]>, std::size_t>;
zdata_t deflate(
		const typename fpz::byte_t* const src_ptr,
		const std::size_t src_len
		) {
	// Temporary buffer
	std::unique_ptr<typename fpz::byte_t[]> tmp_data(new typename fpz::byte_t[src_len]);

	// Compress
	z_stream def_stream;
	def_stream.zalloc = Z_NULL;
	def_stream.zfree  = Z_NULL;
	def_stream.opaque = Z_NULL;
	def_stream.avail_in  = src_len;
	def_stream.next_in   = (Bytef*)src_ptr;
	def_stream.avail_out = src_len;
	def_stream.next_out  = (Bytef*)tmp_data.get();

	deflateInit(&def_stream, Z_BEST_COMPRESSION);
	deflate(&def_stream, Z_FINISH);
	deflateEnd(&def_stream);

	const std::size_t compressed_size = def_stream.total_out;

	// Return buffer
	std::unique_ptr<typename fpz::byte_t[]> compressed_data(new typename fpz::byte_t[compressed_size]);
	std::memcpy(compressed_data.get(), tmp_data.get(), compressed_size);

	return std::make_pair(std::move(compressed_data), compressed_size);
}

void inflate(
		const typename fpz::byte_t* const dst_ptr,
		const std::size_t dst_len,
		const zdata_t& src_zdata
		) {
	// Decompress
	z_stream inf_stream;
	inf_stream.zalloc = Z_NULL;
	inf_stream.zfree  = Z_NULL;
	inf_stream.opaque = Z_NULL;
	inf_stream.avail_in  = src_zdata.second;
	inf_stream.next_in   = src_zdata.first.get();
	inf_stream.avail_out = dst_len;
	inf_stream.next_out  = (Bytef*)dst_ptr;

	inflateInit(&inf_stream);
	inflate(&inf_stream, Z_NO_FLUSH);
	inflateEnd(&inf_stream);
}
} // namespace fpz
#endif
