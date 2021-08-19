#ifndef __FPZ_FPZ_F64_HPP__
#define __FPZ_FPZ_F64_HPP__
#include "detail.hpp"

namespace fpz {

template <>
constexpr std::size_t get_stream_block_size<double>(){return 2;}
template <>
constexpr std::size_t get_com_byte<double>(){return 3;};
template <>
constexpr std::size_t get_raw_byte<double>(){return 13;};

template <>
void decompose<double>(
		byte_t* const dst_com_ptr,
		byte_t* const dst_raw_ptr,
		const double* const src_ptr,
		const std::size_t num_elements
		) {
	union reinterpretor {
		std::uint64_t bs;
		double fp;
	};
	for (std::size_t i = 0; i < get_num_stream_blocks<double>(num_elements); i++) {
		const auto block_filled = (num_elements - get_stream_block_size<double>() * i) > 1;
		const auto bs0 = reinterpretor{.fp = src_ptr[get_stream_block_size<double>() * i + 0]}.bs;
		const auto bs1 = block_filled ? reinterpretor{.fp = src_ptr[get_stream_block_size<double>() * i + 1]}.bs : 0lu;

		// Store exponent and sign
		const uint64_t exponent_sign0 = (bs0 >> 52);
		const uint64_t exponent_sign1 = (bs1 >> 52);
		dst_com_ptr[i * get_com_byte<double>() + 0] = exponent_sign0 >> 4;
		dst_com_ptr[i * get_com_byte<double>() + 1] = ((exponent_sign0 & 0xf) << 4) | (exponent_sign1 >> 8);
		dst_com_ptr[i * get_com_byte<double>() + 2] = exponent_sign1 & 0xff;

		// Store mantissa and sign
		dst_raw_ptr[i * get_raw_byte<double>() +  0] =  (bs0 >> 44) & 0xff;
		dst_raw_ptr[i * get_raw_byte<double>() +  1] =  (bs0 >> 36) & 0xff;
		dst_raw_ptr[i * get_raw_byte<double>() +  2] =  (bs0 >> 28) & 0xff;
		dst_raw_ptr[i * get_raw_byte<double>() +  3] =  (bs0 >> 20) & 0xff;
		dst_raw_ptr[i * get_raw_byte<double>() +  4] =  (bs0 >> 12) & 0xff;
		dst_raw_ptr[i * get_raw_byte<double>() +  5] =  (bs0 >>  4) & 0xff;
		dst_raw_ptr[i * get_raw_byte<double>() +  6] = ((bs0 & 0xf) << 4) | ((bs1 >> 48) & 0xf);
		dst_raw_ptr[i * get_raw_byte<double>() +  7] =  (bs1 >> 40) & 0xff;
		dst_raw_ptr[i * get_raw_byte<double>() +  8] =  (bs1 >> 32) & 0xff;
		dst_raw_ptr[i * get_raw_byte<double>() +  9] =  (bs1 >> 24) & 0xff;
		dst_raw_ptr[i * get_raw_byte<double>() + 10] =  (bs1 >> 16) & 0xff;
		dst_raw_ptr[i * get_raw_byte<double>() + 11] =  (bs1 >>  8) & 0xff;
		dst_raw_ptr[i * get_raw_byte<double>() + 12] =  (bs1      ) & 0xff;
	}
}

template <>
void compose<double>(
		double* const dst_ptr,
		const byte_t* const src_com_ptr,
		const byte_t* const src_raw_ptr,
		const std::size_t num_elements
		) {
	union reinterpretor {
		std::uint64_t bs;
		double fp;
	};
	for (std::size_t i = 0; i < get_num_stream_blocks<double>(num_elements); i++) {
		// Store exponent and sign
		uint64_t exponent_sign0 = 0, exponent_sign1 = 0;
		exponent_sign0 |=  static_cast<uint64_t>(src_com_ptr[i * get_com_byte<double>() + 0]) << 4;
		exponent_sign0 |=  static_cast<uint64_t>(src_com_ptr[i * get_com_byte<double>() + 1]) >> 4;
		exponent_sign1 |= (static_cast<uint64_t>(src_com_ptr[i * get_com_byte<double>() + 1]) & 0xf) << 8;
		exponent_sign1 |=  static_cast<uint64_t>(src_com_ptr[i * get_com_byte<double>() + 2]);

		// Store mantissa and sign
		uint64_t mantissa0 = 0, mantissa1 = 0;
		mantissa0 |=  (static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() +  0]) << 44);
		mantissa0 |=  (static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() +  1]) << 36);
		mantissa0 |=  (static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() +  2]) << 28);
		mantissa0 |=  (static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() +  3]) << 20);
		mantissa0 |=  (static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() +  4]) << 12);
		mantissa0 |=  (static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() +  5]) << 4 );
		mantissa0 |=  (static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() +  6]) >> 4 );
		mantissa1 |= ((static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() +  6]) & 0xf) << 48);
		mantissa1 |=  (static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() +  7]) << 40);
		mantissa1 |=  (static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() +  8]) << 32);
		mantissa1 |=  (static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() +  9]) << 24);
		mantissa1 |=  (static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() + 10]) << 16);
		mantissa1 |=  (static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() + 11]) <<  8);
		mantissa1 |=  (static_cast<uint64_t>(src_raw_ptr[i * get_raw_byte<double>() + 12]));

		dst_ptr[get_stream_block_size<double>() * i + 0] = reinterpretor{.bs = ((exponent_sign0 << 52) | mantissa0)}.fp;
		const auto block_filled = (num_elements - get_stream_block_size<double>() * i) > 1;
		if (block_filled) {
			dst_ptr[get_stream_block_size<double>() * i + 1] = reinterpretor{.bs = ((exponent_sign1 << 52) | mantissa1)}.fp;
		}
	}
}

} // namespace fpz
#endif
