#pragma once
#include "UIList.h"
#include "../StatusWindow/WeaponWindow.h"

class WeaponList :
    public UIList
{
private:
	std::unique_ptr<WeaponWindow> _weaponStatusWindow;

public:
	WeaponList(const Vector2Int& leftup, std::deque<std::shared_ptr<UI>>* uiDeque);
	~WeaponList();

	void Draw()override;
};

