#pragma once
#include "UI.h"
#include "Astar.h"
#include "TimeLine.h"
#include <memory>
#include "Status.h"
#include "MapChip.h"

class Charactor;

class BattlePrediction :
	public UI
{
private:
	const Charactor& _selfCharactor;
	const Charactor& _targetCharactor; 
	unsigned int _distance;
	const Map_Chip _mapChip;
	std::unique_ptr<Track<float>> _hpAnimAlpha; 
	int GetChengePoint(const Dir& dir, bool rightAttack, BattleStatus& selfStatus, BattleStatus& targetStatus);

public:
	BattlePrediction(const Charactor& self, const Charactor& target, std::deque<std::shared_ptr<UI>> uiDeque, const Vector2Int& attackStartPos, const Map_Chip mapChip);
	~BattlePrediction();

	void Update(const Input& input)override;
	void Draw()override;

	void DrawHPBer(int& drawY, const Rect& windowRect, bool rightAttack, int fontH);
};

