#pragma once
#include "UI.h"
#include "Astar.h"
#include "TimeLine.h"
#include <memory>

class Charactor;

class BattlePrediction :
	public UI
{
private:
	const Charactor& _selfCharactor;
	const Charactor& _targetCharactor; 
	const Astar::ResultPos& _attackPos;
	std::unique_ptr<Track<float>> _hpAnimAlpha;

public:
	BattlePrediction(const Charactor& self, const Charactor& target, std::deque<std::shared_ptr<UI>> uiDeque, const Astar::ResultPos& attackPos);
	~BattlePrediction();

	void Update(const Input& input)override;
	void Draw()override;
};

