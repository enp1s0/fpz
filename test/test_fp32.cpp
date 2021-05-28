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
	std::printf("Passed %8lu / %8lu\n", (N - num_errors), N);
}

int main() {
	test<float>();
}