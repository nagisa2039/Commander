#pragma once
#include <random>
#include <time.h>

namespace
{
	// �����_���p�V�[�h
	std::random_device seed;
	// �����_������
	std::mt19937 mt(seed());
	// 0����99�܂ł̃����_������
	std::uniform_int_distribution<int> range(0, 99);

	/// <summary>
	/// �����蔻��
	/// </summary>
	/// <param name="value">������</param>
	/// <returns></returns>
	bool Hit(const int value)
	{
		return value > range(mt);
	}
}