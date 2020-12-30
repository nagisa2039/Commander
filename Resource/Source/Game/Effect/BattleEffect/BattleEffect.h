#pragma once
#include "../Effect.h"
#include <vector>

class BattleCharactor;

/// <summary>
/// 攻撃時用エフェクト
/// </summary>
class BattleEffect :
	public Effect
{
protected:
	// エフェクト管理用配列
	std::vector<std::shared_ptr<Effect>>& _effects;
	// 生成した戦闘キャラクター
	BattleCharactor& _self;
	// 目標のターゲットキャラクター
	BattleCharactor& _target; 
	// クリティカルフラグ
	const bool _critical;

	/// <summary>
	/// 目標キャラクターにダメージを与える
	/// </summary>
	/// <returns>ダメージ量</returns>
	unsigned int AddDamage();

public:
	/// <param name="self">生成した戦闘キャラクター</param>
	/// <param name="target">目標のターゲットキャラクター</param>
	/// <param name="effects">エフェクト管理用配列</param>
	/// <param name="critical">クリティカルフラグ</param>
	/// <param name="camera">カメラ</param>
	/// <param name="cameraActive">カメラを考慮した描画をするか</param>
	BattleEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects,
		bool critical, Camera& camera, bool cameraActive = false);
	virtual ~BattleEffect();
};

