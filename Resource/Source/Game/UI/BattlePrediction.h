#pragma once
#include "UI.h"
#include "Astar.h"
#include "TimeLine.h"
#include <memory>
#include "Status.h"

class Charactor;

class BattlePrediction :
	public UI
{
private:
	const Charactor& _selfCharactor;
	const Charactor& _targetCharactor; 
	const unsigned int _distance;
	std::unique_ptr<Track<float>> _hpAnimAlpha; 
	int GetChengePoint(const Dir& dir, bool rightAttack, Status& selfStatus, Status& targetStatus);

public:
	BattlePrediction(const Charactor& self, const Charactor& target, std::deque<std::shared_ptr<UI>> uiDeque, const unsigned int distance);
	~BattlePrediction();

	void Update(const Input& input)override;
	void Draw()override;
	
};

