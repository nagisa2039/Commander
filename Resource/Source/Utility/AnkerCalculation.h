#pragma once
#include "Geometry.h"
#include <array>
#include <functional>

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

class AnkerCalculation
{
public:
	AnkerCalculation();
	~AnkerCalculation();

	// •`‰æ‚Ég—p‚·‚é¶ãÀ•W‚ğæ“¾‚·‚é
	Vector2Int GetDrawPos(const Vector2Int& drawPos, const Size& size, const Anker anker)const;

private:
	std::array< std::function<Vector2Int(const Vector2Int&, const Size&)>, static_cast<size_t>(Anker::max)> _funcs;
};

