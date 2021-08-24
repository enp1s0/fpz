#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <fpz/fpz.hpp>

constexpr std::size_t N = 1lu << 20;

template <class T>
const char* get_type_name_str();
template <> const char* get_type_name_str<float >() {return "float" ;}
template <> const char* get_type_name_str<double>() {return "double";}

template <class T>
void test() {
	std::printf("### TEST for %s ###\n", get_type_name_str<T>());
	std::vector<T> input_array(N);
	std::vector<T> output_array(N);
	std::vector<typename fpz::byte_t> com_array(fpz::get_stream_com_blocks_bytes<T>(N));
	std::vector<typename fpz::byte_t> raw_array(fpz::get_stream_raw_blocks_bytes<T>(N));

	std::mt19937 mt(0);
	std::uniform_real_distribution<T> mantissa_dist(1, 2);
	std::uniform_int_distribution<int> exponent_dist(-100, 100);
	std::uniform_int_distribution<int> sign_dist(0, 1);

	const auto clock_0 = std::chrono::system_clock::now();
	for (std::size_t i = 0; i < N; i++) {
		const auto mantissa = mantissa_dist(mt);
		const auto exponent = exponent_dist(mt);
		const auto sign = 2 * sign_dist(mt) - 1;

		const auto fp = mantissa * sign * std::pow<T>(2, exponent);
		input_array[i] = fp;
	}

	const auto clock_1 = std::chrono::system_clock::now();
	// decompose
	fpz::decompose(
		com_array.data(),
		raw_array.data(),
		input_array.data(),
		N
		);

	const auto clock_2 = std::chrono::system_clock::now();
	// deflate
	auto [compressed_data, compressed_size] = fpz::deflate(com_array.data(), fpz::get_stream_com_blocks_bytes<T>(N));
	const auto uncompressed_size = fpz::get_stream_com_blocks_bytes<T>(N);
	const auto raw_part_size = fpz::get_stream_raw_blocks_bytes<T>(N);
	std::printf("Exponent compression rate : %luByte -> %luByte (%7.3f)\n", uncompressed_size, compressed_size, 100. * compressed_size / uncompressed_size);
	std::printf("Total compression rate    : %luByte -> %luByte (%7.3f)\n", uncompressed_size + raw_part_size,
				compressed_size + raw_part_size,
				100. * (compressed_size + raw_part_size) / (sizeof(T) * N));

	const auto clock_3 = std::chrono::system_clock::now();
	// inflate
	fpz::inflate(com_array.data(), fpz::get_stream_com_blocks_bytes<T>(N), std::make_pair(std::move(compressed_data), compressed_size));

	const auto clock_4 = std::chrono::system_clock::now();
	// compose
	fpz::compose(
		output_array.data(),
		com_array.data(),
		raw_array.data(),
		N
		);

	const auto clock_5 = std::chrono::system_clock::now();

	// validate
	std::size_t num_errors = 0;
	for (std::size_t i = 0; i < N; i++) {
		if (input_array[i] != output_array[i]) {
			num_errors++;
		}
	}
	std::printf("Restoring test: Passed %8lu / %8lu elements\n", (N - num_errors), N);
	std::printf("# Time break down\n");
	std::printf("%15s: %e [s]\n", "init array"    , std::chrono::duration_cast<std::chrono::microseconds>(clock_1 - clock_0).count() * 1e-6);
	std::printf("%15s: %e [s]\n", "fpz::compose"  , std::chrono::duration_cast<std::chrono::microseconds>(clock_2 - clock_1).count() * 1e-6);
	std::printf("%15s: %e [s]\n", "fpz::deflate"  , std::chrono::duration_cast<std::chrono::microseconds>(clock_3 - clock_2).count() * 1e-6);
	std::printf("%15s: %e [s]\n", "fpz::inflate"  , std::chrono::duration_cast<std::chrono::microseconds>(clock_4 - clock_3).count() * 1e-6);
	std::printf("%15s: %e [s]\n", "fpz::decompose", std::chrono::duration_cast<std::chrono::microseconds>(clock_5 - clock_4).count() * 1e-6);
}

int main() {
	test<float>();
	test<double>();
}
