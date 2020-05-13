#pragma once
#include "UI.h"

class Charactor;

class StatusWindow :
	public UI
{
private:
	const Charactor& _charactor;

public:
	StatusWindow(std::deque<std::shared_ptr<UI>>& uiDeque, const Charactor& charactor);
	~StatusWindow();

	void Update(const Input& input)override;
	void Draw()override;
};

