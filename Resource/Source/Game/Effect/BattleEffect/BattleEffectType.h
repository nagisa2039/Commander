#pragma once

/// <summary>
/// 攻撃エフェクトの種類
/// </summary>
enum class BattleEffectType
{
	// 斬撃
	slash,		
	// 魔法
	magic,		
	// 弓
	arrow,		
	// 回復
	recover,	
	// 失敗
	miss,		
	max
};