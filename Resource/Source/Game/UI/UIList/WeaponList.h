#pragma once
#include "UIList.h"
#include "../WeaponStatusWindow.h"

class WeaponList :
    public UIList
{
private:
	std::unique_ptr<WeaponStatusWindow> _weaponStatusWindow;

	void Decision()override;

public:
	WeaponList(const Vector2Int& leftup, std::deque<std::shared_ptr<UI>>* uiDeque);
	~WeaponList();

	void Draw()override;
};

