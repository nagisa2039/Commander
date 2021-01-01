#pragma once
#include <random>
#include <time.h>

namespace
{
	// ランダム用シード
	std::random_device seed;
	// ランダム生成
	std::mt19937 mt(seed());
	// 0から99までのランダム生成
	std::uniform_int_distribution<int> range(0, 99);

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="value">命中率</param>
	/// <returns></returns>
	bool Hit(const int value)
	{
		return value > range(mt);
	}
}