#pragma once
#include "Menu.h"
class PlayerMenu :
	public Menu
{
private:
	enum class Content
	{
		situation,
		retreat,
		turnEnd,
		max
	};

public:
	PlayerMenu(std::deque<std::shared_ptr<UI>>& uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl);
	~PlayerMenu();
};

