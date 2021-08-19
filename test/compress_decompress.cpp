#include <iostream>
#include <random>
#include <vector>
#include <fpz/fpz.hpp>


constexpr std::size_t N = 1lu << 20;

template <class T>
void test() {
	std::vector<T> input_array(N);
	std::vector<T> output_array(N);
	std::vector<typename fpz::byte_t> com_array(fpz::get_com_byte<T>() * N);
	std::vector<typename fpz::byte_t> raw_array(fpz::get_raw_byte<T>() * N);

	std::mt19937 mt(0);
	std::uniform_real_distribution<T> mantissa_dist(1, 2);
	std::uniform_int_distribution<int> exponent_dist(-100, 100);
	std::uniform_int_distribution<int> sign_dist(0, 1);

	for (std::size_t i = 0; i < N; i++) {
		const auto mantissa = mantissa_dist(mt);
		const auto exponent = exponent_dist(mt);
		const auto sign = 2 * sign_dist(mt) - 1;

		const auto fp = mantissa * sign * std::pow<T>(2, exponent);
		input_array[i] = fp;
	}

	// decompose
	fpz::decompose(
		com_array.data(),
		raw_array.data(),
		input_array.data(),
		N
		);

	// deflate
	auto [compressed_data, compressed_size] = fpz::deflate(com_array.data(), fpz::get_com_byte<T>() * N);
	const auto uncompressed_size = fpz::get_com_byte<T>() * N;
	const auto raw_part_size = fpz::get_raw_byte<T>() * N;
	std::printf("Exponent compression rate : %luByte -> %luByte (%7.3f)\n", uncompressed_size, compressed_size, 100. * compressed_size / uncompressed_size);
	std::printf("Total compression rate    : %luByte -> %luByte (%7.3f)\n", uncompressed_size + raw_part_size,
				compressed_size + raw_part_size,
				100. * (compressed_size + raw_part_size) / (uncompressed_size + raw_part_size));

	// inflate
	fpz::inflate(com_array.data(), fpz::get_com_byte<T>() * N, std::make_pair(std::move(compressed_data), compressed_size));

	// compose
	fpz::compose(
		output_array.data(),
		com_array.data(),
		raw_array.data(),
		N
		);

	// validate
	std::size_t num_errors = 0;
	for (std::size_t i = 0; i < N; i++) {
		if (input_array[i] != output_array[i]) {
			num_errors++;
		}
	}
	std::printf("Passed %8lu / %8lu restorings\n", (N - num_errors), N);
}

int main() {
	test<float>();
}
