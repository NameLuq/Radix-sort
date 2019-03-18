#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <numeric>
#include <utility>
#include <random>

template <typename T>
std::ostream &operator<<(std::ostream &o, const std::vector<T> &vec)
{
	for (auto &a : vec)
		o << a << ' ';
	return o << std::endl;
}

template <typename T>
bool operator==(const std::vector<T> &vec1, const std::vector<T> &vec2)
{
	if (vec1.size() != vec2.size())
		return false;
	for (unsigned int i = 0; i < vec1.size(); ++i)
		if (vec1[i] != vec2[i])
			return false;
	return true;
}

inline uint_fast64_t calc(uint_fast64_t num, uint_fast64_t step) { return (num >> (step << 3)) & 256; }

void CountingSort(std::vector<uint_fast64_t> &orig, std::vector<uint_fast64_t> &buf, int step)
{

	std::array<int, 256> count{};
	for (auto &a : orig)
		++count[calc(a, step)];

	for (uint_fast32_t i = 1; i < count.size(); ++i)
		count[i] += count[i - 1];

	for (int i = orig.size() - 1; i >= 0; --i)
		buf[--count[calc(orig[i], step)]] = orig[i];
}

void RadixSort(std::vector<uint_fast64_t> &vec)
{

	std::vector<uint_fast64_t> buffer(vec.size());
	std::vector<uint_fast64_t> *orig = &vec;
	std::vector<uint_fast64_t> *buf = &buffer;

	for (int step = 0; step < 8; ++step)
	{
		CountingSort(*orig, *buf, step);
		std::swap(orig, buf);
	}
}

int main()
{

	std::vector<uint_fast64_t> vec(100'000'000);

	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_int_distribution<uint_fast64_t> uniform_dist(
		0, std::numeric_limits<uint_fast64_t>::max());

	for (uint_fast64_t &i : vec)
		i = uniform_dist(e1);
	std::vector vec2(vec);

	auto start = std::chrono::system_clock::now();
	RadixSort(vec);
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Radix elapsed time: " << elapsed_seconds.count() << std::endl;

	start = std::chrono::system_clock::now();
	std::sort(vec2.begin(), vec2.end());
	end = std::chrono::system_clock::now();

	elapsed_seconds = end - start;
	std::cout << "QSort elapsed time std: " << elapsed_seconds.count() << std::endl;

	if (vec == vec2)
		std::cout << "FINE" << std::endl;
}
