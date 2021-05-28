#ifndef __FPZ_DETAIL_HPP__
#define __FPZ_DETAIL_HPP__
#include <cstdint>
namespace fpz {
using byte_t = std::uint8_t;

template <class T>
constexpr unsigned get_stream_block_size();

template <class T>
constexpr unsigned get_com_byte();
template <class T>
constexpr unsigned get_raw_byte();

template <class T>
void decompose(
		byte_t* const dst_com_ptr,
		byte_t* const dst_raw_ptr,
		const T* const src_ptr,
		const unsigned num_stream_block
		);

template <class T>
void compose(
		T* const src_ptr,
		const byte_t* const dst_com_ptr,
		const byte_t* const dst_raw_ptr,
		const unsigned num_stream_block
		);
} // namespace fpz
#endif
