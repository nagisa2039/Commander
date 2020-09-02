#pragma once
#include <random>
#include <time.h>

#define Int(X) static_cast<int>(X)
#define Size_t(X) static_cast<size_t>(X)

namespace
{
	std::random_device seed;
	std::mt19937 mt(seed());
	std::uniform_int_distribution<int> range(0, 99);

	bool Hit(const int value)
	{
		return value > range(mt);
	}
}