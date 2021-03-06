#pragma once
#include "BattleEffect.h"

/// <summary>
/// 魔法攻撃エフェクト
/// </summary>
class MagicEffect :
	public BattleEffect
{
public:
	/// <param name="self">生成した戦闘キャラクター</param>
	/// <param name="target">目標のターゲットキャラクター</param>
	/// <param name="effects">エフェクト管理用配列</param>
	/// <param name="critical">クリティカルフラグ</param>
	/// <param name="camera">カメラ</param>
	MagicEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, 
		bool critical, Camera& camera);
	~MagicEffect();
};

