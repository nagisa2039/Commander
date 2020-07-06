#pragma once
#include <functional>
#include "UIList.h"
#include "../StatusWindow/WeaponWindow.h"

class WeaponList :
    public UIList
{
private:
	uint8_t& _weaponId;
	std::function<void()> _func;
	std::unique_ptr<WeaponWindow> _weaponWindowBefore;
	std::unique_ptr<WeaponWindow> _weaponWindowAfter;

	void Decision()override;
	void Back()override;
	void ChengeItem()override;

	uint8_t GetWeaponId();

public:
	WeaponList(const Vector2Int& leftup, uint8_t& weaponId, std::deque<std::shared_ptr<UI>>* uiDeque, std::function<void()> func);
	~WeaponList();

	void Draw()override;
};

