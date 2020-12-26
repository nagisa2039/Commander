#pragma once
#include "BattleEffect.h"

/// <summary>
/// 斬撃攻撃エフェクト
/// </summary>
class SlashingEffect :
	public BattleEffect
{
public:
	/// <param name="self">生成した戦闘キャラクター</param>
	/// <param name="target">目標のターゲットキャラクター</param>
	/// <param name="effects">エフェクト管理用配列</param>
	/// <param name="critical">クリティカルフラグ</param>
	/// <param name="camera">カメラ</param>
	SlashingEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, 
		bool critical, Camera& camera);
	~SlashingEffect();
};

