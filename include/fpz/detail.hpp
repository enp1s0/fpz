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
unsigned get_num_stream_blocks(const unsigned num_elements) {
	return (num_elements + get_stream_block_size<T>() - 1) / get_stream_block_size<T>();
}

template <class T>
void decompose(
		byte_t* const dst_com_ptr,
		byte_t* const dst_raw_ptr,
		const T* const src_ptr,
		const unsigned num_elements
		);

template <class T>
void compose(
		T* const src_ptr,
		const byte_t* const dst_com_ptr,
		const byte_t* const dst_raw_ptr,
		const unsigned num_elements
		);
} // namespace fpz
#endif
