#pragma once
#include <memory>
#include <vector>
#include <array>
#include <functional>
#include "BattleEffectType.h"

class Effect;
class Camera;
class BattleCharactor;

/// <summary>
/// 攻撃エフェクト生成クラス
/// </summary>
class BattleEffectFactory
{
private:
	using CrateBattleEffectFunc_t = std::function<std::shared_ptr<Effect>(
		BattleCharactor&, BattleCharactor&, std::vector<std::shared_ptr<Effect>>&,bool, Camera&, bool)>;

	// 攻撃エフェクト種ごとの生成関数配列
	std::array<CrateBattleEffectFunc_t, static_cast<size_t>(BattleEffectType::max)> _createBattleEffectFuncs;

public:
	BattleEffectFactory();
	~BattleEffectFactory()=default;

	/// <summary>
	/// 攻撃エフェクトの生成
	/// </summary>
	/// <param name="type">攻撃エフェクトの種類</param>
	/// <param name="self">生成した戦闘キャラクター</param>
	/// <param name="target">目標のターゲットキャラクター</param>
	/// <param name="effects">エフェクト管理用配列</param>
	/// <param name="critical">クリティカルフラグ</param>
	/// <param name="camera">カメラ</param>
	/// <param name="cameraActive">カメラを考慮した描画をするか</param>
	/// <returns>生成した攻撃エフェクト</returns>
	std::shared_ptr<Effect> CreateBattleEffect(const BattleEffectType type, BattleCharactor & self, BattleCharactor & target, std::vector<std::shared_ptr<Effect>> & effects,
		bool critical, Camera & camera, bool cameraActive = false)const;
};