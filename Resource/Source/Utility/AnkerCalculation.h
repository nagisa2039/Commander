#pragma once
#include "Geometry.h"
#include <array>
#include <functional>

/// <summary>
/// �A���J�[��
/// </summary>
enum class Anker
{
	leftup,
	leftcenter,
	leftdown,
	centerup,
	center,
	centerdown,
	rightup,
	rightcenter,
	rightdown,
	max,
};

/// <summary>
/// �A���J�[����ɕ`�掞�Ɏg�p���鍶����W���v�Z����
/// </summary>
class AnkerCalculation
{
private:
	// �A���J�[���Ƃ̌v�Z�֐��e�[�u��
	std::array< std::function<Vector2Int(const Vector2Int&, const Size&)>, static_cast<size_t>(Anker::max)> _funcs;

public:
	AnkerCalculation();
	~AnkerCalculation();

	/// <summary>
	/// �`�掞�Ɏg�p���鍶����W���擾����
	/// </summary>
	/// <param name="drawPos">�`����W</param>
	/// <param name="size">�T�C�Y</param>
	/// <param name="anker">�A���J�[</param>
	/// <returns></returns>
	Vector2Int GetDrawPos(const Vector2Int& drawPos, const Size& size, const Anker anker)const;
};

