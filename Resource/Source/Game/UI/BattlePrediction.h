#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>
#include "Status.h"
#include "MapChip.h"
#include "Dir.h"

class Charactor;

/// <summary>
/// 戦闘予測
/// </summary>
class BattlePrediction :
	public UI
{
private:
	// 攻撃側キャラクター
	const Charactor& _selfCharactor;
	// 守備側キャラクター
	const Charactor& _targetCharactor;
	// 攻撃距離
	unsigned int _distance;
	// HPアニメーショントラック
	std::unique_ptr<Track<float>> _hpAnimTrack; 

	/// <summary>
	/// 
	/// </summary>
	/// <param name="dir"></param>
	/// <param name="rightAttack"></param>
	/// <param name="selfStatus"></param>
	/// <param name="targetStatus"></param>
	/// <returns></returns>
	int GetChengePoint(const Dir& dir, bool rightAttack, BattleStatus& selfStatus, BattleStatus& targetStatus);

public:
	/// <param name="self">攻撃側キャラクター</param>
	/// <param name="target">守備側キャラクター</param>
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
	/// <param name="attackStartPos">攻撃開始位置</param>
	BattlePrediction(const Charactor& self, const Charactor& target, std::deque<std::shared_ptr<UI>>* uiDeque, const Vector2Int& attackStartPos);
	~BattlePrediction();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(const Input& input)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="drawY"></param>
	/// <param name="windowRect"></param>
	/// <param name="rightAttack"></param>
	/// <param name="fontH"></param>
	void DrawHPBer(int& drawY, const Rect& windowRect, bool rightAttack, int fontH);
};

