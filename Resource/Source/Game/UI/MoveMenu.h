#pragma once
#include "Menu.h"
#include <list>

class MoveMenu :
	public Menu
{
private:
	enum class Content
	{
		battle,
		wait,
		close,
		max
	};

	bool _isBattle;
	std::list<Vector2Int> _attackPosList;
	std::list<Vector2Int>::iterator _attackPosListItr;

	void Decision()override;
	void Back()override;
	void OpenUpdate(const Input& input)override;

public:
	MoveMenu(std::deque<std::shared_ptr<UI>>* uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl);
	~MoveMenu();

	void SetContent(const std::list<Vector2Int>& attackPosList);

	void Draw()override;
};

