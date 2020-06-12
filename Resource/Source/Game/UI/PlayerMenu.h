#pragma once
#include "Menu.h"

class MenuTopInf;

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

	std::deque<std::shared_ptr<UI>> _menuTopDeque;
	std::shared_ptr<MenuTopInf> _menuTop;

public:
	PlayerMenu(std::deque<std::shared_ptr<UI>>& uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl, const unsigned char& turnCnt);
	~PlayerMenu();

	void Update(const Input& input)override;
	void Draw()override;
};

