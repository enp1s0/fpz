#ifndef __FPZ_DETAIL_HPP__
#define __FPZ_DETAIL_HPP__
#include <cstdint>
namespace fpz {
using byte_t = std::uint8_t;

template <class T>
constexpr std::size_t get_stream_block_size();

template <class T>
constexpr std::size_t get_com_byte();
template <class T>
constexpr std::size_t get_raw_byte();

template <class T>
std::size_t get_num_stream_blocks(const std::size_t num_elements) {
	return (num_elements + get_stream_block_size<T>() - 1) / get_stream_block_size<T>();
}

template <class T>
std::size_t get_stream_com_blocks_bytes(const std::size_t num_elements) {
	return get_num_stream_blocks<T>(num_elements) * get_com_byte<T>();
}

template <class T>
std::size_t get_stream_raw_blocks_bytes(const std::size_t num_elements) {
	return get_num_stream_blocks<T>(num_elements) * get_raw_byte<T>();
}

template <class T>
void decompose(
		byte_t* const dst_com_ptr,
		byte_t* const dst_raw_ptr,
		const T* const src_ptr,
		const std::size_t num_elements
		);

template <class T>
void compose(
		T* const src_ptr,
		const byte_t* const dst_com_ptr,
		const byte_t* const dst_raw_ptr,
		const std::size_t num_elements
		);
} // namespace fpz
#endif
