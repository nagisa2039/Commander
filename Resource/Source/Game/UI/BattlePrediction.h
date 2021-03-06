#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>
#include "Status.h"
#include "MapChip.h"
#include "Dir.h"

class Charactor;

/// <summary>
/// ν¬\ͺ
/// </summary>
class BattlePrediction :
	public UI
{
private:
	// U€LN^[
	const Charactor& _selfCharactor;
	// ηυ€LN^[
	const Charactor& _targetCharactor;
	// U£
	unsigned int _distance;
	// HPAj[VgbN
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
	/// <param name="self">U€LN^[</param>
	/// <param name="target">ηυ€LN^[</param>
	/// <param name="uiDeque">UIΜeqΦWΗDeque</param>
	/// <param name="attackStartPos">UJnΚu</param>
	BattlePrediction(const Charactor& self, const Charactor& target, std::deque<std::shared_ptr<UI>>* uiDeque, const Vector2Int& attackStartPos);
	~BattlePrediction();

	/// <summary>
	/// XV
	/// </summary>
	/// <param name="input">όΝξρ</param>
	void Update(const Input& input)override;

	/// <summary>
	/// `ζ
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

