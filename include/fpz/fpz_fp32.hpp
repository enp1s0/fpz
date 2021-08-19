#ifndef __FPZ_FPZ_F32_HPP__
#define __FPZ_FPZ_F32_HPP__
#include "detail.hpp"

namespace fpz {

template <>
constexpr unsigned get_stream_block_size<float>(){return 1;}
template <>
constexpr unsigned get_com_byte<float>(){return 1;};
template <>
constexpr unsigned get_raw_byte<float>(){return 3;};

template <>
void decompose<float>(
		byte_t* const dst_com_ptr,
		byte_t* const dst_raw_ptr,
		const float* const src_ptr,
		const unsigned num_stream_block
		) {
	union reinterpretor {
		std::uint32_t bs;
		float fp;
	};
	for (unsigned i = 0; i < num_stream_block * get_stream_block_size<float>(); i++) {
		const auto bs = reinterpretor{.fp = src_ptr[i]}.bs;

		// Store exponent
		const auto exponent = (bs >> 23) & 0xff;
		dst_com_ptr[i] = exponent;

		// Store mantissa and sign
		const auto sign_mantissa = (bs & 0x7fffff) | ((bs & 0x80000000u) >> 8);
		dst_raw_ptr[get_raw_byte<float>() * i + 0] = (sign_mantissa >>  0) & 0xff;
		dst_raw_ptr[get_raw_byte<float>() * i + 1] = (sign_mantissa >>  8) & 0xff;
		dst_raw_ptr[get_raw_byte<float>() * i + 2] = (sign_mantissa >> 16) & 0xff;
	}
}

template <>
void compose<float>(
		float* const dst_ptr,
		const byte_t* const src_com_ptr,
		const byte_t* const src_raw_ptr,
		const unsigned num_stream_block
		) {
	union reinterpretor {
		std::uint32_t bs;
		float fp;
	};
	for (unsigned i = 0; i < num_stream_block * get_stream_block_size<float>(); i++) {
		// Load mantissa and sign
		const std::uint32_t b0 = src_raw_ptr[get_raw_byte<float>() * i + 0];
		const std::uint32_t b1 = src_raw_ptr[get_raw_byte<float>() * i + 1];
		const std::uint32_t b2 = src_raw_ptr[get_raw_byte<float>() * i + 2];
		const auto sign_mantissa = b0 | (b1 << 8) | (b2 << 16);
		const auto sign = (sign_mantissa << 8) & 0x80000000u;
		const auto mantissa = sign_mantissa & 0x7fffff;

		// Load exponent
		const auto exponent = static_cast<std::uint32_t>(src_com_ptr[i]) << 23;

		const auto bs = sign | exponent | mantissa;

		dst_ptr[i] = reinterpretor{.bs = bs}.fp;
	}
}

} // namespace fpz
#endif
