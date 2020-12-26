#pragma once

/// <summary>
/// 攻撃エフェクトの種類
/// </summary>
enum class BattleEffectType
{
	slash,		// 斬撃
	magic,		// 魔法
	arrow,		// 弓
	recover,	// 回復
	miss,		// 失敗
	max
};