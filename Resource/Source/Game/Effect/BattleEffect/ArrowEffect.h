#pragma once
#include "BattleEffect.h"
#include "TimeLine.h"

/// <summary>
/// 弓攻撃エフェクト
/// </summary>
class ArrowEffect :
	public BattleEffect
{
private:
	// 移動アニメーショントラック
	std::shared_ptr<Track<Vector2Int>> _moveTrack;

public:
	/// <param name="self">生成した戦闘キャラクター</param>
	/// <param name="target">目標のターゲットキャラクター</param>
	/// <param name="effects">エフェクト管理用配列</param>
	/// <param name="critical">クリティカルフラグ</param>
	/// <param name="camera">カメラ</param>
	ArrowEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, 
		bool critical, Camera& camera);
	~ArrowEffect();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(const Input& input)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;
};

