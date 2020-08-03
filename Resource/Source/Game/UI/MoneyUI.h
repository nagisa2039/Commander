#pragma once
#include "UI.h"
#include "Geometry.h"

class MoneyUI :
	public UI
{
private:
	Rect _drawRect;
	const unsigned int& _money;

public:
	MoneyUI(const Vector2Int& pos, std::deque<std::shared_ptr<UI>>* uiDeque);

	void Update(const Input& input);
	void Draw();
};

