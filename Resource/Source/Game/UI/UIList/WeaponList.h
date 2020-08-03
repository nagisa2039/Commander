#pragma once
#include <functional>
#include "UIList.h"
#include "../StatusWindow/WeaponWindow.h"

class MoneyUI;

class WeaponList :
    public UIList
{
private:
	uint8_t& _weaponId;
	std::function<void()> _func;
	std::unique_ptr<WeaponWindow> _weaponWindowBefore;
	std::unique_ptr<WeaponWindow> _weaponWindowAfter;
	std::unique_ptr<MoneyUI> _moneyUI;

	void Decision()override;
	void Buy();
	void Back()override;
	void ChengeItem()override;

	uint8_t GetWeaponId();

public:
	WeaponList(const Vector2Int& leftup, uint8_t& weaponId, const uint8_t typeFilter, std::deque<std::shared_ptr<UI>>* uiDeque, std::function<void()> func);
	~WeaponList();

	void Draw()override;
};

