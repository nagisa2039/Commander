#pragma once
#include "Geometry.h"
#include <array>
#include <functional>

/// <summary>
/// アンカー種
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
/// アンカーを基に描画時に使用する左上座標を計算する
/// </summary>
class AnkerCalculation
{
private:
	// アンカーごとの計算関数テーブル
	std::array< std::function<Vector2Int(const Vector2Int&, const Size&)>, static_cast<size_t>(Anker::max)> _funcs;

public:
	AnkerCalculation();
	~AnkerCalculation();

	/// <summary>
	/// 描画時に使用する左上座標を取得する
	/// </summary>
	/// <param name="drawPos">描画座標</param>
	/// <param name="size">サイズ</param>
	/// <param name="anker">アンカー</param>
	/// <returns></returns>
	Vector2Int GetDrawPos(const Vector2Int& drawPos, const Size& size, const Anker anker)const;
};

